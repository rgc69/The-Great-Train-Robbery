#ifndef PATH_H
#define PATH_H

#include <list>
#include <cassert>

#include "2d/Vector2D.h"




class PathNormale
{
private:
  
	//Una list in cui memorizzare la posizione di tutti i waypoints
  std::list<Vector2D>            m_WayPoints;

  //points to the current waypoint
  std::list<Vector2D>::iterator  curWaypoint;

  //flag to indicate if the path should be looped
  //(The last waypoint connected to the first)
  bool                           m_bLooped;

public:
  
  PathNormale():m_bLooped(false), curWaypoint(NULL){}

  //constructor for creating a path with initial random waypoints. MinX/Y
  //& MaxX/Y define the bounding box of the path.
  PathNormale(/*int    NumWaypoints,
       double MinX,
       double MinY,
       double MaxX,
       double MaxY,*/int x,
	   int y,
       bool   looped):m_bLooped(looped)
  {
    CreatePathNormale(/*NumWaypoints, MinX, MinY, MaxX, MaxY*/x,y);

    curWaypoint = m_WayPoints.begin();
  }


  //returns the current waypoint
  Vector2D    CurrentWaypoint()const{assert(curWaypoint != NULL); return *curWaypoint;}

  //returns true if the end of the list has been reached
  bool        Finished(){return !(curWaypoint != m_WayPoints.end());}
  
  //moves the iterator on to the next waypoint in the list
  inline void SetNextWaypoint();

  //creates a random path which is bound by rectangle described by
  //the min/max values
  std::list<Vector2D> CreatePathNormale(/*int    NumWaypoints,
                                       double MinX,
                                       double MinY,
                                       double MaxX,
                                       double MaxY*/
									   int x,
									   int y);


  void LoopOn(){m_bLooped = true;}
  void LoopOff(){m_bLooped = false;}
 
  //adds a waypoint to the end of the path
  void AddWayPoint(Vector2D new_point);

  //methods for setting the path with either another Path or a list of vectors
  void Set(std::list<Vector2D> new_path){m_WayPoints = new_path;curWaypoint = m_WayPoints.begin();}
  
  //Set istanzia un nuovo path, di cui GetPath() ritorna i Waypoints
  void Set(const PathNormale& path){m_WayPoints=path.GetPathNormale(); curWaypoint = m_WayPoints.begin();}
  

  void Clear(){m_WayPoints.clear();}

  std::list<Vector2D> GetPathNormale()const{return m_WayPoints;}

  //renders the path in orange
  void Render()const; 
};




//-------------------- Methods -------------------------------------------

inline void PathNormale::SetNextWaypoint()
{
  assert (m_WayPoints.size() > 0);
    
  if (++curWaypoint == m_WayPoints.end())
  {
    if (m_bLooped)
    {
	      curWaypoint = m_WayPoints.begin(); 
    }

  }
}  



#endif