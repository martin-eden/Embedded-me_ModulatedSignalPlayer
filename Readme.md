## What

(2025-09)

Emit modulated (PWM) signal for precise amount of time. ATmega328/P


## Details

Interface is quite bare for now and exports just two functions:
one to set frequency and other to play "note" for given time *Emit()*.

But that Emit() is tuned with oscilloscope to produce wave for
almost exact amount of time.

It's used for replaying infrared signals where typical span
is like 350 micro-seconds.


## Code

* [Interface][Interface]
* [Implementation][Implementation]
* [Example][Example]


## Requirements

  * arduino-cli
  * bash


## Install/remove

Easy way is to clone [GetLibs][GetLibs] repo and run it's code.


## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]


[Interface]: src/me_ModulatedSignalPlayer.h
[Implementation]: src/me_ModulatedSignalPlayer.cpp
[Example]: examples/me_ModulatedSignalPlayer/me_ModulatedSignalPlayer.ino

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
