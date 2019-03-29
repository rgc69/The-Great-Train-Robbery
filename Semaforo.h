#ifndef SEMAFORO_H
#define SEMAFORO_H
#pragma warning (disable:4786)

#include "misc/Cgdi.h"
#include "2d/Vector2D.h"
#include <fstream>
#include "EnumerazioneOggetti.h"
#include "misc/Stream_Utility_Functions.h"

//--------------------------- Constants ----------------------------------

class Semaforo
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

		 enum color_type
  {
    none               = 0x00000,
    red                = 0x00002,
    green              = 0x00004,
	yellow             = 0x00006,
    
  };

    //Per il Flag dei semafafori dx o sx
  // std::vector<int>                   m_Flag;
  std::vector<Vector2D>              m_Flag;

//binary flags to indicate whether or not a semaphore color should be active
  int           m_iFlags;

//this function tests if a specific bit of m_iFlags is set
  bool      On(color_type bt){return (m_iFlags & bt) == bt;}
    
  //disallow the copying of Semaphore types
  Semaforo(const Semaforo&);
  Semaforo& operator=(const Semaforo&);

//fills the buffer with vertex data
  void InitializeBuffer();


Semaforo(){}

  Semaforo(Vector2D A, Vector2D B):m_vA(A), m_vB(B)
  {
    CalculateNormal();
  }

  Semaforo(Vector2D A, Vector2D B, Vector2D N):m_vA(A), m_vB(B), m_vN(N)
  { }

  Semaforo(std::ifstream& in){Read(in);}

///////////RENDERING Semaforo ROSSO con scambio verso DESTRA//////////////////////////
 Vector2D RenderRedDx(bool Intersezione = false)const
  {
   
	 gdi->RedBrush();
     gdi->BlackPen();
     gdi->Circle(m_vA.x,m_vA.y,3);
     gdi->GreenBrush();
     gdi->BlackPen();
     gdi->Circle(m_vB.x,m_vB.y,3);

	 double m;
     Vector2D m_vO;
     Vector2D m_vS;
gdi->BrownPen();
//Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
	 //Il binario
    gdi->Line(m_vA.x + 38.1, m_vA.y,m_vA.x,(436 - result));
	m_vO.x = (((m_vA.x + 38.1) + (m_vA.x)))/2;
	m_vO.y = ((m_vA.y) + (436 - result))/2;
    m_vS.x = m_vO.x;
    m_vS.y = (436 - result);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
	  //Il binario
     gdi->Line(m_vA.x + 40, m_vA.y,m_vA.x,(455 - result));
	 m_vO.x = (((m_vA.x + 40) + (m_vA.x)))/2;
	 m_vO.y = ((m_vA.y) + (455 - result))/2;
	 m_vS.x = (m_vO.x + 40);
	 m_vS.y = m_vO.y;
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	//Il binario
    gdi->Line(m_vB.x + 15, m_vB.y - 25,m_vB.x + 15,m_vB.y + 25);
	 m_vO.x = (((m_vB.x + 15) + (m_vB.x + 15)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y + 25))/2;
     m_vS.x = (m_vO.x + 40);
	 m_vS.y = (m_vB.y - 23);
	 	//Lo scambio
	gdi->Line(m_vO, m_vS);
	    } 

    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
	//Il binario
    gdi->Line(m_vB.x - 35, m_vB.y - 15,m_vB.x + 35,m_vB.y - 15);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 15) + (m_vB.y - 15))/2;
     m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y - 45);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
		//Il binario
    gdi->Line(m_vB.x - 25, m_vA.y + 25,m_vB.x + 25,m_vA.y + 25);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 25) + (m_vA.y + 25))/2;
	 m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y + 45);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    }

    //render the normals if rqd
    if (Intersezione)
    {
      return m_vO;
    }
	return m_vS;
  }

////////////RENDERING Semaforo VERDE con scambio verso DESTRA////////////////////////
    Vector2D RenderGreenDx(bool Intersezione = false)const
  {
   
    gdi->GreenBrush();
    gdi->BlackPen();
    gdi->Circle(m_vA.x,m_vA.y,3);
    gdi->RedBrush();
    gdi->BlackPen();
    gdi->Circle(m_vB.x,m_vB.y,3);

		 double m;
     Vector2D m_vO,
		      m_vS,
		      m_vBin;
gdi->BrownPen();
	 //Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
	 m_vBin.x = m_vA.x + 38.1;
	 m_vBin.y = m_vA.y;
	 //Il binario
    gdi->Line(m_vBin.x,m_vBin.y ,m_vA.x,(436 - result));
	m_vO.x = (((m_vA.x + 38.1) + (m_vA.x)))/2;
	m_vO.y = ((m_vA.y) + (436 - result))/2;
    m_vS.x = m_vO.x;
    m_vS.y = (436 - result);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
	  //Il binario
     gdi->Line(m_vA.x + 40, m_vA.y,m_vA.x,(455 - result));
	 m_vO.x = (((m_vA.x + 40) + (m_vA.x)))/2;
	 m_vO.y = ((m_vA.y) + (455 - result))/2;
	 m_vS.x = (m_vO.x + 40);
	 m_vS.y = m_vO.y;
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    }
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vB.x + 15;
	 m_vBin.y = m_vB.y;
	 //Il binario
    gdi->Line(m_vBin.x , m_vBin.y - 25,m_vB.x + 15,m_vB.y + 25);
	 m_vO.x = (((m_vB.x + 15) + (m_vB.x + 15)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y + 25))/2;
     m_vS.x = (m_vO.x + 40);
	 m_vS.y = (m_vB.y - 23);
	 	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 

    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
	 m_vBin.x = m_vA.x - 25;
	 m_vBin.y = m_vB.y - 15;
	//Il binario
    gdi->Line(m_vB.x - 35, m_vB.y - 15,m_vB.x + 35,m_vB.y - 15);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 15) + (m_vB.y - 15))/2;
     m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y - 45);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	 m_vBin.x = m_vB.x - 25;
	 m_vBin.y = m_vA.y + 25;
		//Il binario
    gdi->Line(m_vBin.x, m_vBin.y,m_vB.x + 25,m_vA.y + 25);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 25) + (m_vA.y + 25))/2;
	 m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y + 45);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
	 }

    //ritorna il punto medio del binario
    if (Intersezione)
    {
     return m_vO;
    }
	return m_vBin;
  }

///////////RENDERING Semaforo ROSSO con scambio verso SINISTRA//////////////////////////
 Vector2D RenderRedSx(bool Intersezione = false)const
  {
   
	 gdi->RedBrush();
     gdi->BlackPen();
     gdi->Circle(m_vA.x,m_vA.y,3);
     gdi->GreenBrush();
     gdi->BlackPen();
     gdi->Circle(m_vB.x,m_vB.y,3);

	 double m;
     Vector2D m_vO,
              m_vS,
			  m_vBin;

gdi->BrownPen();
	 //Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
	 m_vBin.x = m_vA.x - 38.1;
	 m_vBin.y = m_vB.y;
	 //Il binario
    gdi->Line(m_vBin.x,m_vBin.y ,m_vB.x,(m_vBin.y + 49));
	m_vO.x = (m_vBin.x + m_vB.x)/2;
	m_vO.y = ((m_vBin.y) + (m_vBin.y + 49))/2;
    m_vS.x = m_vO.x - 40;
    m_vS.y = (m_vO.y);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
 	 m_vBin.x = m_vB.x - 50;
	 m_vBin.y = m_vB.y;
	  //Il binario
     gdi->Line(m_vBin.x, m_vBin.y,m_vB.x,(m_vB.y - 50));
	 m_vO.x = (((m_vBin.x) + (m_vB.x)))/2;
	 m_vO.y = ((m_vBin.y) + (m_vBin.y - 50))/2;
	 m_vS.x = (m_vO.x);
	 m_vS.y = m_vB.y - 50;
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vA.x - 15;
	 m_vBin.y = m_vA.y - 25;
	//Il binario
    gdi->Line( m_vBin.x,  m_vBin.y,m_vA.x - 15,m_vA.y + 25);
	 m_vO.x = (((m_vA.x - 15) + (m_vA.x - 15)))/2;
	 m_vO.y = ((m_vA.y - 25) + (m_vA.y + 25))/2;
     m_vS.x = (m_vO.x - 40);
	 m_vS.y = (m_vA.y - 23);
	 	//Lo scambio
	gdi->Line(m_vO, m_vS);
	    } 

    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	//Il binario
    gdi->Line(m_vB.x - 25, m_vB.y - 25,m_vB.x + 25,m_vB.y - 25);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y - 25))/2;
     m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y - 55);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
		//Il binario
    gdi->Line(m_vB.x - 25, m_vA.y + 20,m_vB.x + 25,m_vA.y + 20);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 20) + (m_vA.y + 20))/2;
	 m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y + 50);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    }

    //ritorna il punto medio del binario
    if (Intersezione)
    {
     return m_vO;
    }
	return m_vBin;
  }

////////////RENDERING Semaforo VERDE con scambio verso SINISTRA////////////////////////
    Vector2D RenderGreenSx(bool Intersezione = false)const
  {
   
    gdi->GreenBrush();
    gdi->BlackPen();
    gdi->Circle(m_vA.x,m_vA.y,3);
    gdi->RedBrush();
    gdi->BlackPen();
    gdi->Circle(m_vB.x,m_vB.y,3);

		 double m;
     Vector2D m_vO,
		      m_vS,
		      m_vBin;

gdi->BrownPen();
	 //Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    { // gdi->BrownPen();
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
	 m_vBin.x = m_vA.x - 38.1;
	 m_vBin.y = m_vB.y;
	 //Il binario
    gdi->Line(m_vBin.x,m_vBin.y ,m_vB.x,(m_vBin.y + 49));
	m_vO.x = (m_vBin.x + m_vB.x)/2;
	m_vO.y = ((m_vBin.y) + (m_vBin.y + 49))/2;
    m_vS.x = m_vO.x - 40;
    m_vS.y = (m_vO.y);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
 	 m_vBin.x = m_vB.x - 50;
	 m_vBin.y = m_vB.y;
	  //Il binario
     gdi->Line(m_vBin.x, m_vBin.y,m_vB.x,(m_vB.y - 50));
	 m_vO.x = (((m_vBin.x) + (m_vB.x)))/2;
	 m_vO.y = ((m_vBin.y) + (m_vBin.y - 50))/2;
	 m_vS.x = (m_vO.x);
	 m_vS.y = m_vB.y - 50;
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    } 
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vA.x - 15;
	 m_vBin.y = m_vA.y - 25;
	//Il binario
    gdi->Line( m_vBin.x,  m_vBin.y,m_vA.x - 15,m_vA.y + 25);
	 m_vO.x = (((m_vA.x - 15) + (m_vA.x - 15)))/2;
	 m_vO.y = ((m_vA.y - 25) + (m_vA.y + 25))/2;
     m_vS.x = (m_vO.x - 40);
	 m_vS.y = (m_vA.y - 23);
	 	//Lo scambio
	gdi->Line(m_vO, m_vS);
	    } 



    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	//Il binario
    gdi->Line(m_vB.x - 25, m_vB.y - 25,m_vB.x + 25,m_vB.y - 25);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y - 25))/2;
     m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y - 55);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    }  

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
		//Il binario
    gdi->Line(m_vB.x - 25, m_vA.y + 20,m_vB.x + 25,m_vA.y + 20);
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 20) + (m_vA.y + 20))/2;
	 m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y + 50);
	//Lo scambio
	gdi->Line(m_vO, m_vS);
    }

    //ritorna il punto medio del binario
    if (Intersezione)
    {
     return m_vO;
    }
	return m_vS;
  }

///////////COORDINATE scambio verso DESTRA//////////////////////////
 Vector2D Dx(bool Intersezione = false)const
  {   
	 double m;
     Vector2D m_vO,
              m_vS,
			  m_vBin;

//Se il semaforo è inclinato verso sinistra

if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
    m_vBin.x = m_vA.x + 38.1;
	m_vBin.y = m_vA.y;
	m_vO.x = (((m_vA.x + 38.1) + (m_vA.x)))/2;
	m_vO.y = ((m_vA.y) + (436 - result))/2;
    m_vS.x = m_vO.x;
    m_vS.y = (436 - result);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
	 m_vBin.x = m_vA.x;
	 m_vBin.y = 455 - result;
  	 m_vO.x = (((m_vA.x + 40) + (m_vA.x)))/2;
	 m_vO.y = ((m_vA.y) + (455 - result))/2;
	 m_vS.x = (m_vO.x + 40);
	 m_vS.y = m_vO.y;
    } 
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vB.x +15;
	 m_vBin.y = m_vB.y + 25;
	 m_vO.x = (((m_vB.x + 15) + (m_vB.x + 15)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y + 25))/2;
     m_vS.x = (m_vO.x + 40);
	 m_vS.y = (m_vB.y - 23);
	    } 
 
    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
	 m_vBin.x = m_vB.x + 35;
	 m_vBin.y = m_vB.y -15;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 15) + (m_vB.y - 15))/2;
     m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y - 45);
    }  

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	 m_vBin.x = m_vB.x - 25;
	 m_vBin.y = m_vA.y + 25;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 25) + (m_vA.y + 25))/2;
	 m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y + 45);
    }

    //ritorna il punto medio del binario
    if (Intersezione)
    {
      return m_vO;
    }
	return m_vBin;
  }

///////////COORDINATE Scambio con scambio verso DESTRA//////////////////////////
 Vector2D ScambioDx(bool Intersezione = false)const
  {   
	 double m;
     Vector2D m_vO,
              m_vS,
			  m_vBin;
//Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
    m_vBin.x = m_vA.x ;
	m_vBin.y = 436 - result;
	m_vO.x = (((m_vA.x + 38.1) + (m_vA.x)))/2;
	m_vO.y = ((m_vA.y) + (436 - result))/2;
    m_vS.x = m_vO.x;
    m_vS.y = (436 - result);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
	 m_vBin.x = m_vA.x + 40;
	 m_vBin.y = m_vA.y;
  	 m_vO.x = (((m_vA.x + 40) + (m_vA.x)))/2;
	 m_vO.y = ((m_vA.y) + (455 - result))/2;
	 m_vS.x = (m_vO.x + 40);
	 m_vS.y = m_vO.y;
    } 
    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vB.x +15;
	 m_vBin.y = m_vB.y - 25;
	 m_vO.x = (((m_vB.x + 15) + (m_vB.x + 15)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y + 25))/2;
     m_vS.x = (m_vO.x + 40);
	 m_vS.y = (m_vB.y - 23);
	    } 

    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
	 m_vBin.x = m_vB.x - 35;
	 m_vBin.y = m_vB.y -15;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 15) + (m_vB.y - 15))/2;
     m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y - 45);
    } 

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	 m_vBin.x = m_vB.x + 25;
	 m_vBin.y = m_vA.y + 25;
	 m_vO.x = (((m_vB.x + 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 25) + (m_vA.y + 25))/2;
	 m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y + 45);
    }
   //ritorna il  binario
    if (Intersezione)
    {
      return m_vBin;
    }
   	return m_vS;
  }
 
	///////////COORDINATE scambio verso SINISTRA//////////////////////////
 Vector2D Sx(bool Intersezione = false)const
  {
   	 double m;
     Vector2D m_vO,
              m_vS,
			  m_vBin,
			  m_vBin1;
	 //Se il semaforo è inclinato verso sinistra
	// if (m_vBin1.x > m_vO.x){
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;

	 m_vBin1.x = m_vB.x;
	 m_vBin1.y = m_vB.y + 49;

	 m_vBin.x = m_vA.x - 38.1;
	 m_vBin.y = m_vB.y;

     m_vO.x = (m_vBin.x + m_vB.x)/2;
	 m_vO.y = ((m_vBin.y) + (m_vBin.y + 49))/2;
     m_vS.x = m_vO.x - 40;
     m_vS.y = (m_vO.y);
    //} 
} 
     //Se il semaforo è inclinato verso destra

if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;

     m_vBin1.x = m_vB.x - 50;
	 m_vBin1.y = m_vB.y ;

 	 m_vBin.x = m_vB.x - 50;
	 m_vBin.y = m_vB.y;

	 m_vO.x = (((m_vBin.x) + (m_vB.x)))/2;
	 m_vO.y = ((m_vBin.y) + (m_vBin.y - 50))/2;
	 m_vS.x = (m_vO.x);
	 m_vS.y = m_vB.y - 50;
}
   
    //Se il semaforo è orizzontale
//if (m_vBin1.x == m_vO.x && m_vO.x < m_vS.x){
if (m_vB.y == m_vA.y)
    {

     m_vBin1.x = m_vA.x - 15;
	 m_vBin1.y = m_vA.y + 25;

	 m_vBin.x = m_vA.x - 15;
	 m_vBin.y = m_vA.y - 25;

	 m_vO.x = (((m_vA.x - 15) + (m_vA.x - 15)))/2;
	 m_vO.y = ((m_vA.y - 25) + (m_vA.y + 25))/2;
     m_vS.x = (m_vO.x - 40);
	 m_vS.y = (m_vA.y - 23);
 } 

	    //} 

    //Se il semaforo è verticale verso l'alto
//if (m_vBin1.x < m_vO.x){
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {

     m_vBin1.x = m_vB.x - 25;
	 m_vBin1.y = m_vB.y - 25;

	 m_vBin.x = m_vB.x - 25;
	 m_vBin.y = m_vB.y - 25;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 25) + (m_vB.y - 25))/2;
     m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y - 55);
}
   // } 

    //Se il semaforo è verticale verso il basso
//if (m_vBin1.x > m_vO.x){
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {

     m_vBin1.x = m_vB.x + 25;
	 m_vBin1.y = m_vA.y + 20;

     m_vBin.x = m_vB.x - 25;
	 m_vBin.y = m_vA.y + 25;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 20) + (m_vA.y + 20))/2;
	 m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y + 40);
   // }
}
    //ritorna il punto medio del binario
    if (Intersezione)
    {
     return m_vO;
    }
	return m_vBin1;
  }

	///////////COORDINATE Scambio con scambio verso SINISTRA//////////////////////////
 Vector2D ScambioSx(bool Intersezione = false)const
  {
   	 double m;
     Vector2D m_vO,
              m_vS,
			  m_vBin;
	 //Se il semaforo è inclinato verso sinistra
if (m_vB.y < m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result ;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) + 10;
	 m_vBin.x = m_vA.x - 38.1;
	 m_vBin.y = m_vB.y;

	m_vO.x = (m_vBin.x + m_vB.x)/2;
	m_vO.y = ((m_vBin.y) + (m_vBin.y + 49))/2;
    m_vS.x = m_vO.x - 40;
    m_vS.y = (m_vO.y);
    } 
     //Se il semaforo è inclinato verso destra
if (m_vB.y > m_vA.y && m_vA.x != m_vB.x)
    {
	 m = ((445 - m_vB.y) - (445 - m_vA.y))/(m_vB.x-m_vA.x);
     double result;
     result = -1/m * m_vA.x + (-1/m) * (-m_vB.x) + (445 - m_vB.y) - 10;
 	 
     m_vBin.x = m_vB.x;
	 m_vBin.y = m_vB.y - 50;

	 m_vO.x = (((m_vB.x - 50) + (m_vB.x)))/2;
	 m_vO.y = ((m_vB.y) + (m_vB.y - 50))/2;
	 m_vS.x = m_vO.x;
	 m_vS.y = m_vB.y - 50;

    } 

    //Se il semaforo è orizzontale
if (m_vB.y == m_vA.y)
    {
	 m_vBin.x = m_vA.x - 15;
	 m_vBin.y = m_vA.y - 25;

	 m_vO.x = (((m_vA.x - 15) + (m_vA.x - 15)))/2;
	 m_vO.y = ((m_vA.y - 25) + (m_vA.y + 25))/2;
     m_vS.x = (m_vO.x - 40);
	 m_vS.y = (m_vA.y - 23);

	    } 

    //Se il semaforo è verticale verso l'alto
if (m_vB.x == m_vA.x && m_vB.y > m_vA.y)
    {
	 m_vBin.x = m_vB.x + 25;
	 m_vBin.y = m_vB.y - 25;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vB.y - 15) + (m_vB.y - 15))/2;
     m_vS.x = (m_vB.x + 25);
	 m_vS.y = (m_vB.y - 55);

    } 

    //Se il semaforo è verticale verso il basso
if (m_vB.x == m_vA.x && m_vB.y < m_vA.y)
    {
     m_vBin.x = m_vB.x - 25;
	 m_vBin.y = m_vA.y + 20;
	 m_vO.x = (((m_vB.x - 25) + (m_vB.x + 25)))/2;
	 m_vO.y = ((m_vA.y + 25) + (m_vA.y + 25))/2;
	 m_vS.x = (m_vB.x - 25);
	 m_vS.y = (m_vB.y + 50);
    }
 //ritorna il  binario
    if (Intersezione)
    {
      return m_vBin;
    }
    	return m_vS;
  }


  Vector2D From()const  {return m_vA;}

  void     SetFrom(Vector2D v){m_vA = v; CalculateNormal();}

  Vector2D To()const    {return m_vB;}
  void     SetTo(Vector2D v){m_vB = v; CalculateNormal();}
  
  Vector2D Normal()const{return m_vN;}

  void     SetNormal(Vector2D n){m_vN = n;}
  
  Vector2D Center()const{return (m_vA+m_vB)/2.0;}

  //Per ritornare in un Vettore 2 D la posizione x di uno dei quattro valori
  Vector2D PosizioneSem()const{return m_vA;}
  void     AddmvA(){ m_Flag.push_back(m_vA);}
  const std::vector<Vector2D>&       GetmVa()const{return  m_Flag;}

  std::ostream& Semaforo::Write(std::ostream& os)const
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

 void TurnRedOn(){ m_iFlags |= red;} 
 void TurnRedOff(){if(On(red)) m_iFlags ^=red;}
 bool isRedOn(){return On(red);}


 void TurnGreenOn(){ m_iFlags |= green;}
 void TurnGreenOff(){if(On(green)) m_iFlags ^=green;}
 bool isGreenOn(){return On(green);}


const int* const OnOff1(int x) {
static int i;
i = x;
return &i;
}

const int* const OnOff2(int x) {
static int h;
h = x;
return &h;
}

  };




#endif