#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= (1 << PD6);

    TCCR0A =
        (1 << COM0A1) |
        (1 << WGM01) |
        (1 << WGM00);

    TCCR0B =
        (1 << CS01) |
        (1 << CS00);

    while (1)
    {
        for(int i=0;i<256;i++)
        {
            OCR0A=i;
            _delay_ms(10);
        }

        for(int i=255;i>=0;i--)
        {
            OCR0A=i;
            _delay_ms(10);
        }
    }
}