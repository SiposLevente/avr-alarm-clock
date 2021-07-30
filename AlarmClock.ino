#include <avr/interrupt.h>
#include <avr/io.h>
#include "TimeConverter.h"
#include "ShiftRegisterController.h"

// Controlls the 7 segment display
// If set to 0 the digit will light up
// Uses DDRD/PIND - Pull up inputs
#define DIGITSELECT_0 0
#define DIGITSELECT_1 1
#define DIGITSELECT_2 4
#define DIGITSELECT_3 5

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
unsigned char digitNumbersDecimalDot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xF7, 0xEF, 0xEF};

// The set alarms are stored here.
unsigned int alarms[9];

// This array holds the digit values of the clock.
unsigned int digitsCache[4];

// Holds the time value. Default value = 43200 (12:00).
unsigned int time = 43200;

// Holds the value of the minute counter
// If this variable reaches 0 it indicates that a minute has passed.
unsigned char minuteCounter = 60;

// if true the dot point for the given digit will light up.
unsigned char showDotPoint = 0;

// Holds the value of the selected mode.
unsigned char currentMode = 0;

// Determines if the current mode is in an alternative mode
unsigned char altMode = 0;

// Initial setup. Sets the pins and other basic variables to the default state.
void InitSetup();

// Displays the time on a given digit.
void DisplayDigit(int digitNum, unsigned char dotPoint = 0);

// Steps every 1 second
void TimerOneSetup();

// This timer will refresh the 7 segment display
void TimerZeroSetup();

// External interrupt setup. Detects button presses.
void ExtInterruptSetup();

void InitSetup()
{
    sei();
    TimerZeroSetup();
    TimerOneSetup();
    ExtInterruptSetup();
}

int main()
{
    while (1)
    {
    }
    return 0;
}

void DisplayDigit(int digitNum, unsigned char dotPoint = 0)
{
    unsigned char drawDot = 0;

    if (digitNum == 1)
    {
        drawDot = 1;
    }

    DDRD &= 0xF0;
    DDRD |= (0x0F & ~(1 << digitNum));

    if (drawDot && dotPoint)
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
    TIMSK1 = (1 << OCIE1A);
}

void TimerZeroSetup()
{
    TCCR0A = (1 << COM0A1) | (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);

    // if OCR0A = 108 the refresh rate is ~144Hz
    // if OCR0A = 250 the refresh rate is ~60Hz
    OCR0A = 250;
    TIMSK0 = (1 << OCIE0A);
}

void ExtInterruptSetup()
{
    EIMSK |= (1 << INT0) | (1 << INT1);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT23);
}

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
    if (!minuteCounter--)
    {
        for (int i = 0; i < 4; i++)
        {
            digitsCache[i] = TimeToNum(i, time);
        }

        minuteCounter = 60;
    }
    showDotPoint ^= 0x01;
    time++;
}

// Timer 0 interrupt
ISR(TIMER0_COMPA_vect)
{
    for (int i = 0; i < 4; i++)
    {
        unsigned char dot = 0;
        if (i == 1 && showDotPoint)
        {
            dot = 1;
        }
        DisplayDigit(i, dot);
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
ISR(PCINT2_vect)
{
}