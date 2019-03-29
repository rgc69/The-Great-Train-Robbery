#include "Path.h"
#include "misc/utils.h"
#include "misc/Cgdi.h"
#include "2d/transformations.h"
// Per il ttos
#include "misc/Stream_Utility_Functions.h"

//#define DEBUG /* Turn debugging on */






std::list<Vector2D> PathNormale::CreatePathNormale( int x,
										   int y)


{
     m_WayPoints.clear();

   
    for (int i=0; i<640;i = i + 30)
    {
      

      Vector2D temp(x, y);

      temp.x = x; temp.y -= i;

      
      m_WayPoints.push_back(temp);
                            
    }

    curWaypoint = m_WayPoints.begin();

    return m_WayPoints;
}                



void PathNormale::Render()const
{
  gdi->OrangePen();

  std::list<Vector2D>::const_iterator it = m_WayPoints.begin();

  Vector2D wp = *it++;

  while (it != m_WayPoints.end())
  {
    gdi->Line(wp, *it);

    wp = *it++;
  }

  if (m_bLooped) gdi->Line(*(--it), *m_WayPoints.begin());

#ifdef DEBUG
//Verifichiamo le coordinate dei waypoints
  it = m_WayPoints.begin();
 
   while (it != m_WayPoints.end())
 {
	   	
/*gdi->TextAtPos(100, 150, "Waypoint begin:"); gdi->TextAtPos(210,150,ttos(*m_WayPoints.begin()));
gdi->TextAtPos(100, 200, "Waypoint begin + 1:"); gdi->TextAtPos(230,200,ttos(*m_WayPoints.begin()));
gdi->TextAtPos(100, 250, "Waypoint size:"); gdi->TextAtPos(200,250,ttos(m_WayPoints.size()));*/
for (int n = 0; n < 110; n = n+11) {
      wp = *it++;
	  gdi->TextAtPos(100, 150 + n , "Waypoints:"); gdi->TextAtPos(210 ,150 + n ,ttos(wp));
 }
	    }
	  
	  //Inseriamo un cerchio come test alla posizione x=100, y=100
gdi->TextAtPos(100, 110, " Test: 100,110"); 
gdi->RedBrush();
gdi->Circle(100,110, 4.0 );


//Disegniamo il waypoint. Le sue coordinate dovrebbero coincidere con quelle a schermo
 /* gdi->BrownBrush();
  gdi->Circle(*m_WayPoints.begin(), 4.0 );
  gdi->TextAtPos(*m_WayPoints.begin(), "  1");

  gdi->BrownBrush();
  gdi->Circle(*(--it), 4.0 );*/ 
 for (int n = 0; n < 10; n++) {
      wp = *it++;
	  gdi->BrownBrush();
      gdi->Circle(wp, 4.0 );
 }

#endif /* DEBUG */
 


}
