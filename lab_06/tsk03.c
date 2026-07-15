/*
 * CO321 Lab 6 - Exercise 3
 * Caesar cipher using USART
 *
 * Microcontroller: ATmega328P
 * Clock frequency: 16 MHz
 * Baud rate: 9600
 * Frame format: 8 data bits, no parity, 1 stop bit
 *
 * Caesar cipher key: 3
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>

#define USART_BAUD_RATE 9600UL
#define CAESAR_KEY 3
#define MAX_SENTENCE_LENGTH 100

#define USART_UBRR_VALUE \
    ((F_CPU / (16UL * USART_BAUD_RATE)) - 1UL)

/*
 * Initialize USART0 for 9600 baud, 8N1.
 */
void usart_init(void)
{
    /*
     * Normal asynchronous mode.
     * U2X0 = 0
     */
    UCSR0A = 0x00;

    /*
     * Load baud-rate value.
     */
    UBRR0H = (uint8_t)(USART_UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(USART_UBRR_VALUE);

    /*
     * Enable USART transmitter and receiver.
     */
    UCSR0B = (1 << RXEN0) |
             (1 << TXEN0);

    /*
     * Asynchronous mode
     * No parity
     * One stop bit
     * Eight data bits
     */
    UCSR0C = (1 << UCSZ01) |
             (1 << UCSZ00);
}

/*
 * Send one character.
 */
void usart_send(char character)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
        /* Poll UDRE0 until the transmit register is ready. */
    }

    UDR0 = (uint8_t)character;
}

/*
 * Receive one character.
 */
char usart_receive(void)
{
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
        /* Poll RXC0 until a complete character is received. */
    }

    return (char)UDR0;
}

/*
 * Send a null-terminated string.
 */
void usart_send_string(const char *text)
{
    while (*text != '\0')
    {
        usart_send(*text);
        text++;
    }
}

/*
 * Encrypt one character using Caesar cipher key 3.
 */
char caesar_encrypt_character(char character)
{
    /*
     * Encrypt uppercase alphabetic characters.
     */
    if ((character >= 'A') && (character <= 'Z'))
    {
        character = (char)(
            'A' + ((character - 'A' + CAESAR_KEY) % 26)
        );
    }
    /*
     * Encrypt lowercase alphabetic characters.
     */
    else if ((character >= 'a') && (character <= 'z'))
    {
        character = (char)(
            'a' + ((character - 'a' + CAESAR_KEY) % 26)
        );
    }
    /*
     * Spaces, numbers and punctuation remain unchanged.
     */

    return character;
}

/*
 * Encrypt an entire null-terminated sentence.
 */
void caesar_encrypt_sentence(char *sentence)
{
    uint8_t index = 0;

    while (sentence[index] != '\0')
    {
        sentence[index] =
            caesar_encrypt_character(sentence[index]);

        index++;
    }
}

int main(void)
{
    char sentence[MAX_SENTENCE_LENGTH];
    char received_character;
    uint8_t sentence_length;

    usart_init();

    usart_send_string("\r\n");
    usart_send_string("========================================\r\n");
    usart_send_string("      Caesar Cipher - Key 3\r\n");
    usart_send_string("========================================\r\n");
    usart_send_string("Only alphabetic characters are encrypted.\r\n");
    usart_send_string("Numbers and punctuation remain unchanged.\r\n");

    while (1)
    {
        sentence_length = 0;

        usart_send_string("\r\nEnter a sentence: ");

        /*
         * Receive characters until carriage return or line feed.
         */
        while (1)
        {
            received_character = usart_receive();

            /*
             * End the sentence when carriage return or line feed
             * is received.
             *
             * Accepting both makes the program compatible with
             * different serial terminals.
             */
            if ((received_character == '\r') ||
                (received_character == '\n'))
            {
                /*
                 * Ignore an empty CR or LF. This is useful when
                 * the terminal sends CR and LF together.
                 */
                if (sentence_length == 0)
                {
                    continue;
                }

                break;
            }

            /*
             * Handle Backspace or Delete.
             */
            if ((received_character == '\b') ||
                (received_character == 127))
            {
                if (sentence_length > 0)
                {
                    sentence_length--;

                    /*
                     * Remove the previous character from the terminal.
                     */
                    usart_send('\b');
                    usart_send(' ');
                    usart_send('\b');
                }

                continue;
            }

            /*
             * Store the character if space is available.
             *
             * One position is reserved for the null terminator.
             */
            if (sentence_length < (MAX_SENTENCE_LENGTH - 1))
            {
                sentence[sentence_length] = received_character;
                sentence_length++;

                /*
                 * Echo the received character so the user can see it.
                 */
                usart_send(received_character);
            }
        }

        /*
         * Terminate the sentence as a C string.
         */
        sentence[sentence_length] = '\0';

        /*
         * Encrypt the sentence in the same buffer.
         */
        caesar_encrypt_sentence(sentence);

        /*
         * Return the encrypted result.
         */
        usart_send_string("\r\nEncrypted sentence: ");
        usart_send_string(sentence);
        usart_send_string("\r\n");
    }

    return 0;
}