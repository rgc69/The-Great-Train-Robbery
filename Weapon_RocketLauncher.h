#ifndef ROCKETLAUNCHER_H
#define ROCKETLAUNCHER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   RocketLauncher
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to implement a rocket launcher
//-----------------------------------------------------------------------------
#include "Armi.h"



class  Veicolo;

class RocketLauncher : public Raven_Weapon
{
private:

  void     InitializeFuzzyModule();

public:

  RocketLauncher(Veicolo* owner);


  void Render();

  void Spara(Vector2D pos);

  double GetDesirability(double DistToTarget);
};



#endif