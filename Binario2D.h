#ifndef BINARIO_H
#define BINARIO_H

#include "misc/Cgdi.h"
#include "2d/Vector2D.h"
#include <fstream>


class Binario2D 
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

  Binario2D(){}

  Binario2D(Vector2D A, Vector2D B):m_vA(A), m_vB(B)
  {
    CalculateNormal();
  }

  Binario2D(Vector2D A, Vector2D B, Vector2D N):m_vA(A), m_vB(B), m_vN(N)
  { }

  Binario2D(std::ifstream& in){Read(in);}

  virtual void Render(bool RenderNormals = false)const
  {
    gdi->Line(m_vA, m_vB);

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

    //Per ritornare in un Vettore 2 D la pozione x di uno dei quattro valori
  Vector2D PosizioneBin()const{return m_vA;}

      //Per ritornare in un Vettore 2 D la pozione x di uno dei quattro valori
  Vector2D PosizioneBin2()const{return m_vB;}

  std::ostream& Binario2D::Write(std::ostream& os)const
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

    in >> x >> y;
    SetFrom(Vector2D(x,y));

    in >> x >> y;
    SetTo(Vector2D(x,y));

     in >> x >> y;
    SetNormal(Vector2D(x,y));
  }
  
};

#endif