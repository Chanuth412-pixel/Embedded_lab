#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

uint8_t ct =0;  
volatile uint8_t flg=0;

ISR(PCINT2_vect) {   
    flg =1;
}

int main(void) {
    DDRB =  0x3F;          
    DDRD &=  ~(1 <<PD7);   
    PORTD |=  (1<< PD7);  
    PCICR |= (1<< PCIE2);     
    PCMSK2 |= (1 <<PCINT23);  
    sei(); 
    uint8_t lst=1;

    while (1) {
       if (flg) {
             _delay_ms(50); 
              uint8_t curr = (PIND & (1 <<PD7))>> PD7;
             if (lst== 0 && curr==1) {
                 ct++;
                PORTB = ct & 0x3F;
            }
            lst=curr;
            PCIFR |= (1 << PCIF2); 
            flg =0;
        }
    }

    return 0;
}