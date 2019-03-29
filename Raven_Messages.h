#ifndef RAVEN_MESSAGES_H
#define RAVEN_MESSAGES_H

#include <string>

enum message_type
{
  Msg_Blank,
  Msg_PathReady,
  Msg_NoPathAvailable,
  Msg_Colpito, 
  Msg_Ucciso,
  Msg_GoalQueueEmpty,
  Msg_OpenSesame,
  Msg_RumoreArmaDaFuoco,
  Msg_UserHasRemovedBot
};

//used for outputting debug info
inline std::string MessageToString(int msg)
{
  switch(msg)
  {
  case Msg_PathReady:

    return "Msg_PathReady";

  case Msg_NoPathAvailable:

    return "Msg_NoPathAvailable";

  case Msg_Colpito:

    return "Msg_Colpito";

  case Msg_Ucciso:

    return "Msg_Ucciso";

  case Msg_GoalQueueEmpty:

    return "Msg_GoalQueueEmpty";

  case Msg_OpenSesame:

    return "Msg_OpenSesame";

  case Msg_RumoreArmaDaFuoco:

    return "Msg_RumoreArmaDaFuoco";

  case Msg_UserHasRemovedBot:

    return "Msg_UserHasRemovedBot";

  default:

    return "Undefined message!";
  }
}


#endif

  