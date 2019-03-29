#ifndef GRAVE_MARKERS_H
#define GRAVE_MARKERS_H
#pragma warning (disable : 4786)

#include <list>
#include <vector>
#include "2d/vector2d.h"
#include "time/crudetimer.h"

class SpotIncidente
{
private:
  
  struct GraveRecord
  {
    Vector2D Position;
    double    TimeCreated;

    GraveRecord(Vector2D pos):Position(pos),
                              TimeCreated(Clock->GetCurrentTime())
    {}
  };

private:
  
  typedef std::list<GraveRecord> GraveList;

private:

  //how long a grave remains on screen
  double m_dLifeTime;

  //when a bot dies, a grave is rendered to mark the spot.
  std::vector<Vector2D>   m_vecRIPVB;
  std::vector<Vector2D>   m_vecRIPVBTrans;
  GraveList               m_GraveList;


public:

  SpotIncidente(double lifetime);

  void Update();
  void Render();
  void AddGrave(Vector2D pos);

};

#endif