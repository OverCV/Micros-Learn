/*
 * one.cpp
 * Created: 2024-07-21 14:13:53
 * Author: over.dev
 */

#include <avr/io.h>
#include <avr/iocan128.h>

#include <util/delay.h>

class LED
{
public:
    LED()
    {
        DDRB |= (1 << PB5);
    }
    void toggle()
    {
        PORTB ^= (1 << PB5);
    }
};

int main(void)
{
    /* Replace with your initialization code */
    LED myLed;
    while (1)
    {
        /* Replace with your loop code */
        myLed.toggle();
        _delay_ms(500);
    }
    return 0;
}