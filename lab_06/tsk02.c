#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>

#define USART_BAUD_RATE 9600UL

#define USART_UBRR_VALUE \
    ((F_CPU / (16UL * USART_BAUD_RATE)) - 1UL)

void usart_init(void)
{
    UCSR0A = 0x00;

    UBRR0H = (uint8_t)(USART_UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(USART_UBRR_VALUE);

    UCSR0B = (1 << RXEN0) | 
             (1 << TXEN0);  

    UCSR0C = (1 << UCSZ01) | 
             (1 << UCSZ00);
}

void usart_send(char character)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }

    UDR0 = (uint8_t)character;
}

char usart_receive(void)
{
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
    }

    return (char)UDR0;
}

void usart_send_string(const char *text)
{
    while (*text != '\0')
    {
        usart_send(*text);
        text++;
    }
}

int main(void)
{
    char received_character;

    usart_init();

    usart_send_string("\r\n");
    usart_send_string("1. Chanuth - E/21/007\r\n");
    usart_send_string("2. Lisitha - E/21/009\r\n");
    usart_send_string("\r\n");
   



    while (1)
    {
        received_character = usart_receive();
        usart_send(received_character);
    }

    return 0;
}