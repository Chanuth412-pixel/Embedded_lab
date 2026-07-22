#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

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

uint8_t EEPROMread(uint16_t address)
{
    while (EECR & (1 << EEPE))
    {
    }

    EEAR = address;
    EECR |= (1 << EERE);

    return EEDR;
}