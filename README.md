# rp2-rtc-alarm
Simple micropython wrapper to RP Pico hardware RTC Alarm


This module gives access to Pico SDK's RTC Alarm functions.

Build:

go to ports/rp2 directory and run

make BOARD=PICO_W USER_C_MODULES=../../../rp2-rtc-alarm/rp2_rtc_alarm.cmake

(of course adjust the BOARD and rp2-rtc-alarm directory location)

In the micropython the module rtcalarm is available and has following functions:

set_alarm(datetime, callback) - datetime is tuple with 7 integers, callback is function with no arguments
enable_alarm() - starts alarm
disable_alarm() - stops alarm

datetime tuple contains (year, month, day, dayofweek, hour, minute, second) telling when the alarm will be fired and any of them may be replaced with -1, which means that it is not checked. Eg. alarm firing every minute at 0 sec. should be set with (-1, -1, -1, -1, -1, -1, 0).

Simple example code:

import micropython as mp
from machine import RTC
import rtcalarm as ra

rtc = RTC()

def show_time(_):
  _, _, _, _, hour, minute, _, _ = rtc.datetime()
  print('{:2d}:{:02d}'.format(hour, minute))

def rtcalarm_callback():
  mp.schedule(show_time, None)

ra.set_alarm((-1, -1, -1, -1, -1, -1, 0), rtcalarm_callback)

