#ifndef RAVEN_WEAPON_EVALUATOR
#define RAVEN_WEAPON_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GetWeaponGoal_Evaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:  class to calculate how desirable the goal of fetching a weapon item
//         is 
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "Veicolo.h"


class GetWeaponGoal_Evaluator : public Goal_Evaluator
{ 
  int   m_iWeaponType;

public:

  GetWeaponGoal_Evaluator(double bias,
                          int   WeaponType):Goal_Evaluator(bias),
                                            m_iWeaponType(WeaponType)
  {}
  
  double CalculateDesirability(Veicolo* pBot);

  void  SetGoal(Veicolo* pEnt);

  void  RenderInfo(Vector2D Position, Veicolo* pBot);
};

#endif
  