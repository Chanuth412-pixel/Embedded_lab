#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t pin_changed = 0; //Changed state (Read from mem)

// Triggered whenever the state of Pin 7 changes
ISR(PCINT2_vect) {
    pin_changed = 1;
}

int main(void) {
    DDRB |= (1 << PB0);      // Set Pin 8 as output
    DDRD &= ~(1 << PD7);     // Set Pin 7 as input
    PORTD |= (1 << PD7);     // Enable internal pull-up resistor

    PCICR |= (1 << PCIE2);   // Enable pin-change interrupts for Port D
    PCMSK2 |= (1 << PCINT23);// Enable interrupt on Pin 7

    sei();                   // Enable global interrupts

    uint8_t last_state = 1;  // Button starts unpressed

    while (1) {
        if (pin_changed) {
            _delay_ms(50);   // Debounce delay

            uint8_t current_state = (PIND & (1 << PD7)) >> PD7;

            // Toggle Pin 8 when the button is released
            if (last_state == 0 && current_state == 1) {
                PORTB ^= (1 << PB0);
            }

            last_state = current_state;
            PCIFR |= (1 << PCIF2); // Clear pending interrupt flag
            pin_changed = 0;       // Reset software flag
        }
    }

    return 0;
}