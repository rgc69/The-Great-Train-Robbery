#ifndef GOAL_WANDER_H
#define GOAL_WANDER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_Wander.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Causes a bot to wander until terminated
//-----------------------------------------------------------------------------
#include "Goal.h"
#include "Raven_Goal_Types.h"
#include "Veicolo.h"


class Goal_Wander : public Goal<Veicolo>
{
private:

public:

  Goal_Wander(Veicolo* pBot):Goal<Veicolo>(pBot,goal_wander)
  {}

  void Activate();

  int  Process();

  void Terminate();
};





#endif
