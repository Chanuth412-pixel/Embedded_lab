#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t changed = 0;

ISR(PCINT2_vect) {
    changed = 1;
}

int main(void) {
    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD7);
    PORTD |= (1 << PD7);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT23);
    sei();
    uint8_t prev = 1;

    while (1) {
        if (changed) {
             _delay_ms(50);
            uint8_t now = (PIND & (1 << PD7)) ? 1 : 0;
            if (prev == 0 && now == 1) {
                PORTB ^= (1 << PB0);
            }  
            prev = now;
            PCIFR |= (1 << PCIF2);
            changed = 0;
        }
    }

    return 0;
}