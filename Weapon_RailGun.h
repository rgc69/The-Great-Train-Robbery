#ifndef RAILGUN_H
#define RAILGUN_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Weapon_RailGun.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to implement a rail gun
//-----------------------------------------------------------------------------
#include "Armi.h"


class  Veicolo;



class RailGun : public Raven_Weapon
{
private:

  void  InitializeFuzzyModule();

public:

  RailGun(Veicolo* owner);

  void  Render();

  void  Spara(Vector2D pos);

  double GetDesirability(double DistToTarget);
};



#endif