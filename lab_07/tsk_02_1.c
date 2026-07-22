#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define BAUD 9600UL
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1UL)

static uint8_t sentence[1024];

void usart_init(void)
{
    UCSR0A = 0;

    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_send(uint8_t data)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }

    UDR0 = data;
}

uint8_t usart_receive(void)
{
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
    }

    return UDR0;
}

void usart_send_string(const char *text)
{
    while (*text != '\0')
    {
        usart_send((uint8_t)*text);
        text++;
    }
}

void EEPROMwrite(uint16_t address, uint8_t data)
{
    uint8_t oldSREG;

    while (EECR & (1 << EEPE))
    {
    }

    EEAR = address;
    EEDR = data;

    oldSREG = SREG;
    cli();

    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);

    SREG = oldSREG;
}

int main(void)
{
    uint16_t length = 0;
    uint16_t address;
    uint8_t received_character;

    usart_init();

    usart_send_string("\r\nEnter an ASCII sentence:\r\n");

    while (length < 1024)
    {
        received_character = usart_receive();

        if (received_character == '\n')
        {
            received_character = '\r';
        }

        sentence[length] = received_character;
        length++;

        if (received_character == '\r')
        {
            usart_send_string("\r\n");
            break;
        }

        usart_send(received_character);
    }

    for (address = 0; address < length; address++)
    {
        EEPROMwrite(address, sentence[address]);
    }

    while (EECR & (1 << EEPE))
    {
    }

    usart_send_string("Sentence saved to EEPROM.\r\n");

    while (1)
    {
    }
}