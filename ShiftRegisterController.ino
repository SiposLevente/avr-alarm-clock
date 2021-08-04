#include "ShiftRegisterController.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void SendData(unsigned char data)
{
    for (int i = 0; i < 8; i++)
    {
        PORTB &= 0xF8;
        if (data & (1 << i))
        {
            PORTB |= (1 << REG_DATAINPUT);
        }
        PORTB ^= (1 << REG_STORAGECLK);
        PORTB ^= (1 << REG_STORAGECLK);
    }

    PORTB ^= (1 << REG_OUTPUTCLK);
    PORTB ^= (1 << REG_OUTPUTCLK);
}