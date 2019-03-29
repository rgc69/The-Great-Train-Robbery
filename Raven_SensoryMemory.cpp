#include "Raven_SensoryMemory.h"
#include "MondoDiGioco.h"
#include "crudetimer.h"
#include "misc/cgdi.h"
#include "misc/Stream_Utility_Functions.h"

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
 Raven_SensoryMemory:: Raven_SensoryMemory(Veicolo* owner,
                                           double MemorySpan):m_pOwner(owner),
                                                            m_dMemorySpan(MemorySpan)
                                                          
{}

//--------------------- MakeNewRecordIfNotAlreadyPresent ----------------------

void Raven_SensoryMemory::MakeNewRecordIfNotAlreadyPresent(Veicolo* pOpponent)
{
  //else check to see if this Opponent already exists in the memory. If it doesn't,
  //create a new record
  if (m_MemoryMap.find(pOpponent) == m_MemoryMap.end())
  {
    m_MemoryMap[pOpponent] = MemoryRecord();
  }
}

//------------------------ RemoveBotFromMemory --------------------------------
//
//  this removes a bot's record from memory
//-----------------------------------------------------------------------------
void Raven_SensoryMemory::RemoveBotFromMemory(Veicolo* pVeicolo)
{
  MemoryMap::iterator record = m_MemoryMap.find(pVeicolo);
  
  if (record != m_MemoryMap.end())
  {
    m_MemoryMap.erase(record);
  }
}
  
//----------------------- UpdateWithSoundSource -------------------------------
//
// this updates the record for an individual opponent. Note, there is no need to
// test if the opponent is within the FOV because that test will be done when the
// UpdateVision method is called
//-----------------------------------------------------------------------------
void Raven_SensoryMemory::UpdateWithSoundSource(Veicolo* pNoiseMaker)
{
  //make sure the bot being examined is not this bot
  if (m_pOwner != pNoiseMaker)
  {
    //if the bot is already part of the memory then update its data, else
    //create a new memory record and add it to the memory
    MakeNewRecordIfNotAlreadyPresent(pNoiseMaker);

    MemoryRecord& info = m_MemoryMap[pNoiseMaker];

    //test if there is LOS between bots 
    if (m_pOwner->GetWorld()->isLOSOkay(m_pOwner->Pos(), pNoiseMaker->Pos()))
    {
      info.bShootable = true;
      
     //record the position of the bot
      info.vLastSensedPosition = pNoiseMaker->Pos();
   }
    else
    {
      info.bShootable = false;
    }
    
    //record the time it was sensed
    info.fTimeLastSensed = (double)Clock->GetCurrentTime();
  }
}

//----------------------------- UpdateVision ----------------------------------
//
//  this method iterates through all the bots in the game world to test if
//  they are in the field of view. Each bot's memory record is updated
//  accordingly
//-----------------------------------------------------------------------------
void Raven_SensoryMemory::UpdateVision()
{
  //for each bot in the world test to see if it is visible to the owner of
  //this class
  const std::vector<Veicolo*>& Veicoli = m_pOwner->GetWorld()->GetAllVeicoli();
  std::vector<Veicolo*>::const_iterator curVeicolo;
  for (curVeicolo = Veicoli.begin(); curVeicolo!=Veicoli.end(); ++curVeicolo)
  {
    //make sure the bot being examined is not this bot
    if (m_pOwner != *curVeicolo)
    {
      //make sure it is part of the memory map
      MakeNewRecordIfNotAlreadyPresent(*curVeicolo);

      //get a reference to this bot's data
      MemoryRecord& info = m_MemoryMap[*curVeicolo];

      //test if there is LOS between bots 
      if (m_pOwner->GetWorld()->isLOSOkay(m_pOwner->Pos(), (*curVeicolo)->Pos()))
      {
        info.bShootable = true;

              //test if the bot is within FOV
        if (isSecondInFOVOfFirst(m_pOwner->Pos(),
                                 m_pOwner->Facing(),
                                 (*curVeicolo)->Pos(),
                                  m_pOwner->FieldOfView()))
        {
          info.fTimeLastSensed     = Clock->GetCurrentTime();
          info.vLastSensedPosition = (*curVeicolo)->Pos();
          info.fTimeLastVisible    = Clock->GetCurrentTime();

          if (info.bWithinFOV == false)
          {
            info.bWithinFOV           = true;
            info.fTimeBecameVisible    = info.fTimeLastSensed;
          
          }
        }

        else
        {
          info.bWithinFOV = false;         
        }
      }

      else
      {
        info.bShootable = false;
        info.bWithinFOV = false;
      }
    }
  }//next bot
}


//------------------------ GetListOfRecentlySensedOpponents -------------------
//
//  ritorna un vettore di tutti i nemici "avvertiti" di recente
//-----------------------------------------------------------------------------
std::vector<Veicolo*> 
Raven_SensoryMemory::GetListOfRecentlySensedOpponents()const
{
  //questo vettore immagazzina tutti i nemici di cui si può avere memoria
  std::vector<Veicolo*> opponents;

  double CurrentTime = Clock->GetCurrentTime();

  MemoryMap::const_iterator curRecord = m_MemoryMap.begin();
  for (curRecord; curRecord!=m_MemoryMap.end(); ++curRecord)
  {
 //se questo nemico è stato inserito nella memoria di recente,aggiungiamolo alla lista
    if ( (CurrentTime - curRecord->second.fTimeLastSensed) <= m_dMemorySpan)
    {
      opponents.push_back(curRecord->first);
    }
  }

  return opponents;
}

//----------------------------- isOpponentShootable --------------------------------
//
//  returns true if the bot given as a parameter can be shot (ie. its not
//  obscured by walls)
//-----------------------------------------------------------------------------
bool Raven_SensoryMemory::isOpponentShootable(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end())
  {
    return it->second.bShootable;
  }

  return false;
}

//----------------------------- isOpponentWithinFOV --------------------------------
//
//  returns true if the bot given as a parameter is within FOV
//-----------------------------------------------------------------------------
bool  Raven_SensoryMemory::isOpponentWithinFOV(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end())
  {
    return it->second.bWithinFOV;
  }

  return false;
}

//---------------------------- GetLastRecordedPositionOfOpponent -------------------
//
//  returns the last recorded position of the bot
//-----------------------------------------------------------------------------
Vector2D  Raven_SensoryMemory::GetLastRecordedPositionOfOpponent(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end())
  {
    return it->second.vLastSensedPosition;
  }

  throw std::runtime_error("< Raven_SensoryMemory::GetLastRecordedPositionOfOpponent>: Attempting to get position of unrecorded bot");
}

//----------------------------- GetTimeOpponentHasBeenVisible ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
double  Raven_SensoryMemory::GetTimeOpponentHasBeenVisible(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end() && it->second.bWithinFOV)
  {
    return Clock->GetCurrentTime() - it->second.fTimeBecameVisible;
  }

  return 0;
}

//-------------------- GetTimeOpponentHasBeenOutOfView ------------------------
//
//  returns the amount of time the given opponent has remained out of view
//  returns a high value if opponent has never been seen or not present
//-----------------------------------------------------------------------------
double Raven_SensoryMemory::GetTimeOpponentHasBeenOutOfView(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end())
  {
    return Clock->GetCurrentTime() - it->second.fTimeLastVisible;
  }

  return MaxDouble;
}

//------------------------ GetTimeSinceLastSensed ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
double  Raven_SensoryMemory::GetTimeSinceLastSensed(Veicolo* pOpponent)const
{
  MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);
 
  if (it != m_MemoryMap.end() && it->second.bWithinFOV)
  {
    return Clock->GetCurrentTime() - it->second.fTimeLastSensed;
  }

  return 0;
}

//---------------------- RenderBoxesAroundRecentlySensed ----------------------
//
//  renderizza un box intorno ai nemici "avvertiti" di recente.
//-----------------------------------------------------------------------------
void  Raven_SensoryMemory::RenderBoxesAroundRecentlySensed()const
{
  std::vector<Veicolo*> opponents = GetListOfRecentlySensedOpponents();
  std::vector<Veicolo*>::const_iterator it;
  for (it = opponents.begin(); it != opponents.end(); ++it)
  {
    gdi->GreenPen();
    Vector2D p = (*it)->Pos();
    double   b = 25;//(*it)->BRadius();
      
    gdi->Line(p.x-b, p.y-30, p.x+b, p.y-30);
    gdi->Line(p.x+b, p.y-30, p.x+b, p.y+20);
    gdi->Line(p.x+b, p.y+20, p.x-b, p.y+20);
    gdi->Line(p.x-b, p.y+20, p.x-b, p.y-30);
  }

}
