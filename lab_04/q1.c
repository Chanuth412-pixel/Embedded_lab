#include <avr/io.h>

int main(void)
{
    DDRD |= (1 << PD6);     // OC0A output

    OCR0A = 127;            // 50% duty cycle

    TCCR0A =
        (1 << COM0A1) |     // non-inverting
        (1 << WGM01)  |
        (1 << WGM00);       // Fast PWM

    TCCR0B =
        (1 << CS01) |
        (1 << CS00);        // prescaler 64

    while (1)
    {
    }
}