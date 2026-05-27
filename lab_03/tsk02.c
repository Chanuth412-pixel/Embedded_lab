#include <avr/io.h>

void delay_timer0_16ms(void)
{
    TCNT0 = 0;

    TCCR0A = 0x00;
    TCCR0B = 0x05;      // Prescaler = 1024

    while ((TIFR0 & 0x01) == 0);

    TCCR0B = 0x00;
    TIFR0 = 0x01;
}

void delay_500ms(void)
{
    for (int i = 0; i < 31; i++)
    {
        delay_timer0_16ms();
    }
}

int main(void)
{
    DDRB |= (1 << PB5);

    while (1)
    {
        PORTB ^= (1 << PB5);
        delay_500ms();
    }
}