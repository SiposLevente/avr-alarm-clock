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

int DateToNum(int digit)
{
    switch (digit)
    {
    case 0:
        if (GetMonth() > 9)
        {
            return 1;
        }
        return 0;
        break;

    case 1:
    {
        int tempMonth = GetMonth();
        if (tempMonth > 9)
        {
            return tempMonth - 10;
        }
        return tempMonth;
    }
    break;

    case 2:
        return GetDay() / 10;
        break;

    case 3:
    {
        int tempDay = GetDay();
        return tempDay - (tempDay / 10) * 10;
    }
    break;

    default:
        break;
    }
    return -1;
}

int YearToNum(int digit)
{
    switch (digit)
    {
    case 0:
        // First year digit.
        break;

    case 1:
        // Second year digit.
        break;

    case 2:
        // Third year digit.
        break;

    case 3:
        // Fourth year digit.
        break;

    default:
        return -1;
        break;
    }
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

    if (isLeapYear)
    {
        monthDayCount[1] = 29;
    }
    else
    {
        monthDayCount[1] = 28;
    }
}

int GetMonth()
{
    int tempMonth = date;
    int counter = 0;
    while (counter < 12 && tempMonth > 0)
    {
        tempMonth -= monthDayCount[counter];
        counter++;
    }

    return counter;
}

int GetDay()
{
    int tempMonth = date;
    int counter = GetMonth();
    for (int i = 0; i < GetMonth(); i++)
    {
        tempMonth -= monthDayCount[i];
    }

    return monthDayCount[counter - 1] + tempMonth;
}