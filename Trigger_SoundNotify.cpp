#include "Trigger_SoundNotify.h"
#include "Triggers/TriggerRegion.h"
#include "MondoDiGioco.h"
#include "Raven_Scriptor.h"
#include "constants.h"
#include "MessageDispatcher.h"
#include "Raven_Messages.h"

#include "misc/cgdi.h"

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------

Trigger_SoundNotify::Trigger_SoundNotify(Veicolo* source,
                 double      range):Trigger_LimitedLifetime<Veicolo>(FrameRate/4),
                                                 m_pSoundSource(source)
{
  //set position and range
  SetPos(m_pSoundSource->Pos());

  SetBRadius(range);

  //create and set this trigger's region of influence
  AddCircularTriggerRegion(Pos(), BRadius());
}


//------------------------------ Try ------------------------------------------
//
//  when triggered this trigger adds the bot that made the source of the sound 
//  to the triggering bot's perception.
//-----------------------------------------------------------------------------
void Trigger_SoundNotify::Try(Veicolo* pBot)
{
  //is this bot within range of this sound
  if (isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
  {
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            SENDER_ID_IRRELEVANT,
                            pBot->ID(),
                            Msg_RumoreArmaDaFuoco,
                            m_pSoundSource);
  }   
}




