#include <avr/io.h>

void delay_timer0_2ms(void)
{
    TCNT0 = 225;        // Initial value

    TCCR0A = 0x00;      // Normal mode
    TCCR0B = 0x05;      // Prescaler = 1024

    while ((TIFR0 & 0x01) == 0);   // Wait for overflow

    TCCR0A = 0x00;
    TCCR0B = 0x00;      // Stop timer

    TIFR0 = 0x01;       // Clear overflow flag
}

int main(void)
{
    DDRB |= (1 << PB5);     // PB5 as output

    while (1)
    {
        PORTB ^= (1 << PB5);    // Toggle LED
        delay_timer0_2ms();
    }
}