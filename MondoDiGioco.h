#ifndef MondoDiGioco_H
#define MondoDiGioco_H
#pragma warning (disable:4786)

#include <windows.h>
#include <vector>

#include "2d/Vector2D.h"
#include "time/PrecisionTimer.h"
#include "misc/CellSpacePartition.h"
#include "EntitaBase.h"
#include "EntityFunctionTemplates.h"
#include "Veicolo.h"
#include "Semaforo.h"
#include "EnumerazioneOggetti.h"
#include "Wall2D.h"
#include "Binario2D.h"
#include "SparseGraph.h"
#include "pathmanager.h"
#include "Raven_PathPlanner.h"
class Obstacle;
class Wall2D;
class Binario2D;
class Semaforo;
class PathNormale;
class Proiettile;
class Raven_Map;
typedef std::vector<BaseGameEntity*>::iterator  ObIt;


class MondoDiGioco
{ 
private:

  //a container of all the moving entities
  std::vector<Veicolo*>         m_Vehicles;

//a container of all the semaphore
  std::vector<Semaforo>         m_Semafori;


  //a list of all the bots that are inhabiting the map
  std::list<Veicolo*>            m_Veicoli;

    //this list contains any active projectiles (slugs, rockets,
  //shotgun pellets, etc)
  std::list<Proiettile*>     m_Projectiles;

   std::vector<Proiettile*>     m_Test;

     //this class manages all the path planning requests
  PathManager<Raven_PathPlanner>*  m_pPathManager;

  //any obstacles
  std::vector<BaseGameEntity*>  m_Obstacles;

  //container containing any walls in the environment
  std::vector<Wall2D>           m_Walls;

  //container containing any walls in the environment
  std::vector<Binario2D>           m_Binari;

  CellSpacePartition<Veicolo*>* m_pCellSpace;

  //any path we may create for the vehicles to follow
 PathNormale*                         m_pPath;

   //the current game map
  Raven_Map*                       m_pMap;

   //deletes all entities, empties all containers and creates a new navgraph 
  void  Clear();

   //when a bot is killed a "grave" is displayed for a few seconds. This
  //class manages the graves
 SpotIncidente*                    m_pGraveMarkers;

  //attempts to position a spawning bot at a free spawn point. returns false
  //if unsuccessful 
  bool AttemptToAddVeicolo(Veicolo* pVeicolo); 
  
  //set true to pause the motion
  bool                          m_bPaused;
  //if true a bot is removed from the game
  bool                             m_bRemoveABot;
  //local copy of client window dimensions
  int                           m_cxClient,
                                m_cyClient;
  //the position of the crosshair
  Vector2D                      m_vCrosshair;

  //keeps track of the average FPS
  double                         m_dAvFrameTime;


  //flags to turn aids and obstacles etc on/off
  bool  m_bShowWalls;
  bool  m_bShowBinari;
  bool  m_bShowObstacles;
  bool  m_bShowPath;
  bool  m_bShowDetectionBox;
  bool  m_bShowWanderCircle;
  bool  m_bShowFeelers;
  bool  m_bShowSteeringForce;
  bool  m_bShowFPS;
  bool  m_bRenderNeighbors;
  bool  m_bViewKeys;
  bool  m_bShowCellSpaceInfo;


  void CreateObstacles();

  void CreateWalls();

  void CreateBinari();

  //Semaforo
  void red();


public:
  
  MondoDiGioco(int cx, int cy);

  ~MondoDiGioco();

  void  Update(double time_elapsed);

  void  Render();

  //loads an environment from a file
  bool LoadMap(const std::string& FileName); 

  void  NonPenetrationContraint(Veicolo* v){EnforceNonPenetrationConstraint(v, m_Vehicles);}

  void  TagVehiclesWithinViewRange(BaseGameEntity* pVehicle, double range)
  {
    TagNeighbors(pVehicle, m_Vehicles, range);
  }

  void  TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, double range)
  {
    TagNeighbors(pVehicle, m_Obstacles, range);
  }

  const std::vector<Wall2D>&          Walls(){return m_Walls;}   
  const std::vector<Binario2D>&          Binari(){return m_Binari;}  
  CellSpacePartition<Veicolo*>*       CellSpace(){return m_pCellSpace;}
  const std::vector<BaseGameEntity*>& Obstacles()const{return m_Obstacles;}
  const std::vector<Veicolo*>&        Agents(){return m_Vehicles;}
 // const std::vector<Semaforo>&        Semafori(){return m_Semafori;}
  PathManager<Raven_PathPlanner>* const    GetPathManager(){return m_pPathManager;}
  //handle WM_COMMAND messages
  void        HandleKeyPresses(WPARAM wParam);
  void        HandleMenuItems(WPARAM wParam, HWND hwnd);
  
  void        TogglePause(){m_bPaused = !m_bPaused;}
  bool        Paused()const{return m_bPaused;}
  //removes the last bot to be added
  void RemoveBot();
  Vector2D    Crosshair()const{return m_vCrosshair;}
  void        SetCrosshair(POINTS p);
  void        SetCrosshair(Vector2D v){m_vCrosshair=v;}

  int   cxClient()const{return m_cxClient;}
  int   cyClient()const{return m_cyClient;}
  //this method is called when the user clicks the left mouse button. If there
  //is a possessed bot, this fires the weapon, else does nothing
  void        ClickLeftMouseButton(POINTS p);
   //if a bot is possessed the keyboard is polled for user input and any 
  //relevant bot methods are called appropriately
  void        GetPlayerInput()const;
  const Raven_Map* const                   GetMap()const{return m_pMap;}
  Raven_Map* const                         GetMap(){return m_pMap;}
  bool  RenderWalls()const{return m_bShowWalls;}
  bool  RenderBinari()const{return m_bShowBinari;}
  bool  RenderObstacles()const{return m_bShowObstacles;}
  bool  RenderPath()const{return m_bShowPath;}
  bool  RenderDetectionBox()const{return m_bShowDetectionBox;}
  bool  RenderWanderCircle()const{return m_bShowWanderCircle;}
  bool  RenderFeelers()const{return m_bShowFeelers;}
  bool  RenderSteeringForce()const{return m_bShowSteeringForce;}

  bool  RenderFPS()const{return m_bShowFPS;}
  void  ToggleShowFPS(){m_bShowFPS = !m_bShowFPS;}
  
  void  ToggleRenderNeighbors(){m_bRenderNeighbors = !m_bRenderNeighbors;}
  bool  RenderNeighbors()const{return m_bRenderNeighbors;}
  
  void  ToggleViewKeys(){m_bViewKeys = !m_bViewKeys;}
  bool  ViewKeys()const{return m_bViewKeys;}
  void  AggiungiProiettileLeggero(Veicolo* shooter, Vector2D target);
  void AddRailGunSlug(Veicolo* shooter, Vector2D target);
  void AddShotGunPellet(Veicolo* shooter, Vector2D target);
  const std::vector<Veicolo*>&             GetAllVeicoli()const{return m_Vehicles;}
  void AddVeicoli(unsigned int NumVeicoliToAdd);
  void AddRocket(Veicolo* shooter, Vector2D target);
   //returns a vector of pointers to bots in the FOV of the given bot
  std::vector<Veicolo*> GetAllBotsInFOV(const Veicolo* pBot)const;

  //returns true if the second bot is unobstructed by walls and in the field
  //of view of the first.
  bool        isSecondVisibleToFirst(const Veicolo* pFirst,
                                     const Veicolo* pSecond)const;
  //returns true if the ray between A and B is unobstructed.
  bool        isLOSOkay(Vector2D A, Vector2D B)const;
   //returns true if a bot of size BoundingRadius cannot move from A to B
  //without bumping into world geometry
  bool isPathObstructed(Vector2D A, Vector2D B, double BoundingRadius = 0)const;

    void  TagRaven_BotsWithinViewRange(BaseGameEntity* pRaven_Bot, double range)
              {TagNeighbors(pRaven_Bot, m_Vehicles, range);} 

};



#endif