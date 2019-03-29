#ifndef ArmaLeggera_H
#define ArmaLeggera_H
#pragma warning (disable:4786)

#include "Armi.h"


class  Veicolo;



class ArmaLeggera : public Raven_Weapon
{
private:

  void  InitializeFuzzyModule();
  
public:

  ArmaLeggera(Veicolo*   owner);


  void  Render();

  void  Spara(Vector2D pos);

  double GetDesirability(double DistToTarget);
};



#endif