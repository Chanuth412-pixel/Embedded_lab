#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

int main(void)
{
    // PB0, PB1, PB2, PB3 as outputs
    DDRB = DDRB | 0x0F;

    while (1)
    {
        // Turn all four LEDs ON
        PORTB = PORTB | 0x0F;
        _delay_ms(BLINK_DELAY_MS);

        // Turn all four LEDs OFF
        PORTB = PORTB & ~0x0F;
        _delay_ms(BLINK_DELAY_MS);
    }
}