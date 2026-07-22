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

char caesar_encrypt_character(char character)
{
    if ((character >= 'A') && (character <= 'Z'))
    {
        character = (char)(
            'A' + ((character - 'A' + CAESAR_KEY) % 26)
        );
    }
    else if ((character >= 'a') && (character <= 'z'))
    {
        character = (char)(
            'a' + ((character - 'a' + CAESAR_KEY) % 26)
        );
    }

    return character;
}

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
    usart_send_string("      Caesar Cipher - Key 3\r\n");

    while (1)
    {
        sentence_length = 0;

        usart_send_string("\r\nEnter a sentence: ");

        while (1)
        {
            received_character = usart_receive();

            if ((received_character == '\r') ||
                (received_character == '\n'))
            {
                if (sentence_length == 0)
                {
                    continue;
                }

                break;
            }

            if ((received_character == '\b') ||
                (received_character == 127))
            {
                if (sentence_length > 0)
                {
                    sentence_length--;

                    usart_send('\b');
                    usart_send(' ');
                    usart_send('\b');
                }

                continue;
            }

            if (sentence_length < (MAX_SENTENCE_LENGTH - 1))
            {
                sentence[sentence_length] = received_character;
                sentence_length++;

                usart_send(received_character);
            }
        }

        sentence[sentence_length] = '\0';

        caesar_encrypt_sentence(sentence);

        usart_send_string("\r\nEncrypted sentence: ");
        usart_send_string(sentence);
        usart_send_string("\r\n");
    }

    return 0;
}