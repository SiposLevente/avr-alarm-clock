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

void TimerOneSetup()
{
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15625;
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

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
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
}

// Next button interrupt
// Logic change trigger
ISR(INT0_vect)
{
    if (extIntZeroTriggered == 0)
    {
        switch (currentMode)
        {
        case 0:
            if (editMode)
            {
                if (selectedDigit < 3)
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

// Increment button interrupt
// Logic change trigger
ISR(INT1_vect)
{
    if (editMode)
    {
        if (extIntOneTriggered == 0)
        {
            extIntOneTriggered ^= 0x01;
        }
        else
        {
            extIntOneTriggered ^= 0x01;
        }
    }
}

// Mode select button interrupt
// Logic change trigger
ISR(PCINT0_vect)
{
    btnPress ^= 0x01;
    if (!btnPress)
    {
        if (btnHoldCounter >= 2)
        {
            editMode ^= 0x01;
        }
        else
        {
            if (editMode)
            {
                editMode = 0;
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