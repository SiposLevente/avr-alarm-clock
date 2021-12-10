#ifndef TIMECONVERTER_H
#define TIMECONVERTER_H 1
#include "headers/TimeConverter.h"
#endif

int time = 720;
int year = 2021;
int date = 1;
unsigned char isLeapYear = 0;
unsigned int monthDayCount[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
unsigned int timeCache[4] = {0, 0, 0, 0};
unsigned int dateCache[4] = {0, 0, 0, 0};
unsigned int yearCache[4] = {0, 0, 0, 0};
unsigned int tmpTimeCache[4] = {0, 0, 0, 0};
unsigned int tmpDateCache[4] = {0, 0, 0, 0};
unsigned int tmpYearCache[4] = {0, 0, 0, 0};
unsigned char timeMaxDigits[4] = {2, 9, 5, 9};

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
        return year / 1000;
        break;

    case 1:
        return ((year / 100) - YearToNum(0) * 10);
        break;

    case 2:
        return ((year / 10) - (YearToNum(0) * 100) - (YearToNum(1) * 10));
        break;

    case 3:
        return (year - (YearToNum(0) * 1000) - (YearToNum(1) * 100) - (YearToNum(2) * 10));
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
            if (year % 400 == 0)
            {
                isLeapYear = 1;
            }
            else
            {
                isLeapYear = 0;
            }
        }
    }
    else
    {
        isLeapYear = 0;
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

unsigned char LeapYearChecker(int year)
{
    if (year % 4 == 0)
    {
        if (year % 100 != 0)
        {
            if (year % 400 == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    return 0;
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

void CacheTime()
{
    for (int i = 0; i < 4; i++)
    {
        timeCache[i] = TimeToNum(i);
    }
}

void CacheYear()
{
    for (int i = 0; i < 4; i++)
    {
        yearCache[i] = YearToNum(i);
    }
}

void CacheMonth()
{
    for (int i = 0; i < 2; i++)
    {
        dateCache[i] = DateToNum(i);
    }
}

void CacheDay()
{
    for (int i = 2; i < 4; i++)
    {
        dateCache[i] = DateToNum(i);
    }
}