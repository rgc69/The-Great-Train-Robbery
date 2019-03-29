#ifndef GOAL_FOLLOWPATH_H
#define GOAL_FOLLOWPATH_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_FollowPath.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:
//-----------------------------------------------------------------------------
#include "Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "Veicolo.h"
#include "Raven_PathPlanner.h"
#include "PathEdge.h"



class Goal_FollowPath : public Goal_Composite<Veicolo>
{
private:

  //a local copy of the path returned by the path planner
  std::list<PathEdge>  m_Path;

public:

  Goal_FollowPath(Veicolo* pVeicolo, std::list<PathEdge> path);

  //the usual suspects
  void Activate();
  int Process();
  void Render();
  void Terminate(){}
};

#endif

