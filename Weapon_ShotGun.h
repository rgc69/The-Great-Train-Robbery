#ifndef SHOTGUN_H
#define SHOTGUN_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Weapon_ShotGun.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to implement a shot gun
//-----------------------------------------------------------------------------
#include "Armi.h"


class  Veicolo;



class ShotGun : public Raven_Weapon
{
private:

  void     InitializeFuzzyModule();

  //how much shot the each shell contains
  int      m_iNumBallsInShell;

  //how much the shot spreads out when a cartridge is discharged
  double    m_dSpread;

public:

  ShotGun(Veicolo* owner);

  void  Render();

  void  Spara(Vector2D pos);

  double GetDesirability(double DistToTarget);
};



#endif