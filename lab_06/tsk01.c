/*
 * CO321 Lab 6 - Exercise 1
 * USART initialization, transmission and reception
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
#include <stdint.h>

#define USART_BAUD_RATE 9600UL

/*
 * UBRR value for normal asynchronous USART mode:
 *
 * UBRR = F_CPU / (16 * baud rate) - 1
 */
#define USART_UBRR_VALUE \
    ((F_CPU / (16UL * USART_BAUD_RATE)) - 1UL)

/*
 * Initialize USART0.
 *
 * Configuration:
 * - 9600 baud
 * - Asynchronous mode
 * - 8 data bits
 * - No parity
 * - 1 stop bit
 * - Receiver enabled
 * - Transmitter enabled
 */
void usart_init(void)
{
    /* Use normal-speed asynchronous USART mode. */
    UCSR0A = 0x00;

    /* Set the baud-rate register. */
    UBRR0H = (uint8_t)(USART_UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(USART_UBRR_VALUE);

    /*
     * Enable receiver and transmitter.
     *
     * RXEN0 = Receiver enable
     * TXEN0 = Transmitter enable
     */
    UCSR0B = (1 << RXEN0) |
             (1 << TXEN0);

    /*
     * Set frame format:
     *
     * UMSEL01:0 = 00 : Asynchronous USART
     * UPM01:0   = 00 : No parity
     * USBS0     = 0  : 1 stop bit
     * UCSZ01:0  = 11 : 8 data bits
     */
    UCSR0C = (1 << UCSZ01) |
             (1 << UCSZ00);
}

/*
 * Transmit one character through USART0.
 */
void usart_send(char character)
{
    /*
     * Wait until USART Data Register Empty flag becomes 1.
     * This indicates that UDR0 is ready for new data.
     */
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
        /* Polling */
    }

    /* Write the character to the USART data register. */
    UDR0 = (uint8_t)character;
}

/*
 * Wait for and receive one character through USART0.
 */
char usart_receive(void)
{
    /*
     * Wait until Receive Complete flag becomes 1.
     * This indicates that a complete character is available.
     */
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
        /* Polling */
    }

    /* Return the received character. */
    return (char)UDR0;
}

/*
 * Transmit a null-terminated string.
 */
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
    usart_send_string("CO321 Lab 6 - Exercise 1\r\n");
    usart_send_string("USART initialized at 9600 baud.\r\n");
    usart_send_string("Enter characters to test the echo function:\r\n");

    while (1)
    {
        received_character = usart_receive();

        /*
         * Send the received character back to the computer.
         */
        usart_send(received_character);
    }

    return 0;
}