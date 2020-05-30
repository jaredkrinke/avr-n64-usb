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
//#define SET_BIT(p, b)		(p) |= _BV(b);
//#define CLEAR_BIT(p, b)		(p) &= ~_BV(b);
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
	PORTD = bit_is_set(PINB, 0) ? 0xff : 0x00;
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

unsigned char n64GetStateC()
{
//	unsigned char firstByte = 0;

	// TODO: Loop that is unrolled
	// Poll
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(0);
	writeBit(1);

	// Reset
	/*writeBit(1);
	writeBit(1);
	writeBit(1);
	writeBit(1);
	writeBit(1);
	writeBit(1);
	writeBit(1);
	writeBit(1);*/

	writeStopBit();

	// Expecting 4 byte response, plus stop bit
	/*skipReadByte();
	skipReadByte();
	skipReadByte();
	skipReadByte();
	skipReadBit();


	return 0;*/

	// Bit 3
	loop_until_bit_is_clear(PINB, 0);
	wait10();
	wait10();

	return bit_is_set(PINB, 0);
}

/*
// Line test
unsigned char getLineState()
{
	DDRB = 0x00;
	return bit_is_set(PINB, 0);
}*/

int main()
{
	// NEVER modify PORTB! We must only drive the line low and never high.
	PORTB = 0x00;

	DDRD = 0xff; // Port D set to output
	while (1)
	{
		//n64GetStateC();

		if (n64GetStateC())
		{
			PORTD = 0xff;
		}
		else
		{
			PORTD = 0x00;
		}

		/*_delay_ms(50);
		PORTD = 0x00;*/

		//_delay_ms(50);
		//PORTD = 0xff;
		//_delay_ms(1000);
		//PORTD = 0x00;

		_delay_ms(50);
	}
}
