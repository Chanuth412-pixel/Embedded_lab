#include <avr/io.h>
#include <stdint.h>

#define THRESHOLD 512 

void adc_init_external_aref() {
    ADMUX = 0x00; 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read() {
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);   
    uint8_t low_byte = ADCL;
    uint8_t high_byte = ADCH;
    return (high_byte << 8) | low_byte;
}

int main(void) {

    DDRB |= (1 << PB5);
    
    adc_init_external_aref();
    
    while (1) {
        uint16_t light_level = adc_read();
        if (light_level < THRESHOLD) {
            PORTB |= (1 << PB5); 
        } else {
            PORTB &= ~(1 << PB5);
        }
    }
    return 0;
}
