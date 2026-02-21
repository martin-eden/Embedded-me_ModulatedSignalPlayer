// Modulated signal player implementation

/*
  Author: Martin Eden
  Last mod.: 2026-02-21
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>

#include <me_FrequencyGenerator.h>
#include <me_Delays.h>
#include <me_RunTime.h>
#include <me_Interrupts.h>

// #include <me_Console.h>
// #include <me_DebugPrints.h>

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
void me_ModulatedSignalPlayer::Emit_Us(
  TUint_4 Duration_Us
)
{
  /*
    Worst case is when we called with short (say 100 us) duration

    For short durations we disable interrupts and call microseconds
    delay. That's called "stage 2" here.

    For longer durations we set "stage 2" time mark and chill with
    some delay function and interrupts.
  */

  const TUint_4 Overhead_Us = 40;
  // const TUint_4 Overhead_Us = 43; // for debug version
  const TUint_4 NoInterruptsOffset_Us = 150;

  TUint_4 CurTimeMark_Us;
  TUint_4 NoInterruptsMark_Us;
  TUint_4 EndTimeMark_Us;

  // TUint_4 TimesRemained_Us[3] = { 0 };

  {
    me_Interrupts::TInterruptsDisabler NoInts;

    CurTimeMark_Us = me_RunTime::GetTime_Us();

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

    NoInts.Restore();
    me_Delays::Delay_Us(GetTimeRemained_Us(CurTimeMark_Us, NoInterruptsMark_Us));
    NoInts.Disable();

    CurTimeMark_Us = me_RunTime::GetTime_Us();

  SecondStage:
    /*
      Fine wait. Interrupts disabled. Micro-second precision.
    */

    // TimesRemained_Us[2] = GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us);

    me_Delays::Delay_Us(GetTimeRemained_Us(CurTimeMark_Us, EndTimeMark_Us));

    me_FrequencyGenerator::StopFreqGen();
  }

  /*
  Console.Print("Durations");
  Console.Indent();

  me_DebugPrints::PrintDuration_Us("Total", TimesRemained_Us[0]);
  Console.EndLine();

  me_DebugPrints::PrintDuration_Us("Rough delay", TimesRemained_Us[1]);
  Console.EndLine();

  me_DebugPrints::PrintDuration_Us("Fine delay", TimesRemained_Us[2]);
  Console.EndLine();

  Console.Unindent();
  Console.EndLine();
  //*/
}

/*
  2025 # # # # # #
*/
