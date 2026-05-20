#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0x3F;          // In binary 00111111
    DDRD &= ~(1 << PD7);  // Pin 7 as input
    PORTD |= (1 << PD7);  // Enable pull-up on Pin 7

    uint8_t count = 0;
    uint8_t last_state = 1;

    while (1) {
        uint8_t current_state = (PIND & (1 << PD7)) >> PD7;

        // If the state has changed (either pressed OR released)
        if (current_state != last_state) {
            
            _delay_ms(50); // Pause 

            // Read the pin again to ensure it wasn't just noise
            current_state = (PIND & (1 << PD7)) >> PD7;

            // If it is STILL a valid press (falling edge)
            if (last_state == 1 && current_state == 0) { 
                count++;
                PORTB = count & 0x3F; // Show on Pins 8-13
            }
            
            // Update the state only after the bounce period is over
            last_state = current_state; 
        }
    }
    return 0;
}