#include "Alarm.h"
#include "AlarmClock.h"
#include "ShiftRegisterController.h"

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