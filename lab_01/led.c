#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

int main(void)
{
    // Arduino digital pin 13 = PB5
    DDRB = DDRB | (1 << 5);

    while (1)
    {
        PORTB = PORTB | (1 << 5);
        _delay_ms(BLINK_DELAY_MS);

        PORTB = PORTB & ~(1 << 5);
        _delay_ms(BLINK_DELAY_MS);
    }
}