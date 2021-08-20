#include <avr/interrupt.h>
#include <avr/io.h>
#include "headers/TimeConverter.h"
#include "headers/ShiftRegisterController.h"
#include "headers/AlarmClock.h"

void CacheDisplayDigits(unsigned char arrayToCache[4])
{
    for (int i = 0; i < 4; i++)
    {
        displayCache[i] = arrayToCache[i];
    }
}

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
    CacheMonth();
    CacheDay();
    CacheTime();
}

void DisplayDigit(int digitNum)
{
    switch (currentMode)
    {
    case 0:
        if (altMode)
        {
            // Alt time mode.
        }
        else
        {
            PORTD &= 0x0F;

            PORTD |= (1 << digitNum + DIGITSELECT_0);

            if (digitNum == 1 && showDotPoint)
            {
                SendData(digitNumbersDecimalDot[timeCache[digitNum]]);
            }
            else
            {
                SendData(digitNumbers[timeCache[digitNum]]);
            }
        }
        break;

    case 1:
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
        if (time >= 1440)
        {
            time = 0;
        }

        time++;
        CacheTime();
        if (currentMode == 0)
        {
            // CacheDisplayDigits(timeCache);
        }

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

        default:
            break;
        }
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