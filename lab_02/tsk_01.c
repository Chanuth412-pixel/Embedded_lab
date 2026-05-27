#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0x3F;
    DDRD &= ~(1 << PD7);
    PORTD |= (1 << PD7);
    uint8_t count = 0;
    uint8_t prev = 1;

    while (1) {
         uint8_t now = (PIND & (1 << PD7)) ? 1 : 0;
        if (now != prev) {
            _delay_ms(50);
             now = (PIND & (1 << PD7)) ? 1 : 0;
            if (prev == 1 && now == 0) {
                count++;
                PORTB = count & 0x3F;
            }
            prev = now;
        }
    }
    return 0;
}