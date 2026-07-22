#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>

#define BAUD 9600UL
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1UL)

void usart_init(void)
{
    UCSR0A = 0;

    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;

    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_send(uint8_t data)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }

    UDR0 = data;
}

void usart_send_string(const char *text)
{
    while (*text != '\0')
    {
        usart_send((uint8_t)*text);
        text++;
    }
}

uint8_t EEPROMread(uint16_t address)
{
    while (EECR & (1 << EEPE))
    {
    }

    EEAR = address;
    EECR |= (1 << EERE);

    return EEDR;
}

void usart_send_hex_digit(uint8_t value)
{
    value &= 0x0F;

    if (value < 10)
    {
        usart_send('0' + value);
    }
    else
    {
        usart_send('A' + value - 10);
    }
}

void usart_send_hex_byte(uint8_t value)
{
    usart_send_hex_digit(value >> 4);
    usart_send_hex_digit(value);
}

void usart_send_hex_address(uint16_t value)
{
    usart_send_hex_digit(value >> 12);
    usart_send_hex_digit(value >> 8);
    usart_send_hex_digit(value >> 4);
    usart_send_hex_digit(value);
}

int main(void)
{
    uint16_t address;
    uint8_t data;

    usart_init();

    usart_send_string("\r\nEEPROM contents:\r\n");
    usart_send_string("Address  Hex  ASCII\r\n");
    usart_send_string("-------------------\r\n");

    for (address = 0; address < 1024; address++)
    {
        data = EEPROMread(address);

        usart_send_hex_address(address);
        usart_send_string("     ");
        usart_send_hex_byte(data);
        usart_send_string("    ");

        if (data >= 32 && data <= 126)
        {
            usart_send(data);
        }
        else
        {
            usart_send('.');
        }

        usart_send_string("\r\n");
    }

    usart_send_string("End of EEPROM.\r\n");

    while (1)
    {
    }
}