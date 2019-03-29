#include "Raven_Map.h"
#include "EnumerazioneOggetti.h"
#include "misc/Cgdi.h"
#include "HandyGraphFunctions.h"
//#include "Raven_Door.h"
#include "game/EntityManager.h"
#include "constants.h"
//#include "lua/Raven_Scriptor.h"
#include "misc/Stream_Utility_Functions.h"
#include "Trigger_HealthGiver.h"
#include "Trigger_WeaponGiver.h"
//#include "triggers/Trigger_OnButtonSendMsg.h"/
#include "Trigger_SoundNotify.h"

#include "Raven_UserOptions.h"
#include "SparseGraph.h"

//uncomment to write object creation/deletion to debug console
#define  LOG_CREATIONAL_STUFF
#include "debug/DebugConsole.h"


//----------------------------- ctor ------------------------------------------
//-----------------------------------------------------------------------------
Raven_Map::Raven_Map():m_pNavGraph(NULL),
                       m_pSpacePartition(NULL),
                       m_iSizeY(0),
                       m_iSizeX(0),
                       m_dCellSpaceNeighborhoodRange(0)
{
}
//------------------------------ dtor -----------------------------------------
//-----------------------------------------------------------------------------
Raven_Map::~Raven_Map()
{
  Clear();
}


//---------------------------- Clear ------------------------------------------
//
//  deletes all the current objects ready for a map load
//-----------------------------------------------------------------------------
void Raven_Map::Clear()
{
  //delete the triggers
  m_TriggerSystem.Clear();

  /*delete the doors
  std::vector<Raven_Door*>::iterator curDoor = m_Doors.begin();
  for (curDoor; curDoor != m_Doors.end(); ++curDoor)
  {
    delete *curDoor;
  }

  m_Doors.clear();*/

  std::vector<Binario2D*>::iterator curBinario = m_Binari.begin();
  for (curBinario; curBinario != m_Binari.end(); ++curBinario)
  {
    delete *curBinario;
  }

   std::vector<Rimessa*>::iterator curRi = m_Rimesse.begin();
  for (curRi; curRi != m_Rimesse.end(); ++curRi)
  {
    delete *curRi;
  }

     std::vector<Semaforo*>::iterator curSem = m_Semafori.begin();
  for (curSem; curSem != m_Semafori.end(); ++curSem)
  {
    delete *curSem;
  }

    std::vector<Wall2D*>::iterator curWall = m_Walls.begin();
  for (curWall; curWall != m_Walls.end(); ++curWall)
  {
    delete *curWall;
  }

  m_Walls.clear();
  m_Binari.clear();
  m_SpawnPoints.clear();
  m_Semafori.clear();

  //delete the navgraph
  delete m_pNavGraph;   

  //delete the partioning info
  delete m_pSpacePartition;
}


//----------------------------- AddWall ---------------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddWall(std::ifstream& in)
{
  m_Walls.push_back(new Wall2D(in));
}

Wall2D* Raven_Map::AddWall(Vector2D from, Vector2D to)
{
  Wall2D* w = new Wall2D(from, to);

  m_Walls.push_back(w);

  return w;
}

//----------------------------- AddBinario ---------------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddBinario(std::ifstream& in)
{
  m_Binari.push_back(new Binario2D(in));
}

Binario2D* Raven_Map::AddBinario(Vector2D from, Vector2D to)
{
  Binario2D* w = new Binario2D(from, to);

  m_Binari.push_back(w);

  return w;
}

//----------------------------- AddRimessa ---------------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddRimessa(std::ifstream& in)
{
  m_Rimesse.push_back(new Rimessa(in));
}

Rimessa* Raven_Map::AddRimessa(Vector2D from, Vector2D to)
{
 Rimessa* w = new Rimessa(from, to);

  m_Rimesse.push_back(w);

  return w;
}

//---------------------------- AddSemaforo----------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddSemaforo(std::ifstream& in)
{ 

  m_Semafori.push_back(new Semaforo(in));


}

Semaforo* Raven_Map::AddSemaforo(Vector2D from, Vector2D to)
{
 Semaforo* w = new Semaforo(from, to);

  m_Semafori.push_back(w);

  return w;
}
/*/--------------------------- AddDoor -----------------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddDoor(std::ifstream& in)
{
  Raven_Door* pDoor = new Raven_Door(this, in);

  m_Doors.push_back(pDoor);

  //register the entity 
  EntityMgr->RegisterEntity(pDoor);
}

//--------------------------- AddDoorTrigger ----------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddDoorTrigger(std::ifstream& in)
{
  Trigger_OnButtonSendMsg<Raven_Bot>* tr = new Trigger_OnButtonSendMsg<Raven_Bot>(in);

  m_TriggerSystem.Register(tr);

  //register the entity 
  EntityMgr->RegisterEntity(tr);
  
}
*/



//---------------------------- AddSpawnPoint  ----------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddSpawnPoint(std::ifstream& in)
{
  double x, y, dummy3,dummy;

 in >> dummy3 >> x >> y >> dummy >> dummy;                   //dummy values are artifacts from the map editor

 m_SpawnPoints.push_back(Vector2D(x,y));
  
   }


//---------------------------- AddFlag  ----------------------------------
//------------------------------------------------------------------------
//Per caricare i flag degli scambi

void Raven_Map::AddFlag(std::ifstream& in)
{
  double x, y, dummy3,dummy;

 in >> dummy >> x >> y >> dummy >> dummy3 >> dummy;                   //dummy values are artifacts from the map editor

  m_Flag.push_back(dummy3);
  
   }


//---------------------------- AddPrimoSemaforo  ----------------------------------
//------------------------------------------------------------------------
//Per caricare la posizione del primo semaforo di uno scambio
void Raven_Map::AddPrimoSemaforo(std::ifstream& in)
{
  double x, y, dummy3,dummy;

 in >> x >> y >> dummy >> dummy >> dummy3 >> dummy;                   //dummy values are artifacts from the map editor

  m_PrimoSemaforo.push_back(Vector2D(x,y));
  
   }
//----------------------- AddHealth__Giver ----------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddHealth_Giver(std::ifstream& in)
{
  Trigger_HealthGiver* hg = new Trigger_HealthGiver(in);

  m_TriggerSystem.Register(hg);

  //let the corresponding navgraph node point to this object
  NavGraph::NodeType& node = m_pNavGraph->GetNode(hg->GraphNodeIndex());

  node.SetExtraInfo(hg);

  //register the entity 
  EntityMgr->RegisterEntity(hg);
}

//----------------------- AddWeapon__Giver ----------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::AddWeapon_Giver(int type_of_weapon, std::ifstream& in)
{
  Trigger_WeaponGiver* wg = new Trigger_WeaponGiver(in);

  wg->SetEntityType(type_of_weapon);

  //add it to the appropriate vectors
  m_TriggerSystem.Register(wg);

  //let the corresponding navgraph node point to this object
  NavGraph::NodeType& node = m_pNavGraph->GetNode(wg->GraphNodeIndex());

  node.SetExtraInfo(wg);

  //register the entity 
  EntityMgr->RegisterEntity(wg);
}


//------------------------- LoadFlag ------------------------------------
//
//  Richiama la funzione che carica i flag degli scambi (dx o sx) dal file .map
//-----------------------------------------------------------------------------
bool Raven_Map::LoadFlag(const std::string& filename)
{  
  std::ifstream in(filename.c_str());
  if (!in)
  {
    ErrorBox("Nome di mappa errato!");
    return false;
  }

  Clear();
 while (!in.eof())
  {
	 int Flag = 12;
	  in >> Flag;
	  if(Flag == 12)
		  AddFlag(in);
	  }
	    return true;
}



//------------------------- LoadPrimoSemaforo ------------------------------------
//
//  Richiama la funzione che carica il primo semaforo di uno scambio  dal file .map
//-----------------------------------------------------------------------------
bool Raven_Map::LoadPrimoSemaforo(const std::string& filename)
{  
  std::ifstream in(filename.c_str());
  if (!in)
  {
    ErrorBox("Nome di mappa errato!");
    return false;
  }

  Clear();
 while (!in.eof())
  {
	 int Flag = 12;
	  in >> Flag;
	  if(Flag == 12)
		  AddPrimoSemaforo(in);
	  }
	    return true;
}
//------------------------- LoadMap ------------------------------------
//
//  sets up the game environment from map file
//-----------------------------------------------------------------------------
bool Raven_Map::LoadMap(const std::string& filename)
{ 

  std::ifstream in(filename.c_str());
  if (!in)
  {
    ErrorBox("Nome di mappa errato!");
    return false;
  }

    Clear();
    BaseGameEntity::ResetNextValidID();
//Questa � la funzione che richiama AddFlag per caricare i flag degli scambi	
  LoadFlag(filename);
  //Questa � la funzione che richiama AddPrimoSemaforo per caricare il
  //primo semaforo di uno scambio	
  LoadPrimoSemaforo(filename);
  //first of all read and create the navgraph. This must be done before
  //the entities are read from the map file because many of the entities
  //will be linked to a graph node (the graph node will own a pointer
  //to an instance of the entity)
 m_pNavGraph = new NavGraph(false);
  
 m_pNavGraph->Load("graf2.map");

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "NavGraph per " << filename << " caricato correttamente" << "";
	debug_con << "Numero nodi " << m_pNavGraph->NumNodes() <<  "";
	debug_con << "Numero edges " << m_pNavGraph->NumEdges() <<  "";
#endif

  //determine the average distance between graph nodes so that we can
  //partition them efficiently
  m_dCellSpaceNeighborhoodRange = CalculateAverageGraphEdgeLength(*m_pNavGraph) + 1;

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Average edge length is " << CalculateAverageGraphEdgeLength(*m_pNavGraph) << "";
#endif

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Range di vicinananza settato a " << m_dCellSpaceNeighborhoodRange << "";
#endif
  //partition the graph nodes
 PartitionNavGraph();
  //load in the map size and adjust the client window accordingly
  in >> m_iSizeX >> m_iSizeY;


#ifdef LOG_CREATIONAL_STUFF
   debug_con << "Dimensioni della finestra: " << m_iSizeX << "" << m_iSizeY << "";
#endif

#ifdef LOG_CREATIONAL_STUFF
   debug_con << "Partiziono i nodi del grafo di navigazione..." << "";
#endif




  //get the handle to the game window and resize the client area to accommodate
  //the map
  extern char* g_szApplicationName;
  extern char* g_szWindowClassName;
  HWND hwnd = FindWindow(g_szWindowClassName, g_szApplicationName);
  const int ExtraHeightRqdToDisplayInfo = 50;
  ResizeWindow(hwnd, m_iSizeX, m_iSizeY+ExtraHeightRqdToDisplayInfo);

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Caricamento mappa..." << "";
#endif

 
  //now create the environment entities
  while (!in.eof())
  {   
    //get type of next map object
    int EntityType;
    
    in >> EntityType;

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Creo " << GetNameOfType(EntityType) << "";
#endif

    //create the object
    switch(EntityType)
    {

    case type_wall:
 
        AddWall(in); break;

    case type_binario:
 
        AddBinario(in);
		break;

    case type_rimessa:
 
        AddRimessa(in);
		break;

    case type_semaforo:
        AddSemaforo(in);
		break;

    case type_sliding_door:
 
//        AddDoor(in);
		break;

    case type_door_trigger:
 
//        AddDoorTrigger(in);
		break;

   case type_spawn_point:
     
       AddSpawnPoint(in);
	   break;

   case type_health:
     
       AddHealth_Giver(in); 
	   break;

   case type_shotgun:
     
       AddWeapon_Giver(type_shotgun, in); 
	   break;

   case type_rail_gun:
     
       AddWeapon_Giver(type_rail_gun, in);
	   break;

   case type_rocket_launcher:
     
       AddWeapon_Giver(type_rocket_launcher, in);
	   break;

    default:
      
     throw std::runtime_error("<Map::Load>: Si sta cercando di caricare un oggetto indefinito");

      return false;
      
    }//end switch
  }

#ifdef LOG_CREATIONAL_STUFF
    debug_con << filename << " caricata correttamente" << "";
#endif

   //calculate the cost lookup table
  m_PathCosts = CreateAllPairsCostsTable(*m_pNavGraph);

  return true;
}





//------------- CalculateCostToTravelBetweenNodes -----------------------------
//
//  Uses the pre-calculated lookup table to determine the cost of traveling
//  from nd1 to nd2
//-----------------------------------------------------------------------------
double 
Raven_Map::CalculateCostToTravelBetweenNodes(int nd1, int nd2)const
{
  assert (nd1>=0 && nd1<m_pNavGraph->NumNodes() &&
          nd2>=0 && nd2<m_pNavGraph->NumNodes() &&
          "<Raven_Map::CostBetweenNodes>: invalid index");

  return m_PathCosts[nd1][nd2];
}




//-------------------------- PartitionEnvironment -----------------------------
//-----------------------------------------------------------------------------
void Raven_Map::PartitionNavGraph()
{
  if (m_pSpacePartition) delete m_pSpacePartition;

  m_pSpacePartition = new CellSpacePartition<NavGraph::NodeType*>(m_iSizeX,
                                                                  m_iSizeY,
                                                                  10,
                                                                  10,
                                                                  m_pNavGraph->NumNodes());

  //add the graph nodes to the space partition
  NavGraph::NodeIterator NodeItr(*m_pNavGraph);
  for (NavGraph::NodeType* pN=NodeItr.begin();!NodeItr.end();pN=NodeItr.next())
  {
    m_pSpacePartition->AddEntity(pN);
  }   
}

///---------------------------- AddSoundTrigger --------------------------------
//
//  given the bot that has made a sound, this method adds a SoundMade trigger
//-----------------------------------------------------------------------------
void Raven_Map::AddSoundTrigger(Veicolo* pSoundSource, double range)
{
  m_TriggerSystem.Register(new Trigger_SoundNotify(pSoundSource, range));
}

//----------------------- UpdateTriggerSystem ---------------------------------
//
//  given a container of entities in the world this method updates them against
//  all the triggers
//-----------------------------------------------------------------------------
void Raven_Map::UpdateTriggerSystem(std::vector<Veicolo*>& bots)
{
  m_TriggerSystem.Update(bots);
}

//------------------------- GetRandomNodeLocation -----------------------------
//
//  returns the position of a graph node selected at random
//-----------------------------------------------------------------------------
Vector2D Raven_Map::GetRandomNodeLocation()const
{
  NavGraph::ConstNodeIterator NodeItr(*m_pNavGraph);
  int RandIndex = RandInt(0, m_pNavGraph->NumActiveNodes()-1);
  const NavGraph::NodeType* pN = NodeItr.begin();
  while (--RandIndex > 0)
  {
    pN = NodeItr.next();
  }
Vector2D p;
  p.x = 140;
 p.y= 145;
  return p;//pN->Pos();
}


//--------------------------- Render ------------------------------------------
//-----------------------------------------------------------------------------
void Raven_Map::Render()
{
  //render the navgraph
  //if (UserOptions->m_bShowGraph)
  //{
    GraphHelper_DrawUsingGDI<NavGraph>(*m_pNavGraph, Cgdi::grey, UserOptions->m_bShowNodeIndices);
  //}

  /*/render any doors
  std::vector<Raven_Door*>::iterator curDoor = m_Doors.begin();
  for (curDoor; curDoor != m_Doors.end(); ++curDoor)
  {
    (*curDoor)->Render();
  }
*/
  //render all the triggers
  m_TriggerSystem.Render();

  //render dei binari
  std::vector<Binario2D*>::const_iterator curBinario = m_Binari.begin();
  for (curBinario; curBinario != m_Binari.end(); ++curBinario)
  {
    gdi->BrownPen();
    (*curBinario)->Render();
  }

//Render delle Rimesse
  std::vector<Rimessa*>::const_iterator curRi = m_Rimesse.begin();
  for (curRi; curRi != m_Rimesse.end(); ++curRi)
  {
    gdi->GreyBrush();
    gdi->BlackPen();
    (*curRi)->Render();
  }

 //render all the walls
  std::vector<Wall2D*>::const_iterator curWall = m_Walls.begin();
  for (curWall; curWall != m_Walls.end(); ++curWall)
  {
    gdi->ThickBlackPen();
    (*curWall)->Render();
  }
}
