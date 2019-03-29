#include "Weapon_Blaster.h"
#include "Veicolo.h"
#include "misc/Cgdi.h"
#include "MondoDiGioco.h"
#include "Raven_Map.h"
//#include "../lua/Raven_Scriptor.h"
#include "FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Blaster::Blaster(Veicolo*   owner):

                      Raven_Weapon(type_blaster,
                                   0,//script->GetInt("Blaster_DefaultRounds"),
                                   0,//script->GetInt("Blaster_MaxRoundsCarried"),
                                   3,//script->GetDouble("Blaster_FiringFreq"),
                                   50,//script->GetDouble("Blaster_IdealRange"),
                                   5,//script->GetDouble("Bolt_MaxSpeed"),
                                   owner)
{
  //setup the vertex buffer
  const int NumWeaponVerts = 4;
  const Vector2D weapon[NumWeaponVerts] = {Vector2D(0, -1),
                                           Vector2D(10, -1),
                                           Vector2D(10, 1),
                                           Vector2D(0, 1)
                                           };

  
  for (int vtx=0; vtx<NumWeaponVerts; ++vtx)
  {
    m_vecWeaponVB.push_back(weapon[vtx]);
  }

  //setup the fuzzy module
  InitializeFuzzyModule();
}


//------------------------------ ShootAt --------------------------------------

inline void Blaster::Spara(Vector2D pos)
{ 
  if (isReadyForNextShot())
  {
    //fire!
    m_pOwner->GetWorld()->AggiungiProiettileLeggero(m_pOwner, pos);

    UpdateTimeWeaponIsNextAvailable();

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner,100);// script->GetDouble("Blaster_SoundRange"));
  }
}



//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double Blaster::GetDesirability(double DistToTarget)
{
  //fuzzify distance and amount of ammo
  m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);

  m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

  return true;
}

//----------------------- InitializeFuzzyModule -------------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Blaster::InitializeFuzzyModule()
{
  FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

  FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close",0,25,150);
  FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium",25,150,300);
  FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far",150,300,1000);

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability"); 
  FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

  m_FuzzyModule.AddRule(Target_Close, Desirable);
  m_FuzzyModule.AddRule(Target_Medium, FzVery(Undesirable));
  m_FuzzyModule.AddRule(Target_Far, FzVery(Undesirable));
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Blaster::Render()
{
   m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->GreenPen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}