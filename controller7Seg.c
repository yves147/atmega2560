#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define _DEBUG
#define UNICODE
#define _UNICODE

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef struct Segzahl
{
    uint8_t _d1;
    uint8_t _d2;
    uint8_t _d3;
    uint8_t _d4;
} Segzahl;

uint8_t numb(uint8_t d10th)
{
    if (d10th == 0)
        return 119;
    if (d10th == 1)
        return 20;
    if (d10th == 2)
        return 91;
    if (d10th == 3)
        return 94;
    if (d10th == 4)
        return 60;
    if (d10th == 5)
        return 110;
    if (d10th == 6)
        return 111;
    if (d10th == 7)
        return 84;
    if (d10th == 8)
        return 127;
    if (d10th == 9)
        return 126;
}

struct Segzahl
seg(int nummer)
{
    uint8_t dig1000 = (uint8_t)floor((nummer / 1000) % 10);
    uint8_t dig100 = (uint8_t)floor((nummer / 100) % 10);
    uint8_t dig10 = (uint8_t)floor((nummer / 10) % 10);
    uint8_t dig1 = (uint8_t)floor((nummer / 1) % 10);

    Segzahl segzahl = {numb(dig1000), numb(dig100), numb(dig10), numb(dig1)};

    return segzahl;
}

uint8_t itr = 0;

void display(uint8_t szahl)
{
    ++itr;
    Segzahl segzahl = seg(szahl);
    if (itr == 1)
    {
        PORTC = 0x01 | 0x02 | 0x04;
        PORTA = segzahl._d4;
    }
    else if (itr == 2)
    {
        PORTC = 0x01 | 0x02 | 0x08;
        PORTA = segzahl._d3;
    }
    else if (itr == 3)
    {
        PORTC = 0x01 | 0x04 | 0x08;
        PORTA = segzahl._d2;
    }
    else if (itr == 4)
    {
        PORTC = 0x02 | 0x04 | 0x08;
        PORTA = segzahl._d1;
    }
    if (itr == 4)
        itr = 0;
}

void display_init()
{
    DDRA = 0xFF;
    DDRB = _BV(PB4);
    DDRC = 0x0F;

    PORTD = _BV(PORTD0);
}

void adc_init(void)
{
    ADMUX = (1 << REFS0);

    ADCSRA = (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }

    (void)ADCW;
}

uint16_t adc_single(uint8_t channel)
{
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    return ADCW;
}

volatile uint8_t currentX = 0;
volatile uint8_t currentValue = 0;
volatile uint8_t overwriteValue = 0;

void triggerSignalLight()
{
    PORTB |= (1 << PORTB7);
    _delay_ms(500);
    PORTB &= ~(1 << PORTB7);
}

int main(void)
{
    display_init();

    DDRB |= (1 << PORTB7);

    EIMSK |= _BV(INT4);
    EICRB |= _BV(ISC40);

    adc_init();

    while (1)
    {
        if (adc_single(0) > 896 && currentX != 0)
        {
            currentX--;
            triggerSignalLight();
        }
        else if (adc_single(0) < 128 && currentX != 3)
        {
            currentX++;
            triggerSignalLight();
        }
        if (adc_single(1) > 896 && ((currentValue / ((int)pow(10, currentX))) % 10) != 9)
        {
            currentValue += (int)pow(10, currentX);
            triggerSignalLight();
        }
        else if (adc_single(1) < 128 && ((currentValue / ((int)pow(10, currentX))) % 10) != 0)
        {
            currentValue -= (int)pow(10, currentX);
            triggerSignalLight();
        }
        display(currentValue);
    }

    return 0;
}

ISR(INT4_vect, ISR_BLOCK)
{
    if (((PINE & _BV(PORTE4)) == 0))
    {
        currentValue = 0;
        currentX = 0;
        triggerSignalLight();
    }
}
