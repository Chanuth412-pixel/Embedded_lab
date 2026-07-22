#include <avr/io.h>

void adc_init() {
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0); 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
} 
uint8_t adc_read_left_adjusted() {
    ADCSRA |= (1 << ADSC); 
    while (!(ADCSRA & (1 << ADIF))); 
    ADCSRA |= (1 << ADIF); 
    return ADCH; 

int main(void) {
    DDRD = 0xFF; 
    adc_init();
    while (1) {
        PORTD = adc_read_left_adjusted();
    }
    return 0;
}