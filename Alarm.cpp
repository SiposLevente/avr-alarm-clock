
#ifndef ALARM_H
#define ALARM_H 1
#include "headers/Alarm.h"
#endif

#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H 1
#include "headers/AlarmClock.h"
#endif

#ifndef SHIFTREGCONTROLL_H
#define SHIFTREGCONTROLL_H 1
#include "headers/ShiftRegisterController.h"
#endif

#ifndef TIMECONVERTER_H
#define TIMECONVERTER_H 1
#include "headers/TimeConverter.h"
#endif

void DisplayAlarms(int digitNum)
{
    if (digitNum == 3)
    {
        SendData(digitNumbers[currentAlarm]);
    }
    else
    {
        SendData(digitLetters[digitNum]);
    }
}

void DisplayAlarmTime(int digitNum)
{
    SendData(digitNumbers[alarmCache[digitNum]]);
}

void CacheAlarm()
{
    for (int i = 0; i < 4; i++)
    {
        alarmCache[i] = TimeToNum(alarms[currentAlarm].time, i);
    }
}