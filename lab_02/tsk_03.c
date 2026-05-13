#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t count = 0;
volatile uint8_t pin_changed = 0;

// Set flag when Pin 7 changes state
ISR(PCINT2_vect) {
    pin_changed = 1;
}

int main(void) {
    DDRB = 0x3F;          // Pins 8–13 as LED outputs
    DDRD &= ~(1 << PD7);  // Pin 7 as input
    PORTD |= (1 << PD7);  // Enable internal pull-up

    PCICR |= (1 << PCIE2);     // Enable Port D pin-change interrupts
    PCMSK2 |= (1 << PCINT23);  // Enable interrupt on Pin 7

    sei(); // Enable global interrupts

    uint8_t last_state = 1;

    while (1) {
        if (pin_changed) {
            _delay_ms(50); // Debounce delay

            uint8_t current_state = (PIND & (1 << PD7)) >> PD7;

            // Count once when the button is released
            if (last_state == 0 && current_state == 1) {
                count++;
                PORTB = count & 0x3F;
            }

            last_state = current_state;
            PCIFR |= (1 << PCIF2); // Clear interrupt flag
            pin_changed = 0;
        }
    }

    return 0;
}