#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char ocount = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 100;  
    ocount++;

    if (ocount >= 100)  
    {
        PORTB ^= (1 << PB5);
        ocount = 0;
    }
}

void timer0_init(void)
{
    TCNT0 = 100;
    TCCR0A = 0x00;
    TCCR0B = 0x05;          
    TIMSK0 = (1 << TOIE0);  

    sei();
}

int main(void)
{
    DDRB |= (1 << PB5);
    timer0_init();
    while (1)
    {
    }
}