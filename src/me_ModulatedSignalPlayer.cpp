// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-29
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

[[gnu::noinline]] static void StopEmittingAfter_Us(
  TUint_2 NumMicros
)
{
  const TUint_2 Overhead_Us = 44;

  if (NumMicros > Overhead_Us)
    NumMicros = NumMicros - Overhead_Us;
  else
    NumMicros = 0;

  me_Delays::Delay_Us(NumMicros);

  me_FrequencyGenerator::StopFreqGen();
}

static TUint_4 DurationToMicros(
  me_Duration::TDuration Duration
)
{
  return
    (TUint_4) 1000000 * Duration.S +
    (TUint_4) 1000 * Duration.MilliS +
    (TUint_4) Duration.MicroS;
}

static me_Duration::TDuration MicrosToDuration(
  TUint_4 NumMicros
)
{
  me_Duration::TDuration Result;

  Result.KiloS = 0;
  Result.S = NumMicros / 1000000;
  Result.MilliS = NumMicros / 1000 % 1000;
  Result.MicroS = NumMicros % 1000;

  return Result;
}

/*
  Emit frequency for given duration

  This code is tuned to compensate overhead of function calls.
*/
void me_ModulatedSignalPlayer::Emit(
  me_Duration::TDuration Duration
)
{
  const TUint_4 TimeReserve_Us = 800;
  const TUint_2 MainDelayStep_Us = 200;
  const TUint_4 FinalCorrection_Us = 2;

  TUint_4 Duration_Us;
  TUint_4 CurTime_Us;
  TUint_4 EndTime_Us;
  TUint_4 TimeRemained_Us;

  TUint_1 OrigSreg = SREG;

  Duration_Us = DurationToMicros(Duration);

  cli();

  CurTime_Us = DurationToMicros(me_RunTime::GetTime_Precise());

  me_FrequencyGenerator::StartFreqGen();

  EndTime_Us = CurTime_Us + Duration_Us;

  if (Duration_Us > TimeReserve_Us)
  {
    SREG = OrigSreg;

    while (true)
    {
      CurTime_Us = DurationToMicros(me_RunTime::GetTime_Precise());

      if (CurTime_Us < EndTime_Us)
        TimeRemained_Us = EndTime_Us - CurTime_Us;
      else
        TimeRemained_Us = 0;

      if (TimeRemained_Us <= TimeReserve_Us)
        break;

      me_Delays::Delay_Us(MainDelayStep_Us);
    }

    cli();
  }

  CurTime_Us = DurationToMicros(me_RunTime::GetTime_Precise());

  if (CurTime_Us < EndTime_Us)
    TimeRemained_Us = EndTime_Us - CurTime_Us;
  else
    TimeRemained_Us = 0;

  if (TimeRemained_Us > FinalCorrection_Us)
    TimeRemained_Us = TimeRemained_Us - FinalCorrection_Us;
  else
    TimeRemained_Us = 0;

  StopEmittingAfter_Us(TimeRemained_Us);

  SREG = OrigSreg;

  /*
  // me_DebugPrints::Print("Duration (us)", Duration_Us);
  // Console.EndLine();
  Console.Write("Duration");
  me_DebugPrints::PrintDuration(MicrosToDuration(Duration_Us));
  Console.EndLine();

  // me_DebugPrints::Print("Current time (us)", CurTime_Us);
  // Console.EndLine();
  Console.Write("Current time");
  me_DebugPrints::PrintDuration(MicrosToDuration(CurTime_Us));
  Console.EndLine();

  // me_DebugPrints::Print("End time (us)", EndTime_Us);
  // Console.EndLine();
  Console.Write("End time");
  me_DebugPrints::PrintDuration(MicrosToDuration(EndTime_Us));
  Console.EndLine();

  // me_DebugPrints::Print("Remained time (us)", TimeRemained_Us);
  // Console.EndLine();
  Console.Write("Remained time");
  me_DebugPrints::PrintDuration(MicrosToDuration(TimeRemained_Us));
  Console.EndLine();

  Console.EndLine();
  //*/
}

/*
  2025-09-15
  2025-10-27
  2025-10-28
*/
