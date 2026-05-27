#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char overflow_count = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 100;   // About 10 ms overflow

    overflow_count++;

    if (overflow_count >= 10)   // 10 × 10ms = 100ms
    {
        PORTB ^= (1 << PB5);
        overflow_count = 0;
    }
}

void timer0_init(void)
{
    TCNT0 = 100;

    TCCR0A = 0x00;
    TCCR0B = 0x05;          // Prescaler = 1024

    TIMSK0 = (1 << TOIE0);  // Enable Timer0 overflow interrupt

    sei();
}

int main(void)
{
    DDRB |= (1 << PB5);

    timer0_init();

    while (1)
    {
        // main loop empty
    }
}