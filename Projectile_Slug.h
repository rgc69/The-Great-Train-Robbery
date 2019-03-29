#ifndef SLUG_H
#define SLUG_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Slug.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to implement a railgun slug
//-----------------------------------------------------------------------------

#include "Proiettili.h"

class Veicolo;
class Raven_Environment;

class Slug : public Proiettile
{
private:

  //when this projectile hits something it's trajectory is rendered
  //for this amount of time
  double   m_dTimeShotIsVisible;

  //tests the trajectory of the shell for an impact
  void  TestForImpact();

    //returns true if the shot is still to be rendered
  bool  isVisibleToPlayer()const{return Clock->GetCurrentTime() < m_dTimeOfCreation + m_dTimeShotIsVisible;}
  
public:

  Slug(Veicolo* shooter, Vector2D target);
  
  void Render();

  void Update();
  
};


#endif