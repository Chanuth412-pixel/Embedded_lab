/*
 * CO321 Lab 6 - Exercise 2
 * Transmit group-member names and E-numbers
 *
 * Microcontroller: ATmega328P
 * Clock frequency: 16 MHz
 * Baud rate: 9600
 * Frame format: 8 data bits, no parity, 1 stop bit
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
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
        /* Wait until UDR0 is ready. */
    }

    UDR0 = (uint8_t)character;
}

char usart_receive(void)
{
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
        /* Wait until a character is received. */
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
    usart_init();

    /*
     * Wait for the computer's serial terminal to become ready.
     */
    _delay_ms(1000);

    usart_send_string("\r\n");
    usart_send_string("========================================\r\n");
    usart_send_string("        CO321 Embedded Systems\r\n");
    usart_send_string("       Lab 6 - Serial Communication\r\n");
    usart_send_string("========================================\r\n");
    usart_send_string("\r\n");

    /*
     * Replace these sample details with your group details.
     */
    usart_send_string("Group Members:\r\n");
    usart_send_string("\r\n");

    usart_send_string("1. Student Name One - E/21/001\r\n");
    usart_send_string("2. Student Name Two - E/21/002\r\n");
    usart_send_string("3. Student Name Three - E/21/003\r\n");

    usart_send_string("\r\n");
    usart_send_string("Transmission completed.\r\n");
    usart_send_string("========================================\r\n");

    /*
     * The program remains here after sending the information.
     */
    while (1)
    {
        /* Infinite loop */
    }

    return 0;
}