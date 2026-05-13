#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Volatile variables are required for data shared between ISRs and main
volatile uint8_t binary_val = 0;
volatile uint8_t flag_A = 0;
volatile uint8_t flag_B = 0;

// ISR for Button A (Pin 7) using Pin Change Interrupt
ISR(PCINT2_vect) {
    flag_A = 1; // Set software flag for processing in main
}

// ISR for Button B (Pin 2) using External Interrupt INT0
ISR(INT0_vect) {
    flag_B = 1; // Set software flag for processing in main
}

int main(void) {
    // Configure PB0-PB5 as outputs for 6 LEDs
    DDRB = 0x3F; 

    // Configure PD7 and PD2 as inputs
    DDRD &= ~((1 << PD7) | (1 << PD2)); 
    // Enable internal pull-up resistors to maintain HIGH when unpressed
    PORTD |= (1 << PD7) | (1 << PD2); 

    // 1. Configure External Interrupt (INT0) for Button B on Pin 2
    // ISC01=1, ISC00=0 sets the interrupt to trigger on a falling edge
    EICRA |= (1 << ISC01); 
    // Enable the external interrupt request for INT0
    EIMSK |= (1 << INT0); 

    // 2. Configure Pin Change Interrupt for Button A on Pin 7
    // PCIE2 enables interrupts for the Port D group (PCINT23..16)
    PCICR |= (1 << PCIE2); 
    // PCINT23 specifically enables the interrupt for pin PD7
    PCMSK2 |= (1 << PCINT23); 

    // Enable global interrupts by setting the I-bit in the status register
    sei(); 

    while (1) {
        // Handle Button A: Inputs a binary '0'
        if (flag_A) {
            _delay_ms(50); // Debounce delay
            if (!(PIND & (1 << PD7))) { // Verify valid press (LOW state)
                binary_val = (binary_val << 1); // Shift bits left (adds 0 at LSB)
                PORTB = binary_val & 0x3F;      // Display on 6 LEDs
                while(!(PIND & (1 << PD7)));    // Wait for button release
            }
            PCIFR |= (1 << PCIF2); // Clear pending hardware flags
            flag_A = 0;            // Reset software flag
        }

        // Handle Button B: Inputs a binary '1'
        if (flag_B) {
            _delay_ms(50); // Debounce delay
            if (!(PIND & (1 << PD2))) { // Verify valid press (LOW state)
                binary_val = (binary_val << 1) | 1; // Shift left and OR with 1
                PORTB = binary_val & 0x3F;          // Display on 6 LEDs
                while(!(PIND & (1 << PD2)));        // Wait for button release
            }
            EIFR |= (1 << INTF0); // Clear pending hardware flags
            flag_B = 0;           // Reset software flag
        }
    }
    return 0;
}