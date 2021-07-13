#include <avr/interrupt.h>
#include <avr/io.h>
#include "TimeConverter.h"

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

// These pins controll the shift register
#define REG_VCC 6
#define REG_DATAINPUT 8
#define REG_STORAGECLK 9
#define REG_SHIFTCLK 10

// Controlls the buzzer and the led
#define BUZZER 11

// To display the correct number you have to start sending the digits from the last digit (which place represents the highest value)
// 1 => 0x06 => 00000110 => (send 0 and shift) x 5, (send 1 and shift) x 2, (send 0 and shift)
uint8_t DigitNumbers[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Numbers with a decimal dot.
uint8_t DigitNumbersDecimalDot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xF7, 0xEF, 0xEF};

// The set alarms are stored here.
uint32_t Alarms[9];

// Holds the time value. Default value = 43200 (12:00).
uint32_t Time = 43200;

// Initial setup. Sets the pins and other basic variables to the default state.
void initSetup();

// Displays on the given digit, the given number.
void displayDigit(int digitNum, int displayedNum);

void initSetup()
{
}

int main()
{
    while (1)
    {
    }
    return 0;
}

void displayDigit(int digitNum, int displayedNumb)
{
    switch (digitNum)
    {
    case 0:
        DDRD &= 0xF0;
        DDRD |= 0x01;
        break;

    case 1:
        DDRD &= 0xF0;
        DDRD |= 0x02;
        break;

    case 2:
        DDRD &= 0xF0;
        DDRD |= 0x04;
        break;

    case 3:
        DDRD &= 0xF0;
        DDRD |= 0x08;
        break;

    default:
        break;
    }
}