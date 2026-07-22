#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

int main(void)
{
    DDRD |= (1 << DDD6);
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);

    while (1)
    {
        for(uint16_t duty = 0; duty <= 255; duty++) {
             OCR0A = duty; 
             _delay_ms(10); } 
             
        for(int16_t duty = 255; duty >= 0; duty--) { 
            OCR0A = duty; 
            _delay_ms(10); } 
    }
}