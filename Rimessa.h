#ifndef RIMESSA_H
#define RIMESSA_H
//------------------------------------------------------------------------
//
//  Name:   Rimessa.h
//
//  Desc:   classe per renderizzare rettangoli. 
//
//------------------------------------------------------------------------
#include "misc/Cgdi.h"
#include "2d/Vector2D.h"
#include <fstream>


class Rimessa 
{
protected:

  Vector2D    m_vA,
              m_vB,
              m_vN;

  void CalculateNormal()
  {
    Vector2D temp = Vec2DNormalize(m_vB - m_vA);

    m_vN.x = -temp.y;
    m_vN.y = temp.x;
  }

public:

  Rimessa(){}

  Rimessa(Vector2D A, Vector2D B):m_vA(A), m_vB(B)
  {
    CalculateNormal();
  }

  Rimessa(Vector2D A, Vector2D B, Vector2D N):m_vA(A), m_vB(B), m_vN(N)
  { }

  Rimessa(std::ifstream& in){Read(in);}

  virtual void Render(bool RenderNormals = false)const
  {
      gdi->Rect(m_vA.x,m_vA.y, m_vB.x,m_vB.y);

	 /*/ gdi->RedBrush();
    gdi->BlackPen();
gdi->Circle(m_vA.x,m_vA.y,10);
gdi->GreenBrush();
    gdi->BlackPen();
gdi->Circle(m_vB.x,m_vB.y,10);*/

    //render the normals if rqd
    if (RenderNormals)
    {
      int MidX = (int)((m_vA.x+m_vB.x)/2);
      int MidY = (int)((m_vA.y+m_vB.y)/2);

      gdi->Line(MidX, MidY, (int)(MidX+(m_vN.x * 5)), (int)(MidY+(m_vN.y * 5)));
    }
  }

  Vector2D From()const  {return m_vA;}
  void     SetFrom(Vector2D v){m_vA = v; CalculateNormal();}

  Vector2D To()const    {return m_vB;}
  void     SetTo(Vector2D v){m_vB = v; CalculateNormal();}
  
  Vector2D Normal()const{return m_vN;}
  void     SetNormal(Vector2D n){m_vN = n;}
  
  Vector2D Center()const{return (m_vA+m_vB)/2.0;}

  std::ostream& Rimessa::Write(std::ostream& os)const
  {
    os << std::endl;
    os << From() << ",";
    os << To() << ",";
    os << Normal();
    return os;
  }

 void Read(std::ifstream& in)
  {
	 double x,y;
    in >> x >> y ;
    SetFrom(Vector2D(x,y));

    in >> x >> y ;
    SetTo(Vector2D(x,y));

     in >> x >> y ;
    SetNormal(Vector2D(x,y));
  }
  
};

#endif