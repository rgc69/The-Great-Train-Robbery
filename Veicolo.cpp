#include "Veicolo.h"
#include "2d/C2DMatrix.h"
#include "2d/Geometry.h"
#include "SistemaMovimento.h"
#include "Raven_SteeringBehaviors.h"
#include "2d/Transformations.h"
#include "MondoDiGioco.h"
#include "misc/CellSpacePartition.h"
#include "misc/cgdi.h"
#include "SistemaDiArma.h"
#include "Raven_Scriptor.h"
#include "Scriptor.h"
#include "Telegram.h"
#include "Raven_Messages.h"
#include "MessageDispatcher.h"
#include "Raven_SensoryMemory.h"
#include "Regulator.h"
#include "DebugConsole.h"
#include "Raven_Goal_Types.h"
#include "Goal_Think.h"
using std::vector;
using std::list;

//----------------------------- ctor -------------------------------------
//------------------------------------------------------------------------
Veicolo::Veicolo(MondoDiGioco* world,
               Vector2D position,
               double    rotation,
               Vector2D velocity,
               double    mass,
               double    max_force,
               double    max_speed,
               double    max_turn_rate,
               double    scale):    MovingEntity(position,
                                                 scale,
                                                 velocity,
                                                 max_speed,
                                                 Vector2D(sin(rotation),-cos(rotation)),
                                                 mass,
                                                 Vector2D(scale,scale),
                                                 0.3,
                                                 max_force),

                                       m_pWorld(world),
									   m_pBrain(NULL),
                                       m_vSmoothedHeading(Vector2D(0,0)),
                                       m_bSmoothingOn(false),
                                       m_dTimeElapsed(0.0),
									   m_bHit(false),
									   m_iScore(0),
									   m_Status(spawning),
									   m_pPathPlanner(NULL),
									   m_iHealth(3),
									   m_iMaxHealth(5),
									   m_iNumUpdatesHitPersistant((int)(5))
{  
	
  InitializeBuffer();

  SetEntityType(type_bot);

  //set up the steering behavior class
  m_pSteering = new SteeringBehavior(this);
  
  //create the navigation module
  m_pPathPlanner = new Raven_PathPlanner(this);
 
  //set up the smoother
  m_pHeadingSmoother = new Smoother<Vector2D>(Prm.NumCicliPerAddolcire, Vector2D(0.0, 0.0)); 
  
    //crea il sistema di puntamento
  m_pTargSys = new Raven_TargetingSystem(this);

  m_pWeaponSys = new SistemaArmi(this,
                                       0.2,// script->GetDouble("Bot_ReactionTime"),
                                        0.0,//script->GetDouble("Bot_AimAccuracy"),
                                        1);//script->GetDouble("Bot_AimPersistance"));
   
  //Il veicolo parte con la prua rivolta nella direzione del vettore direzione.
  //Fondamentale per la renderizzazione del cannoncino, e per la sua rotazione.
    m_vFacing = m_vHeading;

	  //crea i regolatori
  //m_pWeaponSelectionRegulator = new Regulator(2);
   m_pGoalArbitrationRegulator =  new Regulator(4);
   m_pTargetSelectionRegulator = new Regulator(2);
   m_pTriggerTestRegulator = new Regulator(8);
   m_pVisionUpdateRegulator = new Regulator(8);

  //create the goal queue
   m_pBrain = new Goal_Think(this);

  //per quanti secondi il veicolo mantiene memoria dei dati del mondo di gioco
  m_pSensoryMem = new Raven_SensoryMemory(this,5);
}
  

//---------------------------- dtor -------------------------------------
//-----------------------------------------------------------------------
Veicolo::~Veicolo()
{

//  debug_con << "Cancello Veicolo (id = " << ID() << ")" << "";
  delete m_pSteering;
  delete m_pPathPlanner;
  delete m_pHeadingSmoother;
  delete m_pWeaponSys;
  delete m_pTargSys;
  //delete m_pWeaponSelectionRegulator;
  delete m_pGoalArbitrationRegulator;
  delete m_pTargetSelectionRegulator;
  delete m_pTriggerTestRegulator;
  delete m_pVisionUpdateRegulator;
  delete m_pBrain;
  delete m_pSensoryMem;
}

//------------------------------- Spawn ---------------------------------------
//
//  spawns the bot at the given position
//-----------------------------------------------------------------------------
void Veicolo::Spawn(Vector2D pos)
{
    SetAlive();
    SetPos(pos);
    m_pWeaponSys->Initialize();
	m_pBrain->RemoveAllSubgoals();
    RestoreHealthToMaximum();
    m_pTargSys->ClearTarget();
}

//---------------------------- isReadyForTriggerUpdate ------------------------
//
//  returns true if the bot is ready to be tested against the world triggers
//-----------------------------------------------------------------------------
bool Veicolo::isReadyForTriggerUpdate()const
{
 return m_pTriggerTestRegulator->isReady();
}

//--------------------------- HandleMessage -----------------------------------
//-----------------------------------------------------------------------------
bool Veicolo::HandleMessage(const Telegram& msg)
{
  //first see if the current goal accepts the message
  if (GetBrain()->HandleMessage(msg)) return true;
 
  //handle any messages not handles by the goals
  switch(msg.Msg)
  {
  case Msg_Colpito:

    //just return if already dead or spawning
    if (isDead()) return true;

    //the extra info field of the telegram carries the amount of damage
    ReduceHealth(DereferenceToType<int>(msg.ExtraInfo));

    //if this bot is now dead let the shooter know
    if (isDead())
    {
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              ID(),
                              msg.Sender,
                              Msg_Ucciso,
                              NO_ADDITIONAL_INFO);
    }

    return true;

  case Msg_Ucciso:
    
    IncrementScore();
    
    //il veicolo ucciso deve essere rimosso come obiettivo
    m_pTargSys->ClearTarget();

    return true;

  case Msg_RumoreArmaDaFuoco:

    //Aggiunge la fonte del rumore alla "memoria" dell'entità
   GetSensoryMem()->UpdateWithSoundSource((Veicolo*)msg.ExtraInfo);

    return true;

  case Msg_UserHasRemovedBot:
    {

      Veicolo* pRemovedBot = (Veicolo*)msg.ExtraInfo;

      GetSensoryMem()->RemoveBotFromMemory(pRemovedBot);

      //if the removed bot is the target, make sure the target is cleared
      if (pRemovedBot == GetTargetSys()->GetTarget())
      {
        GetTargetSys()->ClearTarget();
      }

      return true;
    }


  default: return false;
  }
}

//------------------ RotateFacingTowardPosition -------------------------------
//
//  given a target position, this method rotates the bot's facing vector
//  by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//----------------------------------------------------------------------------
bool Veicolo::RotateFacingTowardPosition(Vector2D target)
{
  Vector2D toTarget = Vec2DNormalize(target - Pos());

  double dot = m_vFacing.Dot(toTarget);

  //clamp to rectify any rounding errors
  Clamp(dot, -1, 1);

  //determine the angle between the heading vector and the target
  double angle = acos(dot);

  //return true if the bot's facing is within WeaponAimTolerance degs of
  //facing the target
  const double WeaponAimTolerance = 0.01; //2 degs approx

  if (angle < WeaponAimTolerance)
  {
    m_vFacing = toTarget;
    return true;
  }

  //clamp the amount to turn to the max turn rate
  if (angle > m_dMaxTurnRate) angle = m_dMaxTurnRate;
  
  //The next few lines use a rotation matrix to rotate the player's facing
  //vector accordingly
  C2DMatrix RotationMatrix;
  
  //notice how the direction of rotation has to be determined when creating
  //the rotation matrix
  RotationMatrix.Rotate(angle * m_vFacing.Sign(toTarget));	
  RotationMatrix.TransformVector2Ds(m_vFacing);

  return false;
}


//--------------------------------- ReduceHealth ----------------------------
void Veicolo::ReduceHealth(unsigned int val)
{
  m_iHealth -= val;

  if (m_iHealth <= 0)
  {
    SetDead();
  }

  m_bHit = true;

  m_iNumUpdatesHitPersistant = (int)(10);
}

//---------------------------- FireWeapon -------------------------------------
//
//  fires the current weapon at the given position
//-----------------------------------------------------------------------------
void Veicolo::FireWeapon(Vector2D pos)
{
  m_pWeaponSys->Spara(pos);
}

//------------------------------ Update ----------------------------------
//
//  Updates the vehicle's position from a series of steering behaviors
//------------------------------------------------------------------------
void Veicolo::Update(double time_elapsed)
{    
	 //process the currently active goal. Note this is required even if the bot
  //is under user control. This is because a goal is created whenever a user 
  //clicks on an area of the map that necessitates a path planning request.
  //m_pBrain->Process();

  //update the time elapsed
  m_dTimeElapsed = time_elapsed;
  if (isDead()) return;
  //keep a record of its old position so we can update its cell later
  //in this method
  Vector2D OldPos = Pos();


  Vector2D ForzaMovimento ;

  //calculate the combined force from each steering behavior in the 
  //vehicle's list
  ForzaMovimento  = m_pSteering->Calculate();
    
  //Acceleration = Force/Mass
  Vector2D acceleration = ForzaMovimento  / m_dMass;

  //update velocity
  m_vVelocity += acceleration * time_elapsed; 

  //make sure vehicle does not exceed maximum velocity
  m_vVelocity.Truncate(m_dMaxSpeed);

  //update the position
  m_vPos += m_vVelocity * time_elapsed;

  //update the heading if the vehicle has a non zero velocity
  if (m_vVelocity.LengthSq() > 0.00000001)
  {    
    m_vHeading = Vec2DNormalize(m_vVelocity);

    m_vSide = m_vHeading.Perp();
  }

  //EnforceNonPenetrationConstraint(this, World()->Agents());

  //treat the screen as a toroid
  WrapAround(m_vPos, m_pWorld->cxClient(), m_pWorld->cyClient());

  //update the vehicle's current cell if space partitioning is turned on
  if (Steering()->isSpacePartitioningOn())
  {
    World()->CellSpace()->UpdateEntity(this, OldPos);
  }

  if (isSmoothingOn())
  {
    m_vSmoothedHeading = m_pHeadingSmoother->Update(Heading());
  }

if(ID() != 7)
 {
  //esamina gli opponenti nel sistema di memoria del Veicolo
  //e ne seleziona uno come obiettivo corrente
    if (m_pTargetSelectionRegulator->isReady())
    {      
      m_pTargSys->Update();
    }
    //update the sensory memory with any visual stimulus
    if (m_pVisionUpdateRegulator->isReady())
    {
      m_pSensoryMem->UpdateVision();
    }
	 //this method aims the bot's current weapon at the current target
    //and takes a shot if a shot is possible
    m_pWeaponSys->TakeAimAndShoot();

	  //appraise and arbitrate between all possible high level goals
    if (m_pGoalArbitrationRegulator->isReady())
    {
       m_pBrain->Arbitrate(); 
    }

 }  
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Veicolo::Render()
{ 

  //quando un treno viene colpito da un proiettile questo valore viene
  //settato per stabilire per quanto tempo il cerchio rosso deve circondare
  //il veicolo per indicare che è stato colpito. Il circolo viene renderizzato
  //fino a quando questo valore è positivo
	m_iNumUpdatesHitPersistant--;
  if (isDead() || isSpawning()) return;
 
  //renderizza il cerchio rosso per indicare che il veicolo è stato colpito
  if (m_bHit &&  m_iHealth > 0 &&  m_iHealth < 5)
  {
    gdi->ThickRedPen();
    gdi->HollowBrush();
    gdi->Circle(Pos(), 30);
	  
	if (m_iNumUpdatesHitPersistant <= 0)
    {
      m_bHit = false;
    }
  }
    

  //un vettore per contenere i vertici trasformati
  static std::vector<Vector2D>  m_vecVehicleVBTrans;

	    
       gdi->BluePen();
	   
  
    m_vecVehicleVBTrans = WorldTransform(m_vecVehicleVB,
                                         Pos(),
                                         Heading(),
                                         Side(),
                                         Scale());
  

  gdi->ClosedShape(m_vecVehicleVBTrans);

   gdi->BluePen();
	 
	 //Disegna il circoletto per la base del cannoncino  
	   gdi->YellowBrush();
       gdi->Circle((Pos()), 0.5 * Scale().x);
	    
	   //renderizza l'arma del veicolo
    m_pWeaponSys->RenderCurrentWeapon();
  

  //render di ogni menu
  if (m_pWorld->ViewKeys())
  {
    Steering()->RenderAids();
  }

}


//----------------------------- InitializeBuffer -----------------------------
//
//  fills the vehicle's shape buffer with its vertices
//-----------------------------------------------------------------------------
void Veicolo::InitializeBuffer()
{
  const int NumVehicleVerts = 4;
  if (isDead()) return;
  Vector2D vehicle[NumVehicleVerts] = { Vector2D(-1.0f,0.6f),
                                        Vector2D(2.0f,0.6f),										
                                        Vector2D(2.0f,-0.6f),
                                        Vector2D(-1.0f,-0.6f)                                ,
                                       };

  //setup the vertex buffers and calculate the bounding radius
  for (int vtx=0; vtx<NumVehicleVerts; ++vtx)
  {
    m_vecVehicleVB.push_back(vehicle[vtx]);
  }
   
}

void Veicolo::IncreaseHealth(unsigned int val)
{
  m_iHealth+=val; 
  Clamp(m_iHealth, 0, m_iMaxHealth);
}

//----------------- CalculateExpectedTimeToReachPosition ----------------------
//
//  returns a value indicating the time in seconds it will take the bot
//  to reach the given position at its current speed.
//-----------------------------------------------------------------------------
double Veicolo::CalculateTimeToReachPosition(Vector2D pos)const
{
  return Vec2DDistance(Pos(), pos) / (MaxSpeed() * FrameRate);
}

//------------------------ isAtPosition ---------------------------------------
//
//  returns true if the vehicle is close to the given position
//-----------------------------------------------------------------------------
bool Veicolo::isAtPosition(Vector2D pos)const
{
  const static double tolerance = 10.0;
  
  return Vec2DDistanceSq(Pos(), pos) < tolerance * tolerance;
}

//------------------------ isAtPositionProx ---------------------------------------
//
//  returns true if the vehicle is close to the given position
//-----------------------------------------------------------------------------
bool Veicolo::isAtPositionProx(Vector2D pos)const
{
  const static double tolerance = 15.0;
  
  return Vec2DDistanceSq(Pos(), pos) < tolerance * tolerance;
}

//------------------------ isAtPositionLargest ---------------------------------------
//
//  returns true if the vehicle is close to the given position
//-----------------------------------------------------------------------------
bool Veicolo::isAtPositionLargest(Vector2D pos)const
{
  const static double tolerance = 30.0;
  
  return Vec2DDistanceSq(Pos(), pos) < tolerance * tolerance;
}

//------------------------ isAtPositionLarghissima ---------------------------------------
//
//  returns true if the vehicle is close to the given position
//-----------------------------------------------------------------------------
bool Veicolo::isAtPositionLarghissima(Vector2D pos)const
{
  const static double tolerance = 100.0;
  
  return Vec2DDistanceSq(Pos(), pos) < tolerance * tolerance;
}

void Veicolo::RestoreHealthToMaximum(){m_iHealth = m_iMaxHealth;}

//------------------------- hasLOSt0 ------------------------------------------
//
//  returns true if the bot has line of sight to the given position.
//-----------------------------------------------------------------------------
bool Veicolo::hasLOSto(Vector2D pos)const
{
  return m_pWorld->isLOSOkay(Pos(), pos);
}

//returns true if this bot can move directly to the given position
//without bumping into any walls
bool Veicolo::canWalkTo(Vector2D pos)const
{
  return !m_pWorld->isPathObstructed(Pos(), pos, BRadius());
}

//similar to above. Returns true if the bot can move between the two
//given positions without bumping into any walls
bool Veicolo::canWalkBetween(Vector2D from, Vector2D to)const
{
 return !m_pWorld->isPathObstructed(from, to, BRadius());
}
