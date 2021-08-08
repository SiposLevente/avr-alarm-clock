#include "TimeConverter.h"

int TimeToNum(int digit)
{
    int returnDigit = -1;
    switch (digit)
    {
    case 0:
        returnDigit = time / 600;
        break;

    case 1:
        returnDigit = (time / 60) - TimeToNum(0) * 10;
        break;

    case 2:
        returnDigit = (time % 60) / 10;
        break;

    case 3:
        returnDigit = (time % 60) - TimeToNum(2) * 10;
        break;

    default:
        break;
    }

    return returnDigit;
}

void LeapYearCheck()
{
    if (year % 4 == 0)
    {
        if (year % 100 != 0)
        {
            isLeapYear = 1;
        }
        if (year % 400 == 0)
        {
            isLeapYear = 0;
        }
    }
}