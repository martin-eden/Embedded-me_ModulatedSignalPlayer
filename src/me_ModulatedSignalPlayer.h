// Modulated signal player interface

/*
  Author: Martin Eden
  Last mod.: 2025-10-26
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Duration.h>

namespace me_ModulatedSignalPlayer
{
  void Init();
  TBool SetFrequency_Hz(TUint_4 EmitFreq_Hz);
  void Emit(me_Duration::TDuration Duration);
}

/*
  2025-09-15
*/
