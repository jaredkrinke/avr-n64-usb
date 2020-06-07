#define F_CPU 12000000UL // 12 MHz clock speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

/*
// Assembly version -- FIX THIS -- it's backwards
//#define N64_CONTROL_GET_STATE 0x01

// Write (1 clock cycle, then 11 nops)
#define N64_WAIT_10		asm("nop;\n nop;\n nop;\n nop;\n nop;\n nop;\n nop;\n nop;\n nop;\n nop;\n");
#define N64_DRIVE_LOW	asm("cbi 0x17, 0;\n"); N64_WAIT_10 // DDRB0
#define N64_FLOAT 		asm("sbi 0x17, 0;\n"); N64_WAIT_10 // DDRB0

#define N64_WRITE_ZERO	N64_DRIVE_LOW N64_DRIVE_LOW N64_DRIVE_LOW N64_FLOAT
#define N64_WRITE_ONE	N64_DRIVE_LOW N64_FLOAT N64_FLOAT N64_FLOAT
#define N64_WRITE_STOP	N64_WRITE_ONE

#define N64_REQUEST_STATE()	N64_WRITE_ONE N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_ZERO N64_WRITE_STOP

// Wait for low (2 + 10 cycles once low)
#define N64_WAIT_FOR_LOW()	asm("sbic 0x16, 0;\n rjmp .-4;\n"); N64_WAIT_10 // PINB0
#define N64_READ_PREPARE()	asm("nop;\n");

unsigned char n64GetState()
{
	N64_REQUEST_STATE();
	N64_WAIT_FOR_LOW();
	N64_READ_PREPARE();

	// Only read the first button (2 cycles)
	unsigned char value = bit_is_set(PINB, 0) ? 1 : 0;
	return value;
}*/

// C version
#define MODIFY_BIT(p, b, v)	(p) = ((p) & ~_BV(b)) | (((v) == 0) ? 0 : _BV(b))
#define _NOP() asm("nop;\n");

inline void wait5()
{
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	_NOP();
}

inline void wait10()
{
	wait5();
	wait5();
}

inline void wait12()
{
	wait10();
	_NOP();
	_NOP();
}

inline void writeBit(unsigned char bit)
{
	MODIFY_BIT(DDRB, 0, 1);
	wait10();
	MODIFY_BIT(DDRB, 0, (bit == 0) ? 1 : 0);
	wait10();
	MODIFY_BIT(DDRB, 0, (bit == 0) ? 1 : 0);
	wait10();
	MODIFY_BIT(DDRB, 0, 0);
	wait10();
}

inline void writeStopBit()
{
	MODIFY_BIT(DDRB, 0, 1);
	wait10();
	MODIFY_BIT(DDRB, 0, 0);
	wait10();
	MODIFY_BIT(DDRB, 0, 0);
}

inline void skipReadBit()
{
	loop_until_bit_is_clear(PINB, 0);
	wait10();
	wait10();
	loop_until_bit_is_set(PINB, 0);
}

inline void skipReadByte()
{
	skipReadBit();
	skipReadBit();
	skipReadBit();
	skipReadBit();
	skipReadBit();
	skipReadBit();
	skipReadBit();
	skipReadBit();
}

inline void readAndOutputBit(unsigned char bit)
{
	loop_until_bit_is_clear(PINB, 0);
	wait10();
	wait10();
	MODIFY_BIT(PORTD, bit, bit_is_set(PINB, 0));
	loop_until_bit_is_set(PINB, 0);
}

void n64GetStateC()
{
	// Sned "poll" message
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(1);

	writeStopBit();

	// Response
	skipReadBit();			// A
	readAndOutputBit(7);	// B
	skipReadBit();			// Z
	skipReadBit();			// Start
	readAndOutputBit(0);	// Up
	readAndOutputBit(3);	// Down
	readAndOutputBit(1);	// Left
	readAndOutputBit(2);	// Right
}

int main()
{
	// NEVER modify PORTB! We must only drive the line low and never high.
	PORTB = 0x00;

	// Port D is for output to LEDs
	DDRD = 0xff;
	while (1)
	{
		n64GetStateC();
		_delay_ms(50);
	}
}
