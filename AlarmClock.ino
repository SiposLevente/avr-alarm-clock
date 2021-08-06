#include <avr/interrupt.h>
#include <avr/io.h>
#include "TimeConverter.h"
#include "ShiftRegisterController.h"

// Controlls the 7 segment display
// If set to 0 the digit will light up
// Uses DDRD, controlls the transistors
#define DIGITSELECT_0 4
#define DIGITSELECT_1 5
#define DIGITSELECT_2 6
#define DIGITSELECT_3 7

// Buttons used to operate the alarm clock
// Uses DDRD/PIND - Pull up inputs
#define NEXTBTN 2
#define INCREMENTBTN 3
#define MODESELECTBTN 7

// Controlls the buzzer and the led
#define BUZZER 3

// To display the correct number you have to start sending the digits from the last digit (which place represents the highest value)
// 1 => 0x06 => 00000110 => (send 0 and shift) x 5, (send 1 and shift) x 2, (send 0 and shift)
unsigned char digitNumbers[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Numbers with a decimal dot.
unsigned char digitNumbersDecimalDot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

// The set alarms are stored here.
int alarms[9];

// This array holds the digit values of the clock.
unsigned int digitsCache[4];

// Holds the time value. Default value = 43200 (12:00).
int time = 43200;

// Holds the value of the minute counter
// If this variable reaches 0 it indicates that a minute has passed.
unsigned char minuteCounter = 60;

// if true the dot point for the given digit will light up.
unsigned char showDotPoint = 0;

// Holds the value of the selected mode.
unsigned char currentMode = 0;

// Determines if the current mode is in an alternative mode.
unsigned char altMode = 0;

// 1 if mode button is pressed.
unsigned char btnPress = 0;

// Counts how many seconds were the button pressed.
unsigned char btnHoldCounter = 0;

// Initial setup. Sets the pins and other basic variables to the default state.
void InitSetup();

// Displays the time on a given digit.
void DisplayDigit(int digitNum, unsigned char dotPoint);

// Steps every 1 second
void TimerOneSetup();

// This timer will refresh the 7 segment display
void TimerZeroSetup();

// External interrupt setup. Detects button presses.
void ExtInterruptSetup();

void InitSetup()
{
    CacheDigits(digitsCache, time);
    TimerZeroSetup();
    TimerOneSetup();
    ExtInterruptSetup();
    sei();
    DDRD |= 0xF0;
    PORTD |= 0x0C;
    DDRB |= 0x0F;
    PORTB |= 0x10;
}

int main()
{
    InitSetup();

    while (1)
    {
    }
    return 0;
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

void TimerOneSetup()
{
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15625;
    //OCR1A = 5;
    TIMSK1 = (1 << OCIE1A);
}

void TimerZeroSetup()
{
    TCCR0A = (1 << COM0A1) | (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);

    OCR0A = 80;
    TIMSK0 = (1 << OCIE0A);
}

void ExtInterruptSetup()
{
    EIMSK |= (1 << INT0) | (1 << INT1);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);
}

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
    if (!minuteCounter--)
    {
        if (time >= 86400)
        {
            time = 0;
        }
        
        CacheDigits(digitsCache, time);

        minuteCounter = 60;
    }

    // If the mode button is pressed the hold counter counts.
    if (btnPress)
    {
        btnHoldCounter++;
    }

    showDotPoint ^= 0x01;
    time++;
}

unsigned char currDigit = 0;

// Timer 0 interrupt
ISR(TIMER0_COMPA_vect)
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
// Low level trigger
ISR(INT0_vect)
{
}

// Increment button interrupt
// Low level trigger
ISR(INT1_vect)
{
}

// Mode select button interrupt
// Logic change trigger
ISR(PCINT0_vect)
{
    btnPress ^= 0x01;
    if (btnHoldCounter >= 2)
    {
        // Mode switch
    }
    else
    {
        // Alt mode
    }
}