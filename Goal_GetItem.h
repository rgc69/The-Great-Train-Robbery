#ifndef GOAL_GET_ITEM_H
#define GOAL_GET_ITEM_H
#pragma warning (disable:4786)

#include "Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "Veicolo.h"
#include "trigger.h"


//helper function to change an item type enumeration into a goal type
int ItemTypeToGoalType(int gt);


class Goal_GetItem : public Goal_Composite<Veicolo>
{
private:

  int                     m_iItemToGet;

  Trigger<Veicolo>*     m_pGiverTrigger;

  //true if a path to the item has been formulated
  bool                    m_bFollowingPath;

  //returns true if the bot sees that the item it is heading for has been
  //picked up by an opponent
  bool hasItemBeenStolen()const;

public:

  Goal_GetItem(Veicolo* pBot,
               int        item):Goal_Composite<Veicolo>(pBot,
                                                   ItemTypeToGoalType(item)),
                                m_iItemToGet(item),
                                m_pGiverTrigger(0),
                                m_bFollowingPath(false)
  {}


  void Activate();

  int  Process();

  bool HandleMessage(const Telegram& msg);

  void Terminate(){m_iStatus = completed;}
};






#endif