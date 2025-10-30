// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-30
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

TUint_4 Subtract(
  TUint_4 A,
  TUint_4 B
)
{
  if (A < B)
    return 0;

  return A - B;
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

//*
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
//*/

TUint_4 GetCurTime_Us()
{
  return DurationToMicros(me_RunTime::GetTime_Precise());
}

TUint_4 GetTimeRemained_Us(
  TUint_4 CurTimeMark_Us,
  TUint_4 EndTimeMark_Us
)
{
  return Subtract(EndTimeMark_Us, CurTimeMark_Us);
}

/*
  Emit frequency for given duration

  This code is tuned to compensate overhead of function calls.
*/
void me_ModulatedSignalPlayer::Emit(
  me_Duration::TDuration Duration
)
{
  const TUint_4 Overhead_Us = 47;
  // const TUint_4 Overhead_Us = 51; // for debug version
  const TUint_4 NoInterruptsOffset_Us = 500;
  const TUint_4 MediumStepsOffset_Us = 500 + NoInterruptsOffset_Us;

  TUint_4 CurTimeMark_Us;
  TUint_4 EndTimeMark_Us;

  TUint_4 MediumStepsMark_Us;
  TUint_4 NoInterrupsMark_Us;

  // TUint_4 TimesRemained_Us[4] = { 0 };

  TUint_1 OrigSreg;

  OrigSreg = SREG;

  cli();

  CurTimeMark_Us = GetCurTime_Us();

  me_FrequencyGenerator::StartFreqGen();

  EndTimeMark_Us = CurTimeMark_Us + (DurationToMicros(Duration) - Overhead_Us);

  NoInterrupsMark_Us = Subtract(EndTimeMark_Us, NoInterruptsOffset_Us);
  MediumStepsMark_Us = Subtract(EndTimeMark_Us, MediumStepsOffset_Us);

  // TimesRemained_Us[0] = GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us);

  if (CurTimeMark_Us >= NoInterrupsMark_Us)
    goto ThirdStage;
  else if (CurTimeMark_Us >= MediumStepsMark_Us)
    goto SecondStage;

// FirstStage:

  // TimesRemained_Us[1] = GetTimeRemained_Us(CurTimeMark_Us, MediumStepsMark_Us);

  SREG = OrigSreg;
  me_Delays::Delay_Duration(MicrosToDuration(GetTimeRemained_Us(CurTimeMark_Us, MediumStepsMark_Us)));
  cli();

  CurTimeMark_Us = GetCurTime_Us();

SecondStage:

  // TimesRemained_Us[2] = GetTimeRemained_Us(CurTimeMark_Us, NoInterrupsMark_Us);

  SREG = OrigSreg;
  me_Delays::Delay_Us(GetTimeRemained_Us(CurTimeMark_Us, NoInterrupsMark_Us));
  cli();

  CurTimeMark_Us = GetCurTime_Us();

ThirdStage:

  // TimesRemained_Us[3] = GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us);

  me_Delays::Delay_Us(GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us));

  me_FrequencyGenerator::StopFreqGen();

  SREG = OrigSreg;

  /*
  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  Console.Print("Durations");
  Console.Indent();

  Console.Write("Total");
  me_DebugPrints::PrintDuration(MicrosToDuration(TimesRemained_Us[0]));
  Console.EndLine();

  Console.Write("Rough delay");
  me_DebugPrints::PrintDuration(MicrosToDuration(TimesRemained_Us[1]));
  Console.EndLine();

  Console.Write("Medium delay");
  me_DebugPrints::PrintDuration(MicrosToDuration(TimesRemained_Us[2]));
  Console.EndLine();

  Console.Write("Fine delay");
  me_DebugPrints::PrintDuration(MicrosToDuration(TimesRemained_Us[3]));
  Console.EndLine();

  Console.Unindent();
  Console.EndLine();
  //*/
}

/*
  2025-09-15
  2025-10-27
  2025-10-28
  2025-10-29
  2025-10-30
*/
