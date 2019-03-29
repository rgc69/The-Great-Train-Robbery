#include "MondoDiGioco.h"
#include "Veicolo.h"
#include "constants.h"
#include "Obstacle.h"
#include "2d/Geometry.h"
#include "Binario2D.h"
#include "2d/Transformations.h"
#include "SistemaMovimento.h"
#include "time/PrecisionTimer.h"
#include "misc/Smoother.h"
#include "ParamLoader.h"
#include "misc/WindowUtils.h"
#include "misc/Stream_Utility_Functions.h"
#include "EnumerazioneOggetti.h"
#include "Proiettili.h"
#include "EntityManager.h"
#include "resource.h"
#include "Proiettile_Leggero.h"
#include "SistemaDiArma.h"
#include "SpotIncidente.h"
#include "Raven_Map.h"
#include "DebugConsole.h"
#include "EntitaBase.h"
#include "Projectile_Rocket.h"
#include "Projectile_Pellet.h"
#include "Projectile_Slug.h"
#include "Goal_Think.h"
#include "Raven_SensoryMemory.h"
#include "WallIntersectionTests.h"
#include <list>
using std::list;
#include "Raven_Goal_Types.h"
#define  LOG_CREATIONAL_STUFF

 int vite = 4;
 int coloreSem  = 0;
 int coloreSem2 = 2;
 int x,y;
 int variabile = 0;
//------------------------------- ctor -----------------------------------
//------------------------------------------------------------------------
MondoDiGioco::MondoDiGioco(int cx, int cy):

            m_cxClient(cx),
            m_cyClient(cy),
            m_bPaused(false),
            m_vCrosshair(Vector2D(cxClient()/2.0, cxClient()/2.0)),
            m_bShowBinari(false),
            m_bShowObstacles(false),
            m_bShowPath(false),
            m_bShowWanderCircle(false),
            m_bShowSteeringForce(false),
            m_bShowFeelers(false),
            m_bShowDetectionBox(false),
            m_bShowFPS(true),
            m_dAvFrameTime(0),
            m_pPath(NULL),
            m_bRenderNeighbors(false),
            m_bViewKeys(false),
            m_bShowCellSpaceInfo(false),
			m_pGraveMarkers(NULL),
			m_pMap(NULL),
			m_pPathManager(NULL),
			m_bRemoveABot(false)
		
{
 //Carichiamo la mappa di default
 LoadMap("level1.map");

   //Per settare il tempo di permanenza di CRASH!
   m_pGraveMarkers = new SpotIncidente(1.5);


  //setup the spatial subdivision class
  m_pCellSpace = new CellSpacePartition<Veicolo*>((double)cx, (double)cy, Prm.NumCellsX, Prm.NumCellsY, Prm.NumVeicoli);

  
  double border = 30;
 
}


//-------------------------------- dtor ----------------------------------
//------------------------------------------------------------------------
MondoDiGioco::~MondoDiGioco()
{
  Clear();
  delete m_pCellSpace;
  delete m_pGraveMarkers;
  delete m_pPath;
  delete m_pPathManager;
}

//---------------------------- Clear ------------------------------------------
//
//  distrugge tutti gli oggetti correnti per un nuovo mondo di gioco
//-----------------------------------------------------------------------------
void MondoDiGioco::Clear()
{
#ifdef LOG_CREATIONAL_STUFF
    debug_con << "\n------------------------------ Clearup -------------------------------" <<"";
#endif

  //cancella i veicoli
std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {
#ifdef LOG_CREATIONAL_STUFF
    debug_con << "deleting entity id: " << (*curVehicle)->ID() << " of type "
              << GetNameOfType((*curVehicle)->EntityType()) << "(" << (*curVehicle)->EntityType() << ")" <<"";
#endif

    delete *curVehicle;
  }

  //cancella ogni proiettile attivo
  std::list<Proiettile*>::iterator curP = m_Projectiles.begin();
  for (curP; curP != m_Projectiles.end(); ++curP)
  {
#ifdef LOG_CREATIONAL_STUFF
    debug_con << "deleting projectile id: " << (*curP)->ID() << "";
#endif

    delete *curP;
  }

  //cancella i contenitori degli oggetti
  m_Projectiles.clear();
  m_Vehicles.clear();
 }
//-------------------------------RemoveBot ------------------------------------
//
//  removes the last bot to be added from the game
//-----------------------------------------------------------------------------
void MondoDiGioco::RemoveBot()
{
  m_bRemoveABot = true;
}

//----------------------------- Update -----------------------------------
//------------------------------------------------------------------------
void MondoDiGioco::Update(double time_elapsed)
{ 
if (m_bPaused) return;


  //Test per l'impatto dei Veicoli
   for (int n = 0; n < Prm.NumVeicoli; ++n);
	
//Vettore contenente le posizioni degli SpawnPoints
std::vector<Vector2D>::const_iterator curSp = m_pMap->GetSpawnPoints().begin();
  //for (curSp; curSp != m_pMap->GetSpawnPoints().end(); ++curSp)
//{
   	if( m_Vehicles[Prm.NumVeicoli-1]->isAtPositionLargest(m_Vehicles[Prm.NumVeicoli-n]->Pos())
		&& !(m_Vehicles[Prm.NumVeicoli-1]->isAtPositionLarghissima(*curSp))) 
		
{ 	
		 vite--;
	
		std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {
	  
   if(vite <= 0)
   return;
	      //crea la scritta
     // m_pGraveMarkers->AddGrave((*curVehicle)->Pos());
	  (*curVehicle)->SetDead();
      (*curVehicle)->SetSpawning();
	  
	       }
  //}
	}

	
 m_pGraveMarkers->Update();
 GetPlayerInput();

  //update all the queued searches in the path manager
  m_pPathManager->UpdateSearches();

  const int SampleRate = 10;
  static Smoother<double> FrameRateSmoother(SampleRate, 0.0);

  m_dAvFrameTime = FrameRateSmoother.Update(time_elapsed);
  
 //update di ogni proiettile corrente
  std::list<Proiettile*>::iterator curP = m_Projectiles.begin();
  while (curP != m_Projectiles.end())
  {
    //test per ogni proiettile morto e rimozione se necessaria
    if (!(*curP)->isDead())
    {
      (*curP)->Update();

      ++curP;
    }
    else
    {    
      delete *curP;

      curP = m_Projectiles.erase(curP);
    }  
  }

  //update dei Veicoli
  bool bSpawnPossible = true;

//Un vettore che contiene tutte le entità Treni
std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {
  
    //se lo stato di questo veicolo è 'respawning', cerca di riportarlo in
    //uno spawn point libero

    if ((*curVehicle)->isSpawning() && bSpawnPossible)
    {
      bSpawnPossible = AttemptToAddVeicolo(*curVehicle);
    }
    
    //se lo stato di questo veicolo è 'dead' scrive CRASH nella sua locazione 
    //corrente, quindi cambia il suo stato in 'respawning'
    else if ((*curVehicle)->isDead())
    {
	      //crea la scritta
      m_pGraveMarkers->AddGrave((*curVehicle)->Pos());
	  
	  if(m_Vehicles[Prm.NumVeicoli-1]->isDead())
		    {
      vite--;
	   if(vite <= 0)
   return;
	   }
      //cambia lo stato in spawning
      (*curVehicle)->SetSpawning();
    }

    //se è in vita, update del veicolo.
    else if ( (*curVehicle)->isAlive())
    {
      (*curVehicle)->Update(time_elapsed); 
	  
    }  
  } 
   //update the triggers
  m_pMap->UpdateTriggerSystem(m_Vehicles);

}
  
//----------------------------- AttemptToAddBot -------------------------------
//-----------------------------------------------------------------------------
bool MondoDiGioco::AttemptToAddVeicolo(Veicolo* pVeicolo)
{
  //make sure there are some spawn points available
  if (m_pMap->GetSpawnPoints().size() <= 0)
  {
    ErrorBox("La mappa non ha Spawn Points!"); return false;
  }

  //we'll make the same number of attempts to spawn a Veicolo this update as
  //there are spawn points
  int attempts = m_pMap->GetSpawnPoints().size();

  while (--attempts >= 0)
  { 
    //select a random spawn point
    Vector2D pos = m_pMap->GetRandomSpawnPoint();

    //check to see if it's occupied
	std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  
    bool bAvailable = true;

    for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
    {
      //if the spawn point is unoccupied spawn a Veicolo
      if (Vec2DDistance(pos, (*curVehicle)->Pos()) < (*curVehicle)->BRadius())
      {
        bAvailable = false;
      }
    }

    if (bAvailable)
    {  
      pVeicolo->Spawn(pos);

      return true;   
    }
  }

  return false;
}

//------------------------- HandleKeyPresses -----------------------------
void MondoDiGioco::HandleKeyPresses(WPARAM wParam)
{

  switch(wParam)
  {
     case 'B':
	     	
      TogglePause();
		break;
  }//end switch
}

//---------------------- ClickLeftMouseButton ---------------------------------
//-----------------------------------------------------------------------------
void MondoDiGioco::ClickLeftMouseButton(POINTS p)
{

   
	m_Vehicles[Prm.NumVeicoli-1]->FireWeapon(POINTStoVector(p));
	//m_Vehicles[Prm.NumVeicoli-3]->FireWeapon(POINTStoVector(p));
    //m_Vehicles[a]->FireWeapon(POINTStoVector(p));

}
//------------------------ GetPlayerInput -------------------------------------
//
//  if a Veicolo is possessed the keyboard is polled for user input and any 
//  relevant Veicolo methods are called appropriately
//-----------------------------------------------------------------------------
void MondoDiGioco::GetPlayerInput()const
{
   //for (int a=0; a<Prm.NumVeicoli; ++a)
   //{
     m_Vehicles[Prm.NumVeicoli-1]->RotateFacingTowardPosition(GetClientCursorPosition());
   //}

}

//-------------------------- HandleMenuItems -----------------------------
void MondoDiGioco::HandleMenuItems(WPARAM wParam, HWND hwnd)
{

	     //to grab filenames
   static TCHAR   szFileName[MAX_PATH],
                  szTitleName[MAX_PATH];
  switch(wParam)
  {
     /*case IDM_GAME_LOAD:
           FileOpenDlg(hwnd, szFileName, szTitleName, "La Grande Rapina Al Treno map file (*.map)", "map");
#ifdef LOG_CREATIONAL_STUFF


          debug_con << "Filename: " << szTitleName << "";
 #endif
          if (strlen(szTitleName) > 0)
          {
            LoadMap(szTitleName);
          }

          break;*/

      case IDM_GAME_LOAD:
          
          FileOpenDlg(hwnd, szFileName, szTitleName, "La Grande Rapina Al Treno map file (*.map)", "map");

    
        debug_con << "Filename: " << szTitleName << "";
      

         if (strlen(szTitleName) > 0)
          { 
			    LoadMap(szTitleName);
          }

          break;
		      case ID_VIEW_KEYS:
      {
        ToggleViewKeys();

        CheckMenuItemAppropriately(hwnd, ID_VIEW_KEYS, m_bViewKeys);
      }

      break;

  

      case ID_VIEW_FPS:
      {
        ToggleShowFPS();

        CheckMenuItemAppropriately(hwnd, ID_VIEW_FPS, RenderFPS());
      }

      break;

	   // handle the HELP menu
                case MENU_HELP_ABOUT:                 
                     {
                     //  pop up a message box
                     MessageBox(hwnd, "Version 0.2. All rights reserved.\n  © 2008 SoftDream Inc.", 
                               "La Grande Rapina Al Treno",
                                MB_OK | MB_ICONINFORMATION);
                     } break;
       
  }//end switch
}


//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void MondoDiGioco::Render()
{  
 	m_pGraveMarkers->Render();
    gdi->TransparentText();
    
	//render della mappa
    m_pMap->Render();

std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {
gdi->TextColor(Cgdi::black);
//(*curVehicle)->GetSensoryMem()->RenderBoxesAroundRecentlySensed();
//gdi->TextAtPos((*curVehicle)->Pos(),ttos());
//gdi->TextAtPos((*curVehicle)->Pos()- Vector2D(10,20),ttos((*curVehicle)->ID()));
//gdi->TextAtPos((*curVehicle)->Pos(),ttos(GetNameOfType((*curVehicle)->EntityType())));
//gdi->TextAtPos((*curVehicle)->Pos(),ttos((*curVehicle)->GetNextValidID()));
  }
	for (int n = 0; n < Prm.NumVeicoli; ++n);

if( vite <= 0)
{ 	 
    gdi->TextColor(Cgdi::red);
    gdi->TextAtPos(200,200, "GAME OVER!");
 
   return;
      }

	gdi->TextAtPos(10,15,"LIFE: ");
    gdi->TextAtPos(45,15,ttos(vite));
	gdi->TextAtPos(10,35,"SCORE: $");
    gdi->TextAtPos(75,35,ttos((m_Vehicles[Prm.NumVeicoli-1]->m_iScore) * 100));
 int x = 0,y = 0;
	
 //Renderizza i veicoli
 	for ( n = 0; n < Prm.NumVeicoli; ++n)
    {
	
    m_Vehicles[n]->Render(); 
	} 
    m_Vehicles[Prm.NumVeicoli-1]->m_pWeaponSys->RenderCurrentWeapon();
 
	//Renderizza il secondo treno nel mondo di gioco	 
     //m_Vehicles[Prm.NumVeicoli-2]->Render(); 
     m_Vehicles[Prm.NumVeicoli-2]->SetMaxSpeed(95);
    // m_Vehicles[Prm.NumVeicoli-3]->m_pWeaponSys->RenderCurrentWeapon();
    //m_Vehicles[Prm.NumVeicoli-3]->Render(); 
	 //Renderizza il dollaro e l'energia
	 if(! m_Vehicles[Prm.NumVeicoli-2]->isDead())
		  {    
	 gdi->TextColor(Cgdi::black);
	 gdi->TextAtPos( m_Vehicles[Prm.NumVeicoli-2]->Pos()- Vector2D(4.8,10),"$");
     gdi->TextAtPos( m_Vehicles[Prm.NumVeicoli-2]->Pos()- Vector2D(1,22),ttos( m_Vehicles[Prm.NumVeicoli-2]->m_iHealth));
	   }
if(! m_Vehicles[Prm.NumVeicoli-2]->isDead())
		  {    
	 gdi->TextColor(Cgdi::black);
     gdi->TextAtPos( m_Vehicles[Prm.NumVeicoli-1]->Pos()- Vector2D(1,22),ttos( m_Vehicles[Prm.NumVeicoli-1]->m_iHealth));
	   }
// Render di ogni semaforo

std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
  for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
  {
	  if(coloreSem == 0)
		  {
      gdi->BlackPen();
	  for(int n = 0; n < m_pMap->GetSemafori().size();++n) 
	{ 
if(m_pMap->Flag(n) == 7)
	 {   
	if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
	{ 

   (*curSemaforo)->RenderRedDx();
   (*curSemaforo)->TurnGreenOff();
   (*curSemaforo)->TurnRedOn();
  
    }
}
if(m_pMap->Flag(n) == 6)
	 {   
	if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
	{ 
   (*curSemaforo)->RenderRedSx();
   (*curSemaforo)->TurnGreenOff();
   (*curSemaforo)->TurnRedOn();
    }
}
	  }
	}
	  if(coloreSem == 1)
		  {
      gdi->BlackPen();
	  for(int n = 0; n < m_pMap->GetSemafori().size();++n) 
	{ 
if(m_pMap->Flag(n) == 7)
		  { 
	if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
	{ 
   (*curSemaforo)->RenderGreenDx();
   (*curSemaforo)->TurnRedOff();
   (*curSemaforo)->TurnGreenOn();
}   
	}
if(m_pMap->Flag(n) == 6)
	 {   
	if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
	{ 
   (*curSemaforo)->RenderGreenSx();
   (*curSemaforo)->TurnRedOff();
   (*curSemaforo)->TurnGreenOn();
    }
}
	  }
 }
}
 
 //render di ogni proiettile
 std::list<Proiettile*>::const_iterator curP = m_Projectiles.begin();
  	
  for (curP; curP != m_Projectiles.end(); ++curP)
  {
    (*curP)->Render();

		 //Testiamo l'impatto con i semafori
	 std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
  for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
{ 

		
  if((*curP)->isAtPosition((*curSemaforo)->PosizioneSem()))
	
		{	
(*curP)->m_bDead = true;
   if(coloreSem == 0){//Se semaforo rosso a sinistra....
	   coloreSem = 1; //..semaforo rosso a destra
	  	}
  else{
	  	  coloreSem = 0;//Semaforo rosso a sinistra
	 	}//Fine else
  	}//Fine if
      }//Fine for primo semaforo

  for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
{
  if((*curP)->isAtPosition((*curSemaforo)->PosizioneSem()))
	{ 
   (*curP)->m_bDead = true; 
   
	if(coloreSem2 == 2){
        coloreSem2 = 3;
		coloreSem = 1;
   
	}
  else{
	   coloreSem2 = 2;
       coloreSem = 0;
	}//Fine else
  }//Fine if
   }//Fine for secondo semaforo

  //}//Fine if Veicoli
//}//Fine for Agenti
  }//Fine for list Proiettili

  if (RenderFPS())
  {
    gdi->TextColor(Cgdi::grey);
    gdi->TextAtPos(5, cyClient() - 20, ttos(1.0 / m_dAvFrameTime));
  } 

  if (m_bShowCellSpaceInfo)
  {
    m_pCellSpace->RenderCells();
  }

Vector2D  m_CurrentDestination;

 m_CurrentDestination = m_Vehicles[Prm.NumVeicoli-2]->GetWorld()->GetMap()->GetRandomNodeLocation();
  //  render the path of the bot
 m_Vehicles[Prm.NumVeicoli-2]->GetPathPlanner()->RequestPathToPosition(m_CurrentDestination);
  //gdi->TextAtPos(100,100,ttos(m_Vehicles[Prm.NumVeicoli-2]->GetPathPlanner()->RequestPathToPosition(m_CurrentDestination)));

 
 //////////////////////////Via dalle Rimesse//////////////////////////////////////////


   for (int a = 0; a < Prm.NumVeicoli; ++a)
	   {
//Se le vite sono più di 0 i treni seguono il percorso normale delle rimesse,
//dei binari e dei relativi semafori......
  if (m_Vehicles[a]->m_iHealth >= 1)
{	 
	  //**********TEST*********
	  m_Vehicles[a]->Steering()->WanderOff();

//Vettore contenente le posizioni degli SpawnPoints
std::vector<Vector2D>::const_iterator curSp = m_pMap->GetSpawnPoints().begin();
  for (curSp; curSp != m_pMap->GetSpawnPoints().end(); ++curSp)
{
   for (int a=0; a<Prm.NumVeicoli; ++a)
   {
	   if (m_Vehicles[a]->isAtPosition(*curSp))
	 { 
       Vector2D spawn;
	   spawn = (*curSp);
	  {  
	  delete m_pPath;
      m_pPath = new PathNormale((spawn.x),(spawn.y), true); 
      m_bShowPath = true; 
      m_Vehicles[a]->Steering()->SetPath(m_pPath->GetPathNormale());
      m_Vehicles[a]->Steering()->FollowPathOn();
	  }
	 }

   }
 }
///////////////////////Inizio percorso binari(senza semafori)/////////////////////////
		
for(n = 0; n < m_pMap->GetSemafori().size();++n)  
{
	bool Intersezione = true;
		std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {

  std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();

  for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
  {
	  std::vector<Binario2D*>::const_iterator curBinario = m_pMap->GetBinari().begin();
     for (curBinario; curBinario != m_pMap->GetBinari().end(); ++curBinario)
	 { 

     if ((((*curVehicle)->isAtPositionProx((*curBinario)->PosizioneBin()))))// || 
			 //((*curVehicle)->isAtPosition((*curBinario)->PosizioneBin2()))))
	   {
Vector2D binari;
	 binari = (*curBinario)->PosizioneBin2(); 
     delete m_pPath;
      m_pPath = new PathNormale((binari.x),(binari.y), true); 
      m_bShowPath = true; 
     (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
     (*curVehicle)->Steering()->FollowPathOn();
	 
	  }//Fine if
	 }//Fine for Binario
}//Fine for Semaforo

//Se il semaforo è verso destra (flag nel file esterno == 7)...
	if(m_pMap->Flag(n) == 7)
	{  
 std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
	 for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
	 {		
		 if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  { 
		 //...assegna a Flag le coordinate del primo semaforo tra quelli con flag == 7
		Vector2D flag; 
	    flag =m_pMap->GetPrimoSemaforo()[n];

      if ((*curVehicle)->isAtPositionProx((*curSemaforo)->Dx()))
		  {
     delete m_pPath;
      m_pPath = new PathNormale(( (*curSemaforo)->Dx(Intersezione).x),( (*curSemaforo)->Dx(Intersezione).y), true); 
      m_bShowPath = true; 
     (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
     (*curVehicle)->Steering()->FollowPathOn();
	  }//Fine if Veicolo
	  }
  }//Fine for Semaforo
}//Fine if Flag == 7

	 	if(m_pMap->Flag(n) == 6)
	{ 
			 std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
	 for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
	 {
	 if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  { 
//gdi->Circle((*curSemaforo)->Sx(Intersezione).x,(*curSemaforo)->Sx(Intersezione).y,3);
//gdi->Circle((*curSemaforo)->ScambioSx().x,(*curSemaforo)->ScambioSx().y,3);
//gdi->Circle((*curSemaforo)->ScambioSx(Intersezione).x,(*curSemaforo)->ScambioSx(Intersezione).y,3);	
//gdi->Circle((*curSemaforo)->Sx().x,(*curSemaforo)->Sx().y,3);

	 if ((*curVehicle)->isAtPositionProx((*curSemaforo)->Sx()))
		  {
     delete m_pPath;
      m_pPath = new PathNormale(( (*curSemaforo)->Sx(Intersezione).x),( (*curSemaforo)->Sx(Intersezione).y), true); 
      m_bShowPath = true; 
     (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
     (*curVehicle)->Steering()->FollowPathOn();
	 }//Fine if Flag 
	  }
	 }//Fine for Semaforo
	  }//Fine if Flag == 6
  }//Fine for Veicolo 
}//Fine for n == 0  

  
//////////////////////Inizio percorso con semafori////////
	
for(n = 0; n < m_pMap->GetSemafori().size();++n)  
{
	bool Intersezione = true;
		std::vector<Veicolo*>::const_iterator curVehicle = m_Vehicles.begin();
  for (curVehicle; curVehicle != m_Vehicles.end(); ++curVehicle)
  {

//Se il semaforo è verso destra (flag nel file esterno == 7)...
	if(m_pMap->Flag(n) == 7)
	{  
 std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
	 for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
	 {

//...assegna a Flag le coordinate del primo semaforo tra quelli con flag == 7

		Vector2D flag; 
	    flag =m_pMap->GetPrimoSemaforo()[n];

//Se il veicolo è vicino al semaforo e il semaforo è rosso....

if((*curVehicle)->isAtPositionProx((*curSemaforo)->Dx(Intersezione)) 
   &&  (*curSemaforo)->isRedOn())
  {
  //.....e se la posizione del semaforo con flag == 7 è uguale a quella
  //del semaforo corrente, nei pressi del quale si trova il veicolo...
		if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  {
 
	{ 
	//..allora il veicolo segua il ramo dx del semaforo
Vector2D Scambio;
Scambio = (*curSemaforo)->ScambioDx();
			  delete m_pPath;
      m_pPath = new PathNormale(Scambio.x, Scambio.y, true); 
      m_bShowPath = true; 
      (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
      (*curVehicle)->Steering()->FollowPathOn();

	}
	 }
	 }
//Se il veicolo è vicino al semaforo e il semaforo è verde....
		if((*curVehicle)->isAtPositionProx((*curSemaforo)->Dx(Intersezione)) 
   &&  (*curSemaforo)->isGreenOn())
  
  //..e se la posizione del semaforo con flag == 7 è uguale a quella
  //del semaforo corrente, nei pressi del quale si trova il veicolo...
 { 
		if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  { 
	{ 
	//..allora il veicolo segua il binario dritto del semaforo
Vector2D Scambio;
Scambio = (*curSemaforo)->ScambioDx(Intersezione);
			  delete m_pPath;
      m_pPath = new PathNormale(Scambio.x, Scambio.y, true); 
      m_bShowPath = true; 
      (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
      (*curVehicle)->Steering()->FollowPathOn();
	}
	}
	}	
	 }	 
	} 
	////////////////////////////////////////////////////////////////////////////////////
//Se il semaforo è verso sinistra (flag nel file esterno == 6)...
	if(m_pMap->Flag(n) == 6)
	{  
 std::vector<Semaforo*>::const_iterator curSemaforo = m_pMap->GetSemafori().begin();
	 for (curSemaforo; curSemaforo != m_pMap->GetSemafori().end(); ++curSemaforo)
	 {

//...assegna a Flag le coordinate del primo semaforo tra quelli con flag == 6

		Vector2D flag; 
	    flag =m_pMap->GetPrimoSemaforo()[n];

//Se il veicolo è vicino al semaforo e il semaforo è rosso....

if((*curVehicle)->isAtPositionProx((*curSemaforo)->Sx(Intersezione)) 
   &&  (*curSemaforo)->isRedOn())
  {
  //.....e se la posizione del semaforo con flag == 6 è uguale a quella
  //del semaforo corrente, nei pressi del quale si trova il veicolo...

		if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  { 

	//..allora il veicolo segua il ramo dritto del semaforo
Vector2D Scambio;
Scambio = (*curSemaforo)->ScambioSx(Intersezione);
			  delete m_pPath;
      m_pPath = new PathNormale(Scambio.x, Scambio.y, true); 
      m_bShowPath = true; 
      (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
      (*curVehicle)->Steering()->FollowPathOn();
	}
	
	 }

//Se il veicolo è vicino al semaforo e il semaforo è verde....
		if((*curVehicle)->isAtPositionProx((*curSemaforo)->Sx(Intersezione)) 
   &&  (*curSemaforo)->isGreenOn())
  
  //..e se la posizione del semaforo con flag == 6 è uguale a quella
  //del semaforo corrente, nei pressi del quale si trova il veicolo...
	{ 
		if(m_pMap->GetPrimoSemaforo()[n] == (*curSemaforo)->PosizioneSem())
  { 

	//..allora il veicolo segua il binario sx del semaforo
Vector2D Scambio;
Scambio = (*curSemaforo)->ScambioSx();
			  delete m_pPath;
      m_pPath = new PathNormale(Scambio.x, Scambio.y, true); 
      m_bShowPath = true; 
      (*curVehicle)->Steering()->SetPath(m_pPath->GetPathNormale());
      (*curVehicle)->Steering()->FollowPathOn();

	}
		}	
	 }	 
	} 

  }
 }  
 }
 //....altrimenti, se le vite sono inferiori a 3, il treno cerca il percorso
 //più breve per giungere all'item salute
 else    { //Qui va tutta l'I.A.
	 //**********TEST*********
		 m_Vehicles[a]->Steering()->WanderOn();
	   }
 }
 }
//--------------------------- AggiungiProiettileLeggero -----------------------------------------
//-----------------------------------------------------------------------------
void MondoDiGioco::AggiungiProiettileLeggero(Veicolo* shooter, Vector2D target)
{
  Proiettile* rp = new ProiettileLeggero(shooter, target);

  m_Projectiles.push_back(rp);
  
  #ifdef LOG_CREATIONAL_STUFF
  debug_con << "Aggiungo un proiettile leggero con ID " << rp->ID() << " in posizione " << rp->Pos() << "";
  #endif
}

//------------------------- AddRailGunSlug -----------------------------------
void MondoDiGioco::AddRailGunSlug(Veicolo* shooter, Vector2D target)
{
  Proiettile* rp = new Slug(shooter, target);

  m_Projectiles.push_back(rp);
  
  #ifdef LOG_CREATIONAL_STUFF
  debug_con << "Adding a rail gun slug" << rp->ID() << " at pos " << rp->Pos() << "";
#endif
}

//------------------------- AddShotGunPellet -----------------------------------
void MondoDiGioco::AddShotGunPellet(Veicolo* shooter, Vector2D target)
{
  Proiettile* rp = new Pellet(shooter, target);

  m_Projectiles.push_back(rp);
  
  #ifdef LOG_CREATIONAL_STUFF
  debug_con << "Adding a shotgun shell " << rp->ID() << " at pos " << rp->Pos() << "";
#endif
}

//-------------------------------- LoadMap ------------------------------------
//
//  Inizializza il rendering del mondo di gioco
//-----------------------------------------------------------------------------
bool MondoDiGioco::LoadMap(const std::string& filename)
{  

  //cancella ogni veicolo e proiettile corrente
    Clear();

  //Via la vecchia mappa
  delete m_pMap;
  delete m_pGraveMarkers;
  delete m_pPathManager;

  //Sotto con la nuova
  m_pGraveMarkers = new SpotIncidente(1.5);
  m_pPathManager = new PathManager<Raven_PathPlanner>(1000);
  m_pMap = new Raven_Map();
  vite = 4;

  //Resettiamo l'Entity Manager
  EntityMgr->Reset();


  //Carica i nuovi dati della mappa
  if (m_pMap->LoadMap(filename))
  { 
   AddVeicoli(Prm.NumVeicoli);
      return true;
  }

  return false;
}

//-------------------------- AddVeicoli --------------------------------------
//
//  Carica un veicolo con il suo sistema di movimento di default
//-----------------------------------------------------------------------------
void MondoDiGioco::AddVeicoli(unsigned int NumVeicoliToAdd)
{ 
  while (NumVeicoliToAdd--)
  {
	//Posizione di partenza
    Vector2D SpawnPos;

    //Crea il Veicolo
    Veicolo* vt = new Veicolo(this,
                                    SpawnPos,                 //posizione iniziale
                                    TwoPi,//RandFloat()*TwoPi,//rotazione di partenza
                                    Vector2D(0,0),            //velocità
                                    Prm.MassaVeicolo,         //massa
                                    Prm.MaxSteeringForce,     //forza massima
                                    Prm.VelocitaMassima ,     //velocità massima
                                    Prm.MaxTurnRatePerSecond, //massimo turn rate
                                    Prm.ScalaVeicolo);        //scala

    //sistema di movimento
    vt->Steering()->FlockingOff();
    vt->Steering()->WanderOff();
    vt->SetScale(Vector2D(10, 10));
    vt->SetMaxSpeed(85);

    m_Vehicles.push_back(vt);

    //Registrazione veicolo nell'Entity Manager,database utilizzato,ad esempio, da
	//MessageDispatcher per inviare tutti i messaggi alle entità del mondo di gioco
    EntityMgr->RegisterEntity(vt);

    
#ifdef LOG_CREATIONAL_STUFF
  debug_con << "Aggiungo Veicolo (id = " << ttos(vt->ID()) << ")" << "";
#endif
  }
}

//---------------------------- isLOSOkay --------------------------------------
//
//  returns true if the ray between A and B is unobstructed.
//------------------------------------------------------------------------------
bool MondoDiGioco::isLOSOkay(Vector2D A, Vector2D B)const
{
  return !doWallsObstructLineSegment(A, B, m_pMap->GetWalls());
}

//----------------------------- GetAllBotsInFOV ------------------------------
//
//  returns a vector of pointers to bots within the given bot's field of view
//-----------------------------------------------------------------------------
std::vector<Veicolo*>
MondoDiGioco::GetAllBotsInFOV(const Veicolo* pBot)const
{
  std::vector<Veicolo*> VisibleBots;

  std::vector<Veicolo*>::const_iterator curBot = m_Vehicles.begin();
  for (curBot; curBot != m_Vehicles.end(); ++curBot)
  {
    //make sure time is not wasted checking against the same bot or against a
    // bot that is dead or re-spawning
    if (*curBot == pBot ||  !(*curBot)->isAlive()) continue;

    //first of all test to see if this bot is within the FOV
    if (isSecondInFOVOfFirst(pBot->Pos(),
                             pBot->Facing(),
                             (*curBot)->Pos(),
                             pBot->FieldOfView()))
    {
      //cast a ray from between the bots to test visibility. If the bot is
      //visible add it to the vector
      if (!doWallsObstructLineSegment(pBot->Pos(),
                              (*curBot)->Pos(),
                              m_pMap->GetWalls()))
      {
        VisibleBots.push_back(*curBot);
      }
    }
  }

  return VisibleBots;
}

//---------------------------- isSecondVisibleToFirst -------------------------

bool
MondoDiGioco::isSecondVisibleToFirst(const Veicolo* pFirst,
                                   const Veicolo* pSecond)const
{
  //if the two bots are equal or if one of them is not alive return false
  if ( !(pFirst == pSecond) && pSecond->isAlive())
  {
    //first of all test to see if this bot is within the FOV
    if (isSecondInFOVOfFirst(pFirst->Pos(),
                             pFirst->Facing(),
                             pSecond->Pos(),
                             pFirst->FieldOfView()))
    {
      //test the line segment connecting the bot's positions against the walls.
      //If the bot is visible add it to the vector
      if (!doWallsObstructLineSegment(pFirst->Pos(),
                                      pSecond->Pos(),
                                      m_pMap->GetWalls()))
      {
        return true;
      }
    }
  }

  return false;
}

//------------------------- isPathObstructed ----------------------------------
//
//  returns true if a bot cannot move from A to B without bumping into 
//  world geometry. It achieves this by stepping from A to B in steps of
//  size BoundingRadius and testing for intersection with world geometry at
//  each point.
//-----------------------------------------------------------------------------
bool MondoDiGioco::isPathObstructed(Vector2D A,
                                  Vector2D B,
                                  double    BoundingRadius)const
{
  Vector2D ToB = Vec2DNormalize(B-A);

  Vector2D curPos = A;

  while (Vec2DDistanceSq(curPos, B) > BoundingRadius*BoundingRadius)
  {   
    //advance curPos one step
    curPos += ToB * 0.5 * BoundingRadius;
    
    //test all walls against the new position
    if (doWallsIntersectCircle(m_pMap->GetWalls(), curPos, BoundingRadius))
    {
      return true;
    }
  }

  return false;
}

//------------------------------ AddRocket --------------------------------
void MondoDiGioco::AddRocket(Veicolo* shooter, Vector2D target)
{
  Proiettile* rp = new Rocket(shooter, target);

  m_Projectiles.push_back(rp);
  
  #ifdef LOG_CREATIONAL_STUFF
  debug_con << "Adding a rocket " << rp->ID() << " at pos " << rp->Pos() << "";
  #endif
}