// [me_ModulatedSignalPlayer] test

/*
  Author: Martin Eden
  Last mod.: 2025-09-20
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include <me_Duration.h>

void RunTest()
{
  const TUint_4 EmitFreq_Hz = 25000;
  const me_Duration::TDuration Duration = { 0, 0, 0, 500 };

  Console.Print("--");
  Console.Print("Test of emitting PWM for some time");
  Console.Print("");
  Console.Write("Frequency (Hz)");
  Console.Print(EmitFreq_Hz);
  Console.EndLine();
  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  Console.Print("(");

  if (!me_ModulatedSignalPlayer::SetFrequency_Hz(EmitFreq_Hz))
    Console.Print("Setting frequency failed");
  me_ModulatedSignalPlayer::Emit(Duration);

  Console.Print(")");
}

void setup()
{
  Console.Init();

  Console.Print("( [me_ModulatedSignalPlayer] test");
  Console.Indent();
  RunTest();
  Console.Unindent();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2025-09-15
  2025-09-19
*/
