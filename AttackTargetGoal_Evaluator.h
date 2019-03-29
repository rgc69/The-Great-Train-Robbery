#ifndef RAVEN_ATTACK_GOAL_EVALUATOR
#define RAVEN_ATTACK_GOAL_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   AttackTargetGoal_Evaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:  class to calculate how desirable the goal of attacking the bot's
//         current target is
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "Veicolo.h"


class AttackTargetGoal_Evaluator : public Goal_Evaluator
{ 
public:

  AttackTargetGoal_Evaluator(double bias):Goal_Evaluator(bias){}
  
  double CalculateDesirability(Veicolo* pBot);

  void  SetGoal(Veicolo* pEnt);

  void RenderInfo(Vector2D Position, Veicolo* pBot);
};



#endif