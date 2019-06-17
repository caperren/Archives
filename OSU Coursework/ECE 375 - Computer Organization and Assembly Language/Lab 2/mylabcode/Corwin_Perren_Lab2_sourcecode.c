#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Generic register bitwise operators
#define REG_SET(reg, pin) (reg |= _BV(pin))
#define REG_CLR(reg, pin) (reg &= ~(_BV(pin)))
#define REG_WRITE(reg, pin, value) ((value == 1) ? REG_SET(reg, pin) : REG_CLR(reg, pin))
#define REG_CHECK(reg, pin) ((reg &= (_BV(pin))) > 0)

// Right motor enable
#define RME_DDR DDRB
#define RME_OUT PORTB
#define RME_IN PINB
#define RME_PIN 4
#define RME_IO_OUT() (REG_SET(RME_DDR, RME_PIN))
#define RME_IO_IN() (REG_CLR(RME_DDR, RME_PIN))
#define RME_SET() (REG_SET(RME_OUT, RME_PIN))
#define RME_CLR() (REG_CLR(RME_OUT, RME_PIN))
#define RME_WRITE(value) (REG_WRITE(RME_OUT, RME_PIN, value))
#define RME_STATE() (REG_CHECK(RME_IN, RME_PIN))

// Right motor drive
#define RMD_DDR DDRB
#define RMD_OUT PORTB
#define RMD_IN PINB
#define RMD_PIN 5
#define RMD_IO_OUT() (REG_SET(RMD_DDR, RMD_PIN))
#define RMD_IO_IN() (REG_CLR(RMD_DDR, RMD_PIN))
#define RMD_SET() (REG_SET(RMD_OUT, RMD_PIN))
#define RMD_CLR() (REG_CLR(RMD_OUT, RMD_PIN))
#define RMD_WRITE(value) (REG_WRITE(RMD_OUT, RMD_PIN, value))
#define RMD_STATE() (REG_CHECK(RMD_IN, RMD_PIN))

// Left motor enable
#define LME_DDR DDRB
#define LME_OUT PORTB
#define LME_IN PINB
#define LME_PIN 7
#define LME_IO_OUT() (REG_SET(LME_DDR, LME_PIN))
#define LME_IO_IN() (REG_CLR(LME_DDR, LME_PIN))
#define LME_SET() (REG_SET(LME_OUT, LME_PIN))
#define LME_CLR() (REG_CLR(LME_OUT, LME_PIN))
#define LME_WRITE(value) (REG_WRITE(LME_OUT, LME_PIN, value))
#define LME_STATE() (REG_CHECK(LME_IN, LME_PIN))

// Left motor drive
#define LMD_DDR DDRB
#define LMD_OUT PORTB
#define LMD_IN PINB
#define LMD_PIN 6
#define LMD_IO_OUT() (REG_SET(LMD_DDR, LMD_PIN))
#define LMD_IO_IN() (REG_CLR(LMD_DDR, LMD_PIN))
#define LMD_SET() (REG_SET(LMD_OUT, LMD_PIN))
#define LMD_CLR() (REG_CLR(LMD_OUT, LMD_PIN))
#define LMD_WRITE(value) (REG_WRITE(LMD_OUT, LMD_PIN, value))
#define LMD_STATE() (REG_CHECK(LMD_IN, LMD_PIN))

// Left whisker
#define LW_DDR DDRD
#define LW_OUT PORTD
#define LW_IN PIND
#define LW_PIN 1
#define LW_IO_OUT() (REG_SET(LW_DDR, LW_PIN))
#define LW_IO_IN() (REG_CLR(LW_DDR, LW_PIN))
#define LW_SET() (REG_SET(LW_OUT, LW_PIN))
#define LW_CLR() (REG_CLR(LW_OUT, LW_PIN))
#define LW_WRITE(value) (REG_WRITE(LW_OUT, LW_PIN, value))
#define LW_STATE() (REG_CHECK(LW_IN, LW_PIN))

// Right whisker
#define RW_DDR DDRD
#define RW_OUT PORTD
#define RW_IN PIND
#define RW_PIN 0
#define RW_IO_OUT() (REG_SET(RW_DDR, RW_PIN))
#define RW_IO_IN() (REG_CLR(RW_DDR, RW_PIN))
#define RW_SET() (REG_SET(RW_OUT, RW_PIN))
#define RW_CLR() (REG_CLR(RW_OUT, RW_PIN))
#define RW_WRITE(value) (REG_WRITE(RW_OUT, RW_PIN, value))
#define RW_STATE() (REG_CHECK(RW_IN, RW_PIN))

const unsigned int backup_time = 1000;  //ms
const unsigned int debounce_time = 200; //ms

int main(void){
    // Set motor enable and direction to outputs
    RME_IO_OUT();
    RMD_IO_OUT();
    LME_IO_OUT();
    LMD_IO_OUT();

    // Set whiskers to inputs and enable pullup resistors
    LW_IO_IN();
    LW_SET();

    RW_IO_IN();
    RW_SET();

    // Enables motors and sets bot to move forward
    RMD_SET();
    LMD_SET();
    RME_CLR();  // Setting to zero enables motors
    LME_CLR();  // Setting to zero enables motors


    // Main program loop
    while (1){
        // Get states of whiskers
        unsigned char lw_pressed = !LW_STATE();
        unsigned char rw_pressed = !RW_STATE();

        // Handle noise from button press
        _delay_ms(debounce_time);

        // Logic if the right whisker is pressed
        if(rw_pressed){
            RMD_CLR();
            LMD_CLR();
            _delay_ms(backup_time);
            RMD_SET();
            _delay_ms(backup_time);
            LMD_SET();

        // Logic if the right whisker is pressed
        }else if(lw_pressed){
            RMD_CLR();
            LMD_CLR();
            _delay_ms(backup_time);
            LMD_SET();
            _delay_ms(backup_time);
            RMD_SET();
        }
    }
}
