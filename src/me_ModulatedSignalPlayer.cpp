// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-26
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>
#include <me_Duration.h>

#include <me_FrequencyGenerator.h>
#include <me_Delays.h>

using namespace me_ModulatedSignalPlayer;

void me_ModulatedSignalPlayer::Init()
{
  me_Delays::Init();
}

TBool me_ModulatedSignalPlayer::SetFrequency_Hz(
  TUint_4 EmitFreq_Hz
)
{
  return me_FrequencyGenerator::SetFrequency_Hz(EmitFreq_Hz);
}

/*
  Emit frequency for given duration

  This code is tuned to compensate overhead of function calls.
*/
void me_ModulatedSignalPlayer::Emit(
  me_Duration::TDuration Duration
)
{
  const me_Duration::TDuration EmitOverhead = { 0, 0, 0, 110 };

  if (!me_Duration::Subtract(&Duration, EmitOverhead))
    Duration = me_Duration::Zero;

  me_FrequencyGenerator::StartFreqGen();
  me_Delays::Delay_Duration(Duration);
  me_FrequencyGenerator::StopFreqGen();
}

/*
  2025-09-15
*/
