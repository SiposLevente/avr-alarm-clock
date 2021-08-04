#include "TimeConverter.h"

int TimeToNum(int digit, int time)
{
    int returnDigit = -1;
    switch (digit)
    {
    case 0:
        returnDigit = time / 60 / 60 / 10;
        break;

    case 1:
        returnDigit = (time / 60 / 60) - (TimeToNum(0, time) * 10);
        break;

    case 2:
        returnDigit = (time % (3600)) / 60 / 10;
        break;

    case 3:
        returnDigit = ((time % (3600) / 60) - (TimeToNum(2, time) * 10));
        break;

    default:
        break;
    }

    return returnDigit;
}

void CacheDigits(unsigned int digitsCache[], unsigned int time)
{
    for (int i = 0; i < 4; i++)
    {
        digitsCache[i] = TimeToNum(i, time);
    }
}