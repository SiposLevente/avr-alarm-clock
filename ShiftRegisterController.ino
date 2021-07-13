#include "ShiftRegisterController.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void SendData(unsigned char data)
{
    DDRB &= 0xF9;
    for (int i = 0; i < sizeof(data); i++)
    {
        DDRB &= 0xFE;
        if (data && (1 << i))
        {
            DDRB |= (1 << REG_DATAINPUT);
        }
        DDRB ^= (1 << REG_STORAGECLK);
        DDRB ^= (1 << REG_STORAGECLK);
    }

    DDRB ^= (1 << REG_OUTPUTCLK);
    DDRB ^= (1 << REG_OUTPUTCLK);
}