#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

void play_frequency(unsigned int freq)
{
    unsigned int top;

    // Timer1 Fast PWM frequency formula:
    // f = F_CPU / (prescaler * (1 + ICR1))
    // prescaler = 8
    top = (F_CPU / (8UL * freq)) - 1;

    ICR1 = top;
    OCR1A = top / 2;     // 50% duty cycle
}

int main(void)
{
    DDRB |= (1 << PB1);  // D9 = OC1A output

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // prescaler 8

    while (1)
    {
        play_frequency(500);
        _delay_ms(800);

        play_frequency(1000);
        _delay_ms(800);

        play_frequency(1500);
        _delay_ms(800);

        play_frequency(2000);
        _delay_ms(800);
    }
}