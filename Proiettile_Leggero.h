#ifndef ProiettileLeggero_H
#define ProiettileLeggero_H
#pragma warning (disable:4786)

#include "Proiettili.h"

class Veicolo;



class ProiettileLeggero : public Proiettile
{
private:

  //tests the trajectory of the shell for an impact
  void TestForImpact();
  
public:

  ProiettileLeggero(Veicolo* shooter, Vector2D target);
  
  void Render();

  void Update();
// --------------------------------------------------------------------------------------
};


#endif

  