#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
    DDRD |= (1 << DDD6);   
    OCR0A = 127;           
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); 
    TCCR0B = (1 << CS01) | (1 << CS00); 
    while (1) { }
}