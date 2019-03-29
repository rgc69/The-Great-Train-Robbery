#include "ExploreGoal_Evaluator.h"
#include "Raven_PathPlanner.h"
#include "EnumerazioneOggetti.h"
//#include "../lua/Raven_Scriptor.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"

#include "Goal_Think.h"
#include "Raven_Goal_Types.h"




//---------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double ExploreGoal_Evaluator::CalculateDesirability(Veicolo* pBot)
{
  double Desirability = 0.05;

  Desirability *= m_dCharacterBias;

  return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void ExploreGoal_Evaluator::SetGoal(Veicolo* pBot)
{
  pBot->GetBrain()->AddGoal_Explore();
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void ExploreGoal_Evaluator::RenderInfo(Vector2D Position, Veicolo* pBot)
{
  gdi->TextAtPos(Position, "EX: " + ttos(CalculateDesirability(pBot), 2));
}
