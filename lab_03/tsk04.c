#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 3036;              // 1 second preload with prescaler 256
    PORTB ^= (1 << PB5);       // Toggle PB5 every 1 second
}

void timer1_init(void)
{
    TCNT1 = 3036;              // Initial preload

    TCCR1A = 0x00;             // Normal mode
    TCCR1B = (1 << CS12);      // Prescaler = 256

    TIMSK1 = (1 << TOIE1);     // Enable Timer1 overflow interrupt

    sei();                     // Enable global interrupts
}

void simple_delay(void)
{
    for (volatile long i = 0; i < 50000; i++);
}

int main(void)
{
    DDRB |= (1 << PB5) | (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    timer1_init();

    unsigned char pattern[] = {
        (1 << PB0),
        (1 << PB1),
        (1 << PB2),
        (1 << PB3),
        (1 << PB2),
        (1 << PB1)
    };

    int i = 0;

    while (1)
    {
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
        PORTB |= pattern[i];

        i++;
        if (i >= 6)
        {
            i = 0;
        }

        simple_delay();
    }
}