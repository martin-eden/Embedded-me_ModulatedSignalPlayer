// [me_ModulatedSignalPlayer] test

/*
  Author: Martin Eden
  Last mod.: 2025-10-30
*/

#include <me_ModulatedSignalPlayer.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include <me_Duration.h>
#include <me_Menu.h>

static TUint_4 EmitFreq_Hz = 1000000;
static me_Duration::TDuration EmitDuration = { 0, 0, 0, 300 };

void SetFrequency_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  TUint_2 Freq_kHz;

  Console.Write("Enter frequency (kHz): ");

  if (!Console.Read(&Freq_kHz))
  {
    Console.Print("Frequency reading failed");

    return;
  }

  EmitFreq_Hz = (TUint_4) Freq_kHz * 1000;
}

void GetFrequency_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  me_DebugPrints::Print("Frequency (Hz)", EmitFreq_Hz);
  Console.EndLine();
}

void SetDuration_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  TUint_2 Duration_Us;
  TUint_2 Duration_Ms;

  Console.Write("Enter duration (in microseconds): ");

  if (!Console.Read(&Duration_Us))
  {
    Console.Print("Failed to parse");

    return;
  }

  Duration_Ms = Duration_Us / 1000;

  Duration_Us = Duration_Us % 1000;

  EmitDuration.KiloS = 0;
  EmitDuration.S = 0;
  EmitDuration.MilliS = Duration_Ms;
  EmitDuration.MicroS = Duration_Us;
}

void GetDuration_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  Console.Write("Duration");
  me_DebugPrints::PrintDuration(EmitDuration);
  Console.EndLine();
}

void Emit_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  if (!me_ModulatedSignalPlayer::SetFrequency_Hz(EmitFreq_Hz))
  {
    Console.Print("Setting frequency failed");

    return;
  }

  me_ModulatedSignalPlayer::Emit(EmitDuration);
}

void AddMenuItems(
  me_Menu::TMenu * Menu
)
{
  using
    me_Menu::Freetown::ToItem;

  const TAddress Unused = 0;

  Menu->AddItem(ToItem("sf", "Set frequency (Hz)", SetFrequency_Handler, Unused));
  Menu->AddItem(ToItem("gf", "Get frequency", GetFrequency_Handler, Unused));
  Menu->AddItem(ToItem("sd", "Set duration (in microseconds)", SetDuration_Handler, Unused));
  Menu->AddItem(ToItem("gd", "Get duration", GetDuration_Handler, Unused));
  Menu->AddItem(ToItem("e", "Emit frequency for given duration", Emit_Handler, Unused));
}


void setup()
{
  me_Menu::TMenu Menu;

  Console.Init();
  me_ModulatedSignalPlayer::Init();

  AddMenuItems(&Menu);
  Menu.AddBuiltinCommands();

  Menu.Print();

  Menu.Run();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2025-09-15
  2025-09-19
  2025-10-26
*/
