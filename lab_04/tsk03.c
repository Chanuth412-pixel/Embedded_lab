#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#define PRESCALER 8UL

void timer1_pwm_init(void)
{
    DDRB |= (1 << DDB1); 
    TCCR1A = (1 << WGM11); 
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); 
}

void buzzer_on(void) { 
    TCCR1A |= (1 << COM1A1);
 }
void buzzer_off(void) { 
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); 
    PORTB &= ~(1 << PORTB1); 
}

void set_buzzer_frequency(uint16_t freq, uint8_t duty)
{
    uint32_t top = (F_CPU / (PRESCALER * freq)) - 1;
    ICR1 = top;
    OCR1A = ((top + 1) * duty) / 100 - 1;
}

int main(void)
{
    uint16_t freqs[4] = {500, 1000, 1500, 2000};
    timer1_pwm_init();

    while (1)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            set_buzzer_frequency(freqs[i], 50);
            buzzer_on();
            _delay_ms(700);
            buzzer_off();
            _delay_ms(200);
        }
    }
}