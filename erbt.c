#define F_CPU 16000000UL
#define _DEBUG
#define UNICODE
#define _UNICODE

#define __AVR_ATmega2560__

#include <avr/io.h>
#include <util/delay.h>

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#pragma region ERBT
// Easy Registry Binary Toolchain

#define ert_d_base 2
#define erbt_POW_U8_T _BV

// Manual High
void erbt_M_HIGH(volatile uint8_t *registry, uint8_t comp)
{
	*registry |= comp;
}

// Manual Low
void erbt_M_LOW(volatile uint8_t *registry, uint8_t comp)
{
	*registry &= ~(comp);
}

// Single Entry High
void erbt_SE_HIGH(volatile uint8_t *registry, uint8_t bit)
{
	*registry |= erbt_POW_U8_T(bit);
}

// Single Entry Low
void erbt_SE_LOW(volatile uint8_t *registry, uint8_t bit)
{
	*registry &= ~(erbt_POW_U8_T(bit));
}

// Single Check High
uint8_t erbt_SC_HIGH(volatile uint8_t *registry, uint8_t bit)
{
	return *registry & erbt_POW_U8_T(bit);
}

// Entry Combinator 2 bits
uint8_t erbt_EC(uint8_t b1, uint8_t b2)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2);
}

// Entry Combinator 3 bits
uint8_t erbt_EC_3(uint8_t b1, uint8_t b2, uint8_t b3)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2) + erbt_POW_U8_T(b3);
}

// Entry Combinator 4 bits
uint8_t erbt_EC_4(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2) + erbt_POW_U8_T(b3) + erbt_POW_U8_T(b4);
}

// Entry Combinator 5 bits
uint8_t erbt_EC_5(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2) + erbt_POW_U8_T(b3) + erbt_POW_U8_T(b4) + erbt_POW_U8_T(b5);
}

// Entry Combinator 6 bits
uint8_t erbt_EC_6(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2) + erbt_POW_U8_T(b3) + erbt_POW_U8_T(b4) + erbt_POW_U8_T(b5) + erbt_POW_U8_T(b6);
}

// Entry Combinator 7 bits
uint8_t erbt_EC_7(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
	return erbt_POW_U8_T(b1) + erbt_POW_U8_T(b2) + erbt_POW_U8_T(b3) + erbt_POW_U8_T(b4) + erbt_POW_U8_T(b5) + erbt_POW_U8_T(b6) + erbt_POW_U8_T(b7);
}

// Entry Combinator ROW
uint8_t erbt_EC_ROW(uint8_t bs, uint8_t be)
{
	uint8_t a;
	for (uint8_t j = bs; j < be; j++)
		a += erbt_POW_U8_T(j);
	return a;
}

// Entry Combinator eXcept ROW
uint8_t erbt_EC_XROW(uint8_t bs, uint8_t be)
{
	uint8_t a;
	for (uint8_t j = 0; j < 8; j++)
	{
		if (j >= bs || j <= be)
			continue;
		a += erbt_POW_U8_T(j);
	}
	return a;
}
#pragma endregion ERBT

int main(void)
{
	erbt_SE_HIGH(&DDRB, PORTB7);

	while (1)
	{
		erbt_SE_HIGH(&PORTB, PORTB7);
		_delay_ms(2000);
		erbt_SE_LOW(&PORTB, PORTB7);
		_delay_ms(5000);
	}
}