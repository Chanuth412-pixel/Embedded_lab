#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char 0count = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 100;   
    0count++;
    if (0count >= 5) 
    {
        PORTB ^= (1 << PB0);
        0count = 0;
    }
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 34286;  
    PORTB ^= (1 << PB1); 
}

void timer0_init(void)
{
    TCNT0 = 100;
    TCCR0A = 0x00;
    TCCR0B = (1 << CS02) | (1 << CS00);   
    TIMSK0 = (1 << TOIE0); 
}

void timer1_init(void)
{
    TCNT1 = 34286;
    TCCR1A = 0x00;
    TCCR1B = (1 << CS12);   
    TIMSK1 = (1 << TOIE1); 
}

int main(void)
{
    DDRB |= (1 << PB0) | (1 << PB1);
    timer0_init();
    timer1_init();
    sei(); 
    while (1)
    {
    }
    return 0;
}