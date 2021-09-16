#include <avr/interrupt.h>
#include <avr/io.h>
#include "headers/AlarmClock.h"
#include "headers/TimeConverter.h"
#include "headers/ShiftRegisterController.h"

void DisplayTime(int digitNum)
{
    if (digitNum == 1 && showDotPoint)
    {
        SendData(digitNumbersDecimalDot[timeCache[digitNum]]);
    }
    else
    {
        SendData(digitNumbers[timeCache[digitNum]]);
    }
}

void DisplayTimeAlt(int digitNum)
{
    if (toggleDisplay)
    {
        if (digitNum == 1)
        {
            SendData(digitNumbersDecimalDot[dateCache[digitNum]]);
        }
        else
        {
            SendData(digitNumbers[dateCache[digitNum]]);
        }
    }
    else
    {
        SendData(digitNumbers[yearCache[digitNum]]);
    }
}

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

void Edit(int digitNum)
{
    switch (currentMode)
    {
    case 0:
        unsigned char tmpIsLeapYear = LeapYearChecker(tmpYearCache[0] * 1000 + tmpYearCache[1] * 100 + tmpYearCache[2] * 10 + tmpYearCache[3]);

        // selectedDigit<4 -> display time.
        // 3 < selectedDigit < 8 -> display month/day.
        // 7 < selectedDigit -> display year.
        if (selectedDigit < 4)
        {
            if (digitNum == selectedDigit && showDotPoint)
            {
                SendData(0x00);
            }
            else
            {
                SendData(digitNumbers[tmpTimeCache[digitNum]]);
            }
        }
        else if (3 < selectedDigit && selectedDigit < 8)
        {
            if (digitNum == selectedDigit % 4 && showDotPoint)
            {
                SendData(0x00);
            }
            else
            {
                if (digitNum == 1)
                {
                    SendData(digitNumbersDecimalDot[tmpDateCache[digitNum]]);
                }
                else
                {
                    SendData(digitNumbers[tmpDateCache[digitNum]]);
                }
            }
        }
        else
        {
            if (digitNum == selectedDigit % 4 && showDotPoint)
            {
                SendData(0x00);
            }
            else
            {
                SendData(digitNumbers[tmpYearCache[digitNum]]);
            }
        }

        if (incrementSelectedDigit)
        {
            if (selectedDigit < 4)
            {
                if (selectedDigit == 1 && tmpTimeCache[0] == 2)
                {
                    timeMaxDigits[1] = 3;
                }
                else
                {
                    timeMaxDigits[1] = 9;
                }

                if (tmpTimeCache[selectedDigit] < timeMaxDigits[selectedDigit])
                {
                    tmpTimeCache[selectedDigit]++;

                    if (tmpTimeCache[0] == 2 && tmpTimeCache[1] > 3)
                    {
                        tmpTimeCache[1] = 0;
                    }
                }
                else
                {
                    tmpTimeCache[selectedDigit] = 0;
                }
            }
            else if (3 < selectedDigit && selectedDigit < 8)
            {
                int tmpDateDigit = tmpDateCache[0] * 10 + tmpDateCache[1];
                switch (selectedDigit % 4)
                {
                case 0:
                    if (tmpDateCache[0] < 1)
                    {
                        tmpDateCache[0]++;
                    }
                    else
                    {
                        tmpDateCache[0] = 0;
                    }
                    break;

                case 1:
                    if (tmpDateCache[1] < 9)
                    {
                        tmpDateCache[1]++;
                    }
                    else
                    {
                        tmpDateCache[1] = 0;
                    }
                    break;

                case 2:
                    if (tmpDateCache[2] < monthDayCount[tmpDateDigit - 1] / 10)
                    {
                        tmpDateCache[2]++;
                    }
                    else
                    {
                        tmpDateCache[2] = 0;
                    }
                    break;

                case 3:
                {
                    unsigned char tmpLastDigitOfDay = 9;

                    if (tmpDateCache[2] == monthDayCount[tmpDateDigit - 1] / 10)
                    {
                        tmpLastDigitOfDay = tmpDateDigit - tmpDateDigit / 10 * 10;
                        if (tmpIsLeapYear && tmpDateDigit == 2)
                        {
                            tmpLastDigitOfDay++;
                        }
                    }

                    if (tmpDateCache[3] < tmpLastDigitOfDay)
                    {
                        tmpDateCache[3]++;
                    }
                    else
                    {
                        tmpDateCache[3] = 0;
                    }
                }
                break;
                }
            }
            else
            {
                if (tmpYearCache[selectedDigit] < 9)
                {
                    tmpYearCache[selectedDigit]++;
                }
                else
                {
                    tmpYearCache[selectedDigit] = 0;
                }
            }
            incrementSelectedDigit = 0;
        }

        if (tmpDateCache[0] == 1 && tmpDateCache[1] > 2)
        {
            tmpDateCache[1] = 0;
        }
        if (tmpDateCache[0] == 0 && tmpDateCache[1] == 0)
        {
            tmpDateCache[1] = 1;
        }
        if (tmpDateCache[1] == 2 && tmpDateCache[2] == 3)
        {
            tmpDateCache[2] = 0;
        }
        if (tmpDateCache[3] == 2 || tmpDateCache[3] == 3)
        {
            unsigned char tmpDateDayCount = monthDayCount[tmpDateCache[0] * 10 + tmpDateCache[1] - 1];
            if (tmpIsLeapYear)
            {
                tmpDateDayCount++;
            }

            if (tmpDateCache[3] > tmpDateDayCount - tmpDateDayCount / 10 * 10)
            {
                tmpDateCache[3] = 0;
            }
        }
        break;

        // case 1:
        //     // Editing alarm.
        //     break;
    }
}

void TimerOneSetup()
{
    TCCR1B = (1 << WGM12) | (1 << CS12);
    OCR1A = 31250;
    //OCR1A = 3900;
    TIMSK1 = (1 << OCIE1A);
}

void TimerTwoSetup()
{
    TCCR2A |= (1 << COM2A1) | (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2A = 75;
    TIMSK2 |= (1 << OCIE2A);
}

void ExtInterruptSetup()
{
    EIMSK |= (1 << INT0) | (1 << INT1);
    EICRA |= (1 << ISC10) | (1 << ISC00);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);
}

int main()
{
    InitSetup();

    while (1)
    {
    }
    return 0;
}

void InitSetup()
{
    InitialDigitCacheing();
    TimerOneSetup();
    TimerTwoSetup();
    ExtInterruptSetup();
    sei();
    DDRD |= 0xF0;
    PORTD |= 0x0C;
    DDRB |= 0x0F;
    PORTB |= 0x10;
}

void InitialDigitCacheing()
{
    CacheYear();
    LeapYearCheck();
    CacheMonth();
    CacheDay();
    CacheTime();
}

void DisplayDigit(int digitNum)
{
    PORTD &= 0x0F;

    PORTD |= (1 << digitNum + DIGITSELECT_0);

    if (editMode)
    {
        Edit(digitNum);
    }
    else
    {
        switch (currentMode)
        {
        case 0:
            if (altMode)
            {
                DisplayTimeAlt(digitNum);
            }
            else
            {
                DisplayTime(digitNum);
            }
            break;

        case 1:
            DisplayAlarms(digitNum);
            break;

        default:
            break;
        }
    }
}

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
    if (!showDotPoint)
    {
        if (!minuteCounter--)
        {
            time++;

            if (time >= 1440)
            {
                time = 0;
                int prevMonth = GetMonth();
                date++;
                if ((isLeapYear && date == 367) || (!isLeapYear && date == 366))
                {
                    date = 1;
                    year++;
                    CacheYear();
                    CacheMonth();
                    LeapYearCheck();
                }

                if (GetMonth() != prevMonth)
                {
                    CacheMonth();
                }

                CacheDay();
            }

            CacheTime();
            minuteCounter = 60;
        }

        // If the mode button is pressed the hold counter counts.
        if (btnPress)
        {
            btnHoldCounter++;
        }

        if (!currentMode && altMode)
        {
            if (!altModeCounter--)
            {
                toggleDisplay ^= 0x01;
                altModeCounter = YEARMONTHTOGGLETIME;
            }
        }
    }
    showDotPoint ^= 0x01;
}

// Timer 2 interrupt
ISR(TIMER2_COMPA_vect)
{
    DisplayDigit(currentDigit);

    if (currentDigit == 3)
    {
        currentDigit = 0;
    }
    else
    {
        currentDigit++;
    }

    inputTimer ^= 0x01;
}

// Next button interrupt
// Logic change trigger
ISR(INT0_vect)
{
    if (inputTimer)
    {

        if (extIntZeroTriggered == 0)
        {
            switch (currentMode)
            {
            case 0:
                if (editMode)
                {
                    if (selectedDigit < 11)
                    {
                        selectedDigit++;
                    }
                    else
                    {
                        selectedDigit = 0;
                    }
                }
                else
                {
                    altMode ^= 0x01;
                }
                break;

            case 1:
                if (currentAlarm < ALARMCOUNT)
                {
                    currentAlarm++;
                }
                else
                {
                    currentAlarm = 0;
                }

                break;

            default:
                break;
            }
            extIntZeroTriggered ^= 0x01;
        }
        else
        {
            extIntZeroTriggered ^= 0x01;
        }
    }
}

// Increment button interrupt
// Logic change trigger
ISR(INT1_vect)
{
    if (inputTimer)
    {
        if (editMode)
        {
            if (extIntOneTriggered == 0)
            {
                incrementSelectedDigit = 1;
                extIntOneTriggered ^= 0x01;
            }
            else
            {
                extIntOneTriggered ^= 0x01;
            }
        }
    }
}

// Mode select button interrupt
// Logic change trigger
ISR(PCINT0_vect)
{
    if (inputTimer)
    {
        btnPress ^= 0x01;
        if (!btnPress)
        {
            if (btnHoldCounter >= 2)
            {
                for (int i = 0; i < 4; i++)
                {
                    tmpTimeCache[i] = timeCache[i];
                    tmpDateCache[i] = dateCache[i];
                    tmpYearCache[i] = yearCache[i];
                }
                editMode = 1;
            }
            else
            {
                if (editMode)
                {
                    editMode = 0;
                    year = tmpYearCache[0] * 1000 + tmpYearCache[1] * 100 + tmpYearCache[2] * 10 + tmpYearCache[0];
                    LeapYearCheck();
                    int tmpMonth = tmpDateCache[0] * 10 + tmpDateCache[0];
                    for (int i = 0; i < tmpMonth; i++)
                    {
                        date += monthDayCount[i];
                    }
                    date += tmpDateCache[2] * 10 + tmpDateCache[3];
                    time = tmpTimeCache[0] * 600 + tmpTimeCache[1] * 60 + tmpTimeCache[2] * 10 + tmpTimeCache[3];
                    minuteCounter = 60;
                    CacheYear();
                    CacheMonth();
                    CacheDay();
                    CacheTime();
                }
                else
                {
                    if (currentMode == MODECOUNT - 1)
                    {
                        currentMode = 0;
                    }
                    else
                    {
                        currentMode++;
                        switch (currentMode)
                        {
                        case 0:
                            CacheTime();
                            CacheDay();
                            CacheMonth();
                            CacheYear();
                            break;
                        default:
                            break;
                        }
                    }
                    altMode = 0;
                }
            }
        }
        btnHoldCounter = 0;
    }
}