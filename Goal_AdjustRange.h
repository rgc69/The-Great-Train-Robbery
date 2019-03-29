#ifndef GOAL_ADJUST_RANGE_H
#define GOAL_ADJUST_RANGE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_AdjustRange.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "Goal.h"
#include "Raven_Goal_Types.h"
#include "Veicolo.h"





class Goal_AdjustRange : public Goal<Veicolo>
{
private:

  Veicolo*  m_pTarget;

  double       m_dIdealRange;

public:

  Goal_AdjustRange(Veicolo* pBot);

  void Activate();

  int  Process();

  void Terminate();
 
};






#endif
