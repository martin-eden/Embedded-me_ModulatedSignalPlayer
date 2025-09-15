// [me_ModulatedSignalPlayer] test

/*
  Author: Martin Eden
  Last mod.: 2025-09-15
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>

void RunTest()
{
  const TUint_4 EmitFreq_Hz = 25000;
  const me_Timestamp::TTimestamp Duration = { 0, 0, 0, 500 };

  Console.Print("Emitting pulse at frequency:");
  Console.Write("Frequency (Hz)");
  Console.Print(EmitFreq_Hz);
  Console.EndLine();
  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  if (!me_ModulatedSignalPlayer::SetFrequency_Hz(EmitFreq_Hz))
    Console.Print("Setting frequency failed");
  me_ModulatedSignalPlayer::Emit(Duration);

  Console.Print("Done");
}

void setup()
{
  Console.Init();

  Console.Print("( [me_ModulatedSignalPlayer] test");
  RunTest();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2025-09-15
*/
