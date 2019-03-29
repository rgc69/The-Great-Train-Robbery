#ifndef RAVEN_MAP_H
#define RAVEN_MAP_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_Map.h
//
//   Desc:   this class creates and stores all the entities that make up the
//          Raven game environment. (Binarios, bots, health etc)
//
//          It can read a Raven map editor file and recreate the necessary
//          geometry.
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>
#include "SparseGraph.h"
#include "Binario2D.h"
#include "Semaforo.h"
#include "Rimessa.h"
#include "Trigger.h"
#include "Veicolo.h"
#include "GraphEdgeTypes.h"
#include "GraphNodeTypes.h"
#include "CellSpacePartition.h"
#include "TriggerSystem.h"
#include "Wall2D.h"
class BaseGameEntity;
//class Raven_Door;


class Raven_Map
{
public:

  typedef NavGraphNode<Trigger<Veicolo>*>         GraphNode;
  typedef SparseGraph<GraphNode, NavGraphEdge>      NavGraph;
  typedef CellSpacePartition<NavGraph::NodeType*>   CellSpace;

   typedef Trigger<Veicolo>                          TriggerType;
   typedef TriggerSystem<TriggerType>                TriggerSystem;
  int n;
private:
 
  //the Binarios that comprise the current map's architecture. 
  std::vector<Binario2D*>                m_Binari;

    //the walls that comprise the current map's architecture. 
  std::vector<Wall2D*>                m_Walls;

  //trigger are objects that define a region of space. When a raven bot
  //enters that area, it 'triggers' an event. That event may be anything
  //from increasing a bot's health to opening a door or requesting a lift.
  TriggerSystem                      m_TriggerSystem;    

  //Questo vettore contiene le posizioni di respawn. Quando un veicolo è
  //istanziato,apparirà in un punto scelto a random da questo vettore
    std::vector<Vector2D>              m_SpawnPoints;

  //Questo vettore contiene la posizione del primo semaforo di uno scambio
    std::vector<Vector2D>              m_PrimoSemaforo;

    //Per il Flag dei semafafori dx o sx
   std::vector<int>                   m_Flag;  
	
	
	//Questo vettore contiene le posizioni dei semafori.
  std::vector<Semaforo*>              m_Semafori;

   //Le rimesse presenti nell'attuale architettura della mappa. 
   std::vector<Rimessa*>                m_Rimesse;

  //a map may contain a number of sliding doors.
  //std::vector<Raven_Door*>           m_Doors;
 
  //this map's accompanying navigation graph
  NavGraph*                          m_pNavGraph;  

  //the graph nodes will be partitioned enabling fast lookup
  CellSpace*                        m_pSpacePartition;

  //the size of the search radius the cellspace partition uses when looking for 
  //neighbors 
  double                             m_dCellSpaceNeighborhoodRange;

  int m_iSizeX;
  int m_iSizeY;
  
  void  PartitionNavGraph();

  //this will hold a pre-calculated lookup table of the cost to travel from
  //one node to any other.
  std::vector<std::vector<double> >  m_PathCosts;


    //stream constructors for loading from a file
  void AddBinario(std::ifstream& in);
  void AddSpawnPoint(std::ifstream& in);
  void AddRimessa(std::ifstream& in);
  void AddSemaforo(std::ifstream& in);
  void AddFlag(std::ifstream& in);
  void AddPrimoSemaforo(std::ifstream& in);
  void AddHealth_Giver(std::ifstream& in);
  void AddWall(std::ifstream& in);
  void AddWeapon_Giver(int type_of_weapon, std::ifstream& in);
  //void AddDoor(std::ifstream& in);
 // void AddDoorTrigger(std::ifstream& in);

  void Clear();
  
public:
  
  Raven_Map();  
  ~Raven_Map();

  void Render();

  //carica l'ambiente di gioco da un file
  bool LoadMap(const std::string& FileName); 

    //carica i flag degli scambi da un file
  bool LoadFlag(const std::string& FileName); 

     //carica la posizione del primo semaforo di uno scambio
  bool LoadPrimoSemaforo(const std::string& FileName); 

    //adds a wall and returns a pointer to that wall. (this method can be
  //used by objects such as doors to add walls to the environment)
  Wall2D* AddWall(Vector2D from, Vector2D to);

  //adds a Binario and returns a pointer to that Binario. (this method can be
  //used by objects such as doors to add Binarios to the environment)
  Binario2D* AddBinario(Vector2D from, Vector2D to);

    //aggiunge una rimessa e ritorna un puntatore a quella rimessa
  Rimessa* AddRimessa(Vector2D from, Vector2D to);

      //aggiunge un semaforo e ritorna un puntatore a quel semaforo
 Semaforo* AddSemaforo(Vector2D from, Vector2D to);

 void    AddSoundTrigger(Veicolo* pSoundSource, double range);

 double   CalculateCostToTravelBetweenNodes(int nd1, int nd2)const;

  //returns the position of a graph node selected at random
  Vector2D GetRandomNodeLocation()const;
  
  
  void  UpdateTriggerSystem(std::vector<Veicolo*>& bots);

  const Raven_Map::TriggerSystem::TriggerList&  GetTriggers()const{return m_TriggerSystem.GetTriggers();}
  const std::vector<Binario2D*>&     GetBinari()const{return m_Binari;}
  const std::vector<Wall2D*>&        GetWalls()const{return m_Walls;}
  NavGraph&                          GetNavGraph()const{return *m_pNavGraph;}
  //std::vector<Raven_Door*>&          GetDoors(){return m_Doors;}
  const std::vector<Vector2D>&       GetSpawnPoints()const{return m_SpawnPoints;}
  const std::vector<Rimessa*>&       GetRimesse()const{return m_Rimesse;}
  //Per ritornare in un Vettore 2 D la pozione x di uno dei quattro valori
  Vector2D PosizioneSp(int n)const{return m_SpawnPoints[n];}
  const std::vector<Semaforo*>&      GetSemafori()const{return m_Semafori;}
  const std::vector<Vector2D>&       GetPrimoSemaforo()const{return m_PrimoSemaforo;}
  CellSpace* const                   GetCellSpace()const{return m_pSpacePartition;}
  Vector2D                           GetRandomSpawnPoint(){return m_SpawnPoints[RandInt(0,m_SpawnPoints.size()-1)];}
  int                                GetSizeX()const{return m_iSizeX;}
  int                                GetSizeY()const{return m_iSizeY;}
  int                                GetMaxDimension()const{return Maximum(m_iSizeX, m_iSizeY);}
  double                             GetCellSpaceNeighborhoodRange()const{return m_dCellSpaceNeighborhoodRange;}
   //Per lo scambio dei semafori
  int Flag(int n)const{return m_Flag[n];}
};



#endif
  