# Obvious optimization ideas for AVR 8-Bit arch
## Use static access type
```c
#include <avr/iso.h>

uint8_t string[11] = {"hello world"};

// static keyword frees around 12 bytes in program memory
static void USART_TX(uint8_t data);

int main(void)
{
    uint8_t i = 0;
    while (i < 12) {
        USART_TX(string[i++]);
    }
}

void USART_TX(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}
```

## General data type issues

```c
#include <avr/io.h>

int main(void)
{
    // using uint8_t instead of uint16_t obviously frees 2 bytes
    // but also 11 cycles
    uint8_t a = 10;
    do {
        PORTB ^= 0x80;
    } while (--a);
}
```

## Loops are quiet nice - wow
#### would be automated by O3 or higher
```c
#include <avr/io.h>

int main(void)
{
    uint8_t c = 10;
    // instead of pasting the same command 10 times in a row
    // this would free about 50 bytes and 30 cycles
    do {
        PORTB ^= 0x01;
    } while (--c);
}
```

## If-Else fun
If-Else Subbranches significantly increase cycle performance, but also requires up to 17% more program memory in some normal 3-branch scenarios.

## Putting loops together
```c
#include <avr/io.h>

int main(void)
{
    uint8_t i, t = 0;
    uint8_t m[10] = {0};
    // instead of running 2 loops with one setting values to m and the other increasing t
    // this frees up to 66 bytes
    for (i = 0; i < 10; i++) {
        m [i] = ADCH;
        t += m[i];
    }
    UDR0 = t;
}
```

## Constants in program instead of data space
```c
#include <avr/io.h>
#include <avr/pgmspace.h>

// frees up to 32 bytes (20 of them in program space)
uint8_t string[12] PROGMEM = {"world hello!"};
int main(void)
{
    // if data stored in flash is used multiple times, use a temporary variable instead
    // look in <avr/pgmspace.h> for data type specific functions
    UDR0 = pgm_read_byte(&string[10]);
}
```