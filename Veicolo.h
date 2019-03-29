#ifndef VEICOLO_H
#define VEICOLO_H
#pragma warning (disable:4786)

#include "MovingEntity.h"
#include "2d/Vector2D.h"
#include "misc/Smoother.h"
#include "Raven_TargetingSystem.h"
#include <vector>
#include <list>
#include <string>
class Regulator;
class MondoDiGioco;
class Raven_PathPlanner;
class SteeringBehavior;
class Raven_Steering;
class SistemaArmi;
class Raven_Weapon;
class SpotIncidente;
class Raven_SensoryMemory;
class Goal_Think;

class Veicolo : public MovingEntity
{
private:
  bool                               m_bHit;

  enum Status{alive, dead, spawning};

  //alive, dead or spawning?
  Status                             m_Status;

  //La classe che rappresenta la "memoria" del veicolo. Ogni volta che un
  //veicolo vede o sente un nemico,un record dell'evento è inserito nella memoria.
  Raven_SensoryMemory*               m_pSensoryMem;

 //questa è la classe responsabile della scelta dell'obiettivo
  Raven_TargetingSystem*             m_pTargSys;

  //the bot uses this to plan paths
  Raven_PathPlanner*                 m_pPathPlanner;

    //A regulator object limits the update frequency of a specific AI component
 // Regulator*                         m_pWeaponSelectionRegulator;
    Regulator*                         m_pGoalArbitrationRegulator;
    Regulator*                         m_pTargetSelectionRegulator;
    Regulator*                         m_pTriggerTestRegulator;
    Regulator*                         m_pVisionUpdateRegulator;

  //a pointer to the world data. So a vehicle can access any obstacle,
  //path, wall or agent data
  MondoDiGioco*            m_pWorld;

  //the steering behavior class
  SteeringBehavior*     m_pSteering;

  //the bot uses this object to steer
  Raven_Steering*    m_pSteeringNonNormale;

//the direction the bot is facing (and therefore the direction of aim). 
  //Note that this may not be the same as the bot's heading, which always
  //points in the direction of the bot's movement
  Vector2D                           m_vFacing;

   //a bot only perceives other bots within this field of view
  double                             m_dFieldOfView;

  //some steering behaviors give jerky looking movement. The
  //following members are used to smooth the vehicle's heading
  Smoother<Vector2D>*  m_pHeadingSmoother;

  //this vector represents the average of the vehicle's heading
  //vector smoothed over the last few frames
  Vector2D             m_vSmoothedHeading;

  //when true, smoothing is active
  bool                  m_bSmoothingOn;

 
   //this object handles the arbitration and processing of high level goals
  Goal_Think*                        m_pBrain;
 

  //keeps a track of the most recent update time. (some of the
  //steering behaviors make use of this - see Wander)
  double                m_dTimeElapsed;

    
  //to show that a player has been hit it is surrounded by a thick 
  //red circle for a fraction of a second. This variable represents
  //the number of update-steps the circle gets drawn
  int                                m_iNumUpdatesHitPersistant;

 //set to true when the bot is hit, and remains true until 
  //m_iNumUpdatesHitPersistant becomes zero. (used by the render method to
  //draw a thick red circle around a bot to indicate it's been hit)



  //buffer for the vehicle shape
  std::vector<Vector2D> m_vecVehicleVB;

  //fills the buffer with vertex data
  void InitializeBuffer();

  //disallow the copying of Vehicle types
  Veicolo(const Veicolo&);
  Veicolo& operator=(const Veicolo&);


public:
   //the bot's health. Every time the bot is shot this value is decreased. If
  //it reaches zero then the bot dies (and respawns) Questo è da rimettere in Private
  int                                m_iHealth;

  Veicolo(MondoDiGioco* world,
         Vector2D position,
         double    rotation,
         Vector2D velocity,
         double    mass,
         double    max_force,
         double    max_speed,
         double    max_turn_rate,
      double    scale);
    Veicolo(MondoDiGioco* world, Vector2D pos);
  virtual ~Veicolo();
//  ~Veicolo();


   //the bot's maximum health value. It starts its life with health at this value
  int                                m_iMaxHealth;

   //each time this bot kills another this value is incremented
  int                                m_iScore;

  //updates the vehicle's position and orientation
  void          Update(double time_elapsed);
  Vector2D      Facing()const{return m_vFacing;}
  bool          HandleMessage(const Telegram& msg);
  double        FieldOfView()const{return m_dFieldOfView;}
   //this rotates the bot's heading until it is facing directly at the target
  //position. Returns false if not facing at the target.
  bool          RotateFacingTowardPosition(Vector2D target);
  int           Health()const{return m_iHealth;}
  int           MaxHealth()const{return m_iMaxHealth;}
  void          ReduceHealth(unsigned int val);
  void          RestoreHealthToMaximum();
  void          IncreaseHealth(unsigned int val);
  bool          isDead()const{return m_Status == dead;}
  bool          isAlive()const{return m_Status == alive;}
  bool          isSpawning()const{return m_Status == spawning;}
  void          SetSpawning(){m_Status = spawning;}
  void          SetDead(){m_Status = dead;}
  void          SetAlive(){m_Status = alive;}
  int           Score()const{return m_iScore;}
  void          IncrementScore(){++m_iScore;}
    //returns a value indicating the time in seconds it will take the bot
  //to reach the given position at its current speed.
  double        CalculateTimeToReachPosition(Vector2D pos)const; 
  void          Render();
  MondoDiGioco* const                  GetWorld(){return m_pWorld;} 
                                                                 
  //-------------------------------------------accessor methods
  SteeringBehavior*const  Steering()const{return m_pSteering;}
  Raven_Steering* const              GetSteering(){return m_pSteeringNonNormale;}
  MondoDiGioco*const         World()const{return m_pWorld;} 

     //this handles all the weapons. and has methods for aiming, selecting and
  //shooting them
  SistemaArmi*                m_pWeaponSys;
  Vector2D    SmoothedHeading()const{return m_vSmoothedHeading;}

  //------------------------ isAtPosition ----------------------------------------
 //
 //returns true if the vehicle is close to the given position
  bool          isAtPosition(Vector2D pos)const;

  
    //------------------------ isAtPositionProx ----------------------------------------
 //
 //returns true if the vehicle is close to the given position
  bool          isAtPositionProx(Vector2D pos)const;
  //------------------------ isAtPositionLargest ----------------------------------------
 //
 //returns true if the vehicle is close to the given position
  bool          isAtPositionLargest(Vector2D pos)const;
//--------------------------------------------------------------------------------------------

  //------------------------ isAtPositionLarghissima ----------------------------------------
 //
 //returns true if the vehicle is close to the given position
  bool          isAtPositionLarghissima(Vector2D pos)const;
//--------------------------------------------------------------------------------------------
 SistemaArmi* const          GetWeaponSys()const{return m_pWeaponSys;}
 Raven_SensoryMemory* const         GetSensoryMem()const{return m_pSensoryMem;}
 const Raven_TargetingSystem* const GetTargetSys()const{return m_pTargSys;}
 Raven_TargetingSystem* const       GetTargetSys(){return m_pTargSys;}
 Veicolo* const                   GetTargetBot()const{return m_pTargSys->GetTarget();}
  //spawns the bot at the given position
  void          Spawn(Vector2D pos);
  
  //returns true if the bot has line of sight to the given position.
  bool          hasLOSto(Vector2D pos)const;
    //returns true if this bot can move directly to the given position
  //without bumping into any walls
  bool          canWalkTo(Vector2D pos)const;
  Raven_PathPlanner* const           GetPathPlanner(){return m_pPathPlanner;}
  //similar to above. Returns true if the bot can move between the two
  //given positions without bumping into any walls
  bool          canWalkBetween(Vector2D from, Vector2D to)const;
    //interface for human player
  void          FireWeapon(Vector2D pos);
  Goal_Think* const                  GetBrain(){return m_pBrain;}
  bool        isSmoothingOn()const{return m_bSmoothingOn;}
  void        SmoothingOn(){m_bSmoothingOn = true;}
  void        SmoothingOff(){m_bSmoothingOn = false;}
  void        ToggleSmoothing(){m_bSmoothingOn = !m_bSmoothingOn;}
    //returns true if this bot is ready to test against all triggers
  bool          isReadyForTriggerUpdate()const;
  double       TimeElapsed()const{return m_dTimeElapsed;}

 
};



#endif