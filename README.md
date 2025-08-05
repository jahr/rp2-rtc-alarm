# rp2-rtc-alarm
>Simple micropython wrapper to RP Pico hardware RTC Alarm


This module gives access to Pico SDK's RTC Alarm functions. It is tested only on RP2040.
it's tested on Pico W with MicroPython v1.26.0-preview.524 with Pico-SDK 2.1.1

>Build:

go to ports/rp2 directory and run

```
make BOARD=RPI_PICO_W USER_C_MODULES=../../../rp2-rtc-alarm/rp2_rtc_alarm.cmake
```

(of course adjust the BOARD and rp2-rtc-alarm directory location)

In the micropython the module rtcalarm is available and has following functions:

```
set_alarm() - 
enable_alarm(datetime, callback) - starts alarm. datetime is tuple with 8 integers (the last one is ignored), callback is function with no arguments
disable_alarm() - stops alarm
```

datetime tuple contains `(year, month, day, dayofweek, hour, minute, second, ignored)` telling when the alarm will be fired and any of them may be replaced with -1, which means that it is not checked. Eg. alarm firing every minute at 0 sec. should be set with `(-1, -1, -1, -1, -1, -1, 0, 0)`.

>Simple example code:

```
import micropython as mp
from machine import RTC
import rtcalarm as ra

rtc = RTC()

def show_time(_):
  _, _, _, _, hour, minute, second, _ = rtc.datetime()
  print('{:2d}:{:02d}:{:02d}'.format(hour, minute, second))

def rtcalarm_callback():
  mp.schedule(show_time, None)

ra.enable_alarm((-1, -1, -1, -1, -1, -1, 0, 0), rtcalarm_callback)
```

