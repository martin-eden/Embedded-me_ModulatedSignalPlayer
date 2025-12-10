// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2025-12-10
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>

#include <me_Duration.h>
#include <me_FrequencyGenerator.h>
#include <me_Delays.h>
#include <me_RunTime.h>

#include <avr/common.h>
#include <avr/interrupt.h>

#include <me_Console.h>
#include <me_DebugPrints.h>

using namespace me_ModulatedSignalPlayer;

void me_ModulatedSignalPlayer::Init()
{
  me_RunTime::Init();
  me_RunTime::Start();
}

TBool me_ModulatedSignalPlayer::SetFrequency_Hz(
  TUint_4 EmitFreq_Hz
)
{
  return me_FrequencyGenerator::SetFrequency_Hz(EmitFreq_Hz);
}

static TUint_4 CappedSub(
  TUint_4 A,
  TUint_4 B
)
{
  if (A < B)
    return 0;

  return A - B;
}

static TUint_4 GetCurTime_Us()
{
  TUint_4 Result;

  me_Duration::DurationToMicros(&Result, me_RunTime::GetTime());

  return Result;
}

static TUint_4 GetTimeRemained_Us(
  TUint_4 CurTimeMark_Us,
  TUint_4 EndTimeMark_Us
)
{
  return CappedSub(EndTimeMark_Us, CurTimeMark_Us);
}

/*
  Emit frequency for given duration

  This code is tuned to compensate overhead of function calls.
*/
void me_ModulatedSignalPlayer::Emit(
  me_Duration::TDuration Duration
)
{
  /*
    Worst case is when we called with short (say 100 us) duration

    For short durations we disable interrupts and call microseconds
    delay. That's called "stage 2" here.

    For longer durations we set "stage 2" time mark and chill with
    some delay function and interrupts.
  */

  const TUint_4 Overhead_Us = 47;
  // const TUint_4 Overhead_Us = 51; // for debug version
  const TUint_4 NoInterruptsOffset_Us = 1000;

  TUint_4 Duration_Us;
  TUint_4 CurTimeMark_Us;
  TUint_4 NoInterruptsMark_Us;
  TUint_4 EndTimeMark_Us;

  // TUint_4 TimesRemained_Us[3] = { 0 };

  TUint_1 OrigSreg;

  me_Duration::DurationToMicros(&Duration_Us, Duration);

  OrigSreg = SREG;

  cli();

  CurTimeMark_Us = GetCurTime_Us();

  me_FrequencyGenerator::StartFreqGen();

  EndTimeMark_Us = CurTimeMark_Us + Duration_Us - Overhead_Us;

  NoInterruptsMark_Us = CappedSub(EndTimeMark_Us, NoInterruptsOffset_Us);

  // TimesRemained_Us[0] = GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us);

  if (CurTimeMark_Us >= NoInterruptsMark_Us)
    goto SecondStage;

// FirstStage:
  /*
    Rough wait. Interrupts enabled.
  */

  // TimesRemained_Us[1] = GetTimeRemained_Us(CurTimeMark_Us, NoInterruptsMark_Us);

  SREG = OrigSreg;
  me_Duration::MicrosToDuration(
    &Duration,
    GetTimeRemained_Us(CurTimeMark_Us, NoInterruptsMark_Us)
  );
  me_Delays::Delay_Duration(Duration);
  cli();

  CurTimeMark_Us = GetCurTime_Us();

SecondStage:
  /*
    Fine wait. Interrupts disabled. Micro-second precision.
  */

  // TimesRemained_Us[2] = GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us);

  me_Delays::Delay_Us(GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us));

  me_FrequencyGenerator::StopFreqGen();

  SREG = OrigSreg;

  /*
  Console.Print("Durations");
  Console.Indent();

  Console.Write("Total");
  me_Duration::MicrosToDuration(&Duration, TimesRemained_Us[0]);
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  Console.Write("Rough delay");
  me_Duration::MicrosToDuration(&Duration, TimesRemained_Us[1]);
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  Console.Write("Fine delay");
  me_Duration::MicrosToDuration(&Duration, TimesRemained_Us[2]);
  me_DebugPrints::PrintDuration(Duration);
  Console.Print(TimesRemained_Us[2]);
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
  2025-11-27
*/
