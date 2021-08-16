#include <avr/interrupt.h>
#include <avr/io.h>
#include "TimeConverter.h"
#include "ShiftRegisterController.h"
#include "AlarmClock.h"

void TimerOneSetup()
{
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15625;
    //OCR1A = 5;
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
    CacheDigits();
    TimerOneSetup();
    TimerTwoSetup();
    ExtInterruptSetup();
    sei();
    DDRD |= 0xF0;
    PORTD |= 0x0C;
    DDRB |= 0x0F;
    PORTB |= 0x10;
}

void CacheDigits()
{
    switch (currentMode)
    {
    case 0:
        if (altMode)
        {
        }
        else
        {
        }
        break;

    case 1:
        if (altMode)
        {
        }
        else
        {
        }
        break;

    default:
        break;
    }
}

void DisplayDigit(int digitNum, unsigned char dotPoint = 0)
{
    PORTD &= 0x0F;

    PORTD |= (1 << digitNum + DIGITSELECT_0);

    if (digitNum == 1 && dotPoint)
    {
        SendData(digitNumbersDecimalDot[digitsCache[digitNum]]);
    }
    else
    {
        SendData(digitNumbers[digitsCache[digitNum]]);
    }
}

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
    if (!minuteCounter--)
    {
        if (time >= 1440)
        {
            time = 0;
        }

        time++;
        CacheTime();
        minuteCounter = 60;
    }

    // If the mode button is pressed the hold counter counts.
    if (btnPress)
    {
        btnHoldCounter++;
    }

    showDotPoint ^= 0x01;
}

// Timer 2 interrupt
ISR(TIMER2_COMPA_vect)
{
    switch (currentMode)
    {
    case 0:
        if (altMode == 0)
        {

            unsigned char dot = 0;
            if (currDigit == 1 && showDotPoint)
            {
                dot = 1;
            }
            DisplayDigit(currDigit, dot);
        }
        else
        {
            // Displays year for 5 sec then month and day for 5 sec
        }

        break;

    case 1:
        // Alarm settings
        break;

    default:
        break;
    }

    if (currDigit == 3)
    {
        currDigit = 0;
    }
    else
    {
        currDigit++;
    }
}

// Next button interrupt
// Logic change trigger
ISR(INT0_vect)
{
    switch (currentMode)
    {
    case 0:
        if (editMode)
        {
            // Cycle between digits.
        }
        else
        {
            altMode ^= 0x01;
        }
        break;

    default:
        break;
    }
}

// Increment button interrupt
// Logic change trigger
ISR(INT1_vect)
{
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

        // Solve so this triggers only once.
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
                }
                altMode = 0;
            }
        }
    }
}