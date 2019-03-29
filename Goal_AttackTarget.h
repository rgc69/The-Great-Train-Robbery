#ifndef GOAL_ATTACKTARGET_H
#define GOAL_ATTACKTARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_AttackTarget.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "Veicolo.h"





class Goal_AttackTarget : public Goal_Composite<Veicolo>
{
public:

  Goal_AttackTarget(Veicolo* pOwner):Goal_Composite<Veicolo>(pOwner, goal_attack_target)
  {}

  void Activate();

  int  Process();

  void Terminate(){m_iStatus = completed;}

};






#endif
