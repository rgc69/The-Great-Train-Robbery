#ifndef RAVEN_HEALTH_EVALUATOR
#define RAVEN_HEALTH_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GetHealthGoal_Evaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to calculate how desirable the goal of fetching a health item
//          is
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "Veicolo.h"

class GetHealthGoal_Evaluator : public Goal_Evaluator
{
public:

  GetHealthGoal_Evaluator(double bias):Goal_Evaluator(bias){}
  
  double CalculateDesirability(Veicolo* pBot);

  void  SetGoal(Veicolo* pEnt);

  void RenderInfo(Vector2D Position, Veicolo* pBot);
};

#endif
    
