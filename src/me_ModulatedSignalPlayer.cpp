// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-27
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>

#include <me_Duration.h>
#include <me_FrequencyGenerator.h>
#include <me_Delays.h>
#include <me_RunTime.h>

#include <avr/common.h>
#include <avr/interrupt.h>

// #include <me_Console.h>
// #include <me_DebugPrints.h>

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
  const me_Duration::TDuration LastSnatch = { 0, 0, 0, 280 };
  const TUint_2 LastSnatchOverhead_Us = 26;

  me_Duration::TDuration StartTime;
  me_Duration::TDuration CurTime;
  me_Duration::TDuration EndTime;
  me_Duration::TDuration TimeRemained;
  TUint_2 LastSnatch_Us;

  TUint_1 OrigSreg;

  StartTime = me_RunTime::GetTime_Precise();

  EndTime = StartTime;
  if (!me_Duration::Add(&EndTime, Duration))
    return;

  if (!me_Duration::Subtract(&Duration, LastSnatch))
    Duration = me_Duration::Zero;

  me_FrequencyGenerator::StartFreqGen();

  me_Delays::Delay_Duration(Duration);

  OrigSreg = SREG;
  cli();

  CurTime = me_RunTime::GetTime_Precise();

  TimeRemained = EndTime;
  if (!me_Duration::Subtract(&TimeRemained, CurTime))
    TimeRemained = me_Duration::Zero;
  // Expectation: <TimeRemained> is between (<LastSnatch>, 1 ms)

  if (TimeRemained.MicroS > LastSnatchOverhead_Us)
    LastSnatch_Us = TimeRemained.MicroS - LastSnatchOverhead_Us;
  else
    LastSnatch_Us = 0;

  me_Delays::Delay_Us(LastSnatch_Us);

  me_FrequencyGenerator::StopFreqGen();

  SREG = OrigSreg;

  /*
  Console.Write("StartTime");
  me_DebugPrints::PrintDuration(StartTime);
  Console.EndLine();

  Console.Write("CurTime");
  me_DebugPrints::PrintDuration(CurTime);
  Console.EndLine();

  Console.Write("EndTime");
  me_DebugPrints::PrintDuration(EndTime);
  Console.EndLine();

  Console.Write("TimeRemained");
  me_DebugPrints::PrintDuration(TimeRemained);
  Console.EndLine();

  me_DebugPrints::Print("Last snatch (us)", LastSnatch_Us);
  Console.EndLine();

  Console.EndLine();
  */
}

/*
  2025-09-15
  2025-10-27
*/
