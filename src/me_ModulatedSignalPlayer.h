// Modulated signal player interface

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

/*
  Emits signal at preset frequency for precise amount of time.
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_ModulatedSignalPlayer
{
  void Init();
  TBool SetFrequency_Hz(TUint_4);
  void Emit_Us(TUint_4);
}

/*
  2025-09-15
  2025-12-27
*/
