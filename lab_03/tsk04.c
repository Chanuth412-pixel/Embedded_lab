#define F_CPU 16000000UL // Required for _delay_ms()
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


ISR(TIMER1_OVF_vect)
{
    TCNT1 = 3036; 
    PORTB ^= (1 << PB5);
}

void timer1_init(void)
{
    TCNT1 = 3036;
    TCCR1A = 0x00;
    TCCR1B = (1 << CS12);   
    TIMSK1 = (1 << TOIE1);  
}

int main(void)
{
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB5);
    timer1_init();
    sei(); 

    while (1)
    {
        for (int i = 0; i < 4; i++) 
        {
            PORTB = (PORTB & 0xF0) | (1 << i);
            _delay_ms(100);
        }

        for (int i = 2; i > 0; i--) 
        {
            PORTB = (PORTB & 0xF0) | (1 << i);
            _delay_ms(100);
        }
    }
    return 0;
}