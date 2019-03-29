#ifndef BLASTER_H
#define BLASTER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Weapon_Blaster.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   
//-----------------------------------------------------------------------------
#include "Armi.h"


class  Veicolo;



class Blaster : public Raven_Weapon
{
private:

  void  InitializeFuzzyModule();
  
public:

  Blaster(Veicolo*   owner);


  void  Render();

  void  Spara(Vector2D pos);

  double GetDesirability(double DistToTarget);
};



#endif