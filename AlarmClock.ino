#include <avr/interrupt.h>
#include <avr/io.h>
#include "TimeConverter.h"
#include "ShiftRegisterController.h"

// Controlls the 7 segment display
// If set to 0 the digit will light up
// Uses DDRD/PIND - Pull up inputs
#define DIGITSELECT_0 0
#define DIGITSELECT_1 1
#define DIGITSELECT_2 2
#define DIGITSELECT_3 3

// Buttons used to operate the alarm clock
// Uses DDRD/PIND - Pull up inputs
#define MODESELECTBTN 4
#define INCREMENTBTN 5
#define NEXTBTN 7

// Controlls the buzzer and the led
#define BUZZER 3

// To display the correct number you have to start sending the digits from the last digit (which place represents the highest value)
// 1 => 0x06 => 00000110 => (send 0 and shift) x 5, (send 1 and shift) x 2, (send 0 and shift)
unsigned char DigitNumbers[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Numbers with a decimal dot.
unsigned char DigitNumbersDecimalDot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xF7, 0xEF, 0xEF};

// The set alarms are stored here.
unsigned int Alarms[9];

// Holds the time value. Default value = 43200 (12:00).
unsigned int Time = 43200;

// Initial setup. Sets the pins and other basic variables to the default state.
void initSetup();

// Displays the time on a given digit.
void displayDigit(int digitNum, unsigned char dotPoint = 0);

// Steps every 1 second
void timerOneSetup();

void initSetup()
{
    sei();
    timerOneSetup();
}

int main()
{
    while (1)
    {
    }
    return 0;
}

void displayDigit(int digitNum, unsigned char dotPoint = 0)
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
        SendData(DigitNumbersDecimalDot[TimeToNum(digitNum, Time)]);
    }
    else
    {
        SendData(DigitNumbers[TimeToNum(digitNum, Time)]);
    }
}

void timerOneSetup()
{
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15625;
    TIMSK1 = (1 << OCIE1A);
}

// Holds the value of the minute counter
// If this variable reaches 0 it indicates that a minute has passed.
unsigned char minuteCounter = 60;

// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
    if (!minuteCounter--)
    {
        minuteCounter = 60;
    }
    Time++;
}