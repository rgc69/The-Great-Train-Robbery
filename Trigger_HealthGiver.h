#ifndef HEALTH_GIVER_H
#define HEALTH_GIVER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//
//  Desc:     If a bot runs over an instance of this class its health is
//            increased. 
//
//-----------------------------------------------------------------------------
#include "Trigger_Respawning.h"
#include "TriggerRegion.h"
#include <iosfwd>
#include "Veicolo.h"



class Trigger_HealthGiver : public Trigger_Respawning<Veicolo>
{
private:

  //the amount of health an entity receives when it runs over this trigger
  int   m_iHealthGiven;
  
public:

  Trigger_HealthGiver(std::ifstream& datafile);

  //if triggered, the bot's health will be incremented
  void Try(Veicolo* pBot);
  
  //draws a box with a red cross at the trigger's location
  void Render();

  void Read (std::ifstream& is);
};



#endif