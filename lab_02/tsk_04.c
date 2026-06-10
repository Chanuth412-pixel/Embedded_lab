#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t fla = 0;
volatile uint8_t flb = 0;


ISR(PCINT2_vect) {
    fla = 1;
}

ISR(INT0_vect) {
    flb = 1; 
}

int main(void) {
    DDRB = 0x3F; 
    DDRD &= ~((1 << PD7) | (1 << PD2)); 
    PORTD |= (1 << PD7) | (1 << PD2); 
    EICRA |= (1 << ISC01); 
    EIMSK |= (1 << INT0); 
    PCICR |= (1 << PCIE2); 
    PCMSK2 |= (1 << PCINT23); 
    sei(); 

    while (1) {
        if (fla) {
             _delay_ms(50); /
             
             if (!(PIND & (1 << PD7))) { 
                bin = (bin << 1); 
                PORTB = bin & 0x3F;     
                while(!(PIND & (1 << PD7)));    
            }
            PCIFR |= (1 << PCIF2); 
            fla = 0;            
        }

       
        if (flb) {
            _delay_ms(50); 
            if (!(PIND & (1 << PD2))) { 
                bin = (bin << 1) | 1; 
                PORTB = bin & 0x3F;         
                while(!(PIND & (1 << PD2)));        
            }
            EIFR |= (1 << INTF0); 
            flb = 0;           
        }
    }
    return 0;
}