#include "Proiettili.h"
#include "MondoDiGioco.h"
#include <list>

//------------------ GetClosestIntersectingVeicolo --------------------------------

Veicolo* Proiettile::GetClosestIntersectingVeicolo(Vector2D    From,
                                                       Vector2D    To)const
{
  Veicolo* ClosestIntersectingVeicolo = 0;
  double ClosestSoFar = MaxDouble;

  //iterate through all entities checking against the line segment FromTo
  /*std::list<Veicolo*>::const_iterator curVeicolo;
  for (curVeicolo =  m_pWorld->GetAllVeicoli().begin();
       curVeicolo != m_pWorld->GetAllVeicoli().end();
       ++curVeicolo)
  {*/
  std::vector<Veicolo*>::const_iterator curVeicolo = m_pWorld->GetAllVeicoli().begin();
  for (curVeicolo; curVeicolo != m_pWorld->GetAllVeicoli().end(); ++curVeicolo)
  {
    //make sure we don't check against the shooter of the projectile
    if ( ((*curVeicolo)->ID() != m_iShooterID))
    {
      //if the distance to FromTo is less than the entity's bounding radius then
      //there is an intersection
      if (DistToLineSegment(From, To, (*curVeicolo)->Pos()) < (*curVeicolo)->BRadius())
      {
        //test to see if this is the closest so far
        double Dist = Vec2DDistanceSq((*curVeicolo)->Pos(), m_vOrigin);

        if (Dist < ClosestSoFar)
        {
          Dist = ClosestSoFar;
          ClosestIntersectingVeicolo = *curVeicolo;
        }
      }
    }

  }

  return ClosestIntersectingVeicolo;
}


//---------------------- GetListOfIntersectingVeicolos ----------------------------
std::vector<Veicolo*> Proiettile::GetListOfIntersectingVeicoli(Vector2D From,
                                                                  Vector2D To)const
{
  //this will hold any Veicolo that are intersecting with the line segment
  std::vector<Veicolo*> hits;

  //iterate through all entities checking against the line segment FromTo
  std::vector<Veicolo*>::const_iterator curVeicolo = m_pWorld->GetAllVeicoli().begin();
  for (curVeicolo; curVeicolo != m_pWorld->GetAllVeicoli().end(); ++curVeicolo)
  {
    //make sure we don't check against the shooter of the projectile
    if ( ((*curVeicolo)->ID() != m_iShooterID))
    {
      //if the distance to FromTo is less than the entities bounding radius then
      //there is an intersection so add it to hits
      if (DistToLineSegment(From, To, (*curVeicolo)->Pos()) < (*curVeicolo)->BRadius())
      {
        hits.push_back(*curVeicolo);
      }
    }

  }

  return hits;
}

  
//------------------------ isAtPosition ---------------------------------------
//
//  returns true if the vehicle is close to the given position
//-----------------------------------------------------------------------------
bool Proiettile::isAtPosition(Vector2D pos)const
{
  const static double tolerance = 10.0;
  
  return Vec2DDistanceSq(Pos(), pos) < tolerance * tolerance;
}