#include "SpotIncidente.h"
#include "misc/cgdi.h"
#include "2D/Transformations.h"
#include "misc/Stream_Utility_Functions.h"
//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
SpotIncidente::SpotIncidente(double lifetime):m_dLifeTime(lifetime)
{


      //create the vertex buffer for the graves
    const int NumripVerts = 4;
    const Vector2D rip[NumripVerts] = {Vector2D(-10.0f,6.0f),
                                        Vector2D(10.0f,0.0f),
                                        Vector2D(-10.0f,-6.0f),
                                        Vector2D(10.0f,-6.0f)};
  for (int i=0; i<NumripVerts; ++i)
  {
    m_vecRIPVB.push_back(rip[i]);
  }
}


void SpotIncidente::Update()
{

  GraveList::iterator it = m_GraveList.begin();
  while (it != m_GraveList.end())
  {

	 
   if (Clock->GetCurrentTime() - it->TimeCreated > m_dLifeTime)
	
    {
      it = m_GraveList.erase(it);
    }
    else
    {
      ++it;
    }
  }
}
    

void SpotIncidente::Render()
{
  GraveList::iterator it = m_GraveList.begin();
  Vector2D facing(-1,0);
  for (it; it != m_GraveList.end(); ++it)
  {
    
  /*  m_vecRIPVBTrans = WorldTransform(m_vecRIPVB,
                                   it->Position,
                                   facing,
                                   facing.Perp(),
                                   Vector2D(1,1));

    gdi->BluePen();
    gdi->ClosedShape(m_vecRIPVBTrans);*/
    gdi->TextColor(133,90,0);
    gdi->TextAtPos(it->Position.x - 10, it->Position.y - 20, "CRASH!");
    //gdi->TextAtPos(380, 250, ttos(Clock->GetCurrentTime() - it->TimeCreated));
  }
}

void SpotIncidente::AddGrave(Vector2D pos)
{
  m_GraveList.push_back(GraveRecord(pos));
}