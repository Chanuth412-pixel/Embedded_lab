#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char timer0_count = 0;
volatile unsigned int timer1_count = 0;

/* TIMER0 for 50 ms LED */
ISR(TIMER0_OVF_vect)
{
    TCNT0 = 100;   // About 10 ms

    timer0_count++;

    if (timer0_count >= 5)    // 5 × 10ms = 50ms
    {
        PORTB ^= (1 << PB5);
        timer0_count = 0;
    }
}

/* TIMER1 for 500 ms LED */
ISR(TIMER1_OVF_vect)
{
    TCNT1 = 3036;   // About 4 ms

    timer1_count++;

    if (timer1_count >= 125)   // 125 × 4ms = 500ms
    {
        PORTB ^= (1 << PB3);
        timer1_count = 0;
    }
}

void timer0_init(void)
{
    TCNT0 = 100;

    TCCR0A = 0x00;
    TCCR0B = 0x05;          // Prescaler = 1024

    TIMSK0 = (1 << TOIE0);
}

void timer1_init(void)
{
    TCNT1 = 3036;

    TCCR1A = 0x00;
    TCCR1B = (1 << CS12);   // Prescaler = 256

    TIMSK1 = (1 << TOIE1);
}

int main(void)
{
    DDRB |= (1 << PB5) | (1 << PB3);

    timer0_init();
    timer1_init();

    sei();

    while (1)
    {
        // Everything handled by interrupts
    }
}