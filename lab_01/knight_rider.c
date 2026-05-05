#include <avr/io.h>
#include <util/delay.h>

#define DELAY_MS 200

int main(void)
{
    // PB0, PB1, PB2, PB3 as outputs
    DDRB = DDRB | 0x0F;

    while (1)
    {
        // A
        PORTB = (PORTB & ~0x0F) | (1 << 0);
        _delay_ms(DELAY_MS);

        // B
        PORTB = (PORTB & ~0x0F) | (1 << 1);
        _delay_ms(DELAY_MS);

        // C
        PORTB = (PORTB & ~0x0F) | (1 << 2);
        _delay_ms(DELAY_MS);

        // D
        PORTB = (PORTB & ~0x0F) | (1 << 3);
        _delay_ms(DELAY_MS);

        // C
        PORTB = (PORTB & ~0x0F) | (1 << 2);
        _delay_ms(DELAY_MS);

        // B
        PORTB = (PORTB & ~0x0F) | (1 << 1);
        _delay_ms(DELAY_MS);
    }
}