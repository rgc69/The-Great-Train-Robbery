#ifndef TRIGGER_SOUNDNOTIFY_H
#define TRIGGER_SOUNDNOTIFY_H
#pragma warning (disable:4786)

//  Desc:     whenever an agent makes a sound -- such as when a weapon fires --
//            this trigger can be used to notify other bots of the event.
//
//            This type of trigger has a circular trigger region and a lifetime
//            of 1 update-step
//
//-----------------------------------------------------------------------------
#include "Trigger_LimitedLifetime.h"
#include "Veicolo.h"
#include "MessageDispatcher.h"
#include "Raven_Messages.h"

#include "misc/cgdi.h"



class Trigger_SoundNotify : public Trigger_LimitedLifetime<Veicolo>
{
private:

  //a pointer to the bot that has made the sound
 Veicolo*  m_pSoundSource;

public:

  Trigger_SoundNotify(Veicolo* source, double range);


  void  Try(Veicolo*);

  void  Render(){}

};





#endif