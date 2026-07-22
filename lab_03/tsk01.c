#include <avr/io.h>

void delay_timer0_2ms(void)
{
    TCNT0 = 131;              
    TCCR0A = 0x00;            
    TCCR0B = (1 << CS02);     
    while ((TIFR0 & (1 << TOV0)) == 0); 
    TCCR0B = 0x00;            
    TIFR0 |= (1 << TOV0);     
}

int main(void)
{
    DDRB |= (1 << PB5);     
    while (1)
    {
        PORTB ^= (1 << PB5);  
        delay_timer0_2ms();
    }
}