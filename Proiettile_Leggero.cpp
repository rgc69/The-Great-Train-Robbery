#include "Proiettile_Leggero.h"
#include "Raven_Scriptor.h"
#include "misc/cgdi.h"
#include "Veicolo.h"
#include "MondoDiGioco.h"
#include "constants.h"
#include "WallIntersectionTests.h"
#include "Raven_Map.h"

#include "Raven_Messages.h"
#include "MessageDispatcher.h"
#include "misc/Stream_Utility_Functions.h"

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
ProiettileLeggero::ProiettileLeggero(Veicolo* shooter, Vector2D target):

        Proiettile(target,
                         shooter->GetWorld(),
                         shooter->ID(),
                         shooter->Pos(),
                         shooter->Facing(),
                         1,//script->GetInt("Bolt_Damage"),
                         10,//m_pOwner->Scale(),//script->GetDouble("Bolt_Scale"),
                         8,//script->GetDouble("Bolt_MaxSpeed"),
                         1,//script->GetDouble("Bolt_Mass"),
                         100.0)//script->GetDouble("Bolt_MaxForce"))
{
   assert (target != Vector2D());
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void ProiettileLeggero::Update()
{
		
  if (!m_bImpacted)
  {
    m_vVelocity = MaxSpeed() * Heading();

    //make sure vehicle does not exceed maximum velocity
    m_vVelocity.Truncate(m_dMaxSpeed);

    //update the position
    m_vPos += m_vVelocity;

    
    //if the projectile has reached the target position or it hits an entity
    //or wall it should explode/inflict damage/whatever and then mark itself
    //as dead


    //test to see if the line segment connecting the bolt's current position
    //and previous position intersects with any bots.
    Veicolo* hit = GetClosestIntersectingVeicolo(m_vPos - m_vVelocity,
                                               m_vPos);

	//Settiamo il TimeToLive del proiettile
	if (Clock->GetCurrentTime() - m_dTimeOfCreation > 0.9)
    {
      	m_bDead = true;
    }
	
  
    //if hit
    if (hit)
    {
      m_bImpacted = true;
      m_bDead     = true;

      //send a message to the bot to let it know it's been hit, and who the
      //shot came from
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              m_iShooterID,
                              hit->ID(),
                              Msg_Colpito,
                              (void*)&m_iDamageInflicted);
    }


    //test for impact with a wall
    double dist;
     if( FindClosestPointOfIntersectionWithWalls(m_vPos - m_vVelocity,
                                                 m_vPos,
                                                 dist,
                                                 m_vImpactPoint,
                                                 m_pWorld->GetMap()->GetWalls()))
     {
       m_bDead     = true;
       m_bImpacted = true;

       m_vPos = m_vImpactPoint;

       return;
     }
  }
}


//-------------------------- Render -------------------------------------------
//-----------------------------------------------------------------------------
void ProiettileLeggero::Render()
{
  gdi->ThickBluePen();
  gdi->Line(Pos(), Pos()- Velocity()/2);
   
}

