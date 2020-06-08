// NOTE: This implementation assumes >= 4 MHz clock speed and optimized output (including loop unrolling)
#define F_CPU 4000000UL // 4 MHz clock speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#define MODIFY_BIT(p, b, v) (p) = ((p) & ~_BV(b)) | ((v) ? _BV(b) : 0)
#define _NOP() asm("nop;\n");

// "Poll" request and response
#define N64_MESSAGE_POLL    0x01

#define N64_BUTTON_PRESSED(state, byte, bit) ((((state).byte) & _BV(bit)) != 0)
#define N64_BUTTON_A(state)     N64_BUTTON_PRESSED(state, buttons1, 7)
#define N64_BUTTON_B(state)     N64_BUTTON_PRESSED(state, buttons1, 6)
#define N64_BUTTON_Z(state)     N64_BUTTON_PRESSED(state, buttons1, 5)
#define N64_BUTTON_START(state) N64_BUTTON_PRESSED(state, buttons1, 4)
#define N64_BUTTON_UP(state)    N64_BUTTON_PRESSED(state, buttons1, 3)
#define N64_BUTTON_DOWN(state)  N64_BUTTON_PRESSED(state, buttons1, 2)
#define N64_BUTTON_LEFT(state)  N64_BUTTON_PRESSED(state, buttons1, 1)
#define N64_BUTTON_RIGHT(state) N64_BUTTON_PRESSED(state, buttons1, 0)
#define N64_BUTTON_RESET(state) N64_BUTTON_PRESSED(state, buttons2, 7)
#define N64_BUTTON_RESERVED(state) N64_BUTTON_PRESSED(state, buttons2, 6)
#define N64_BUTTON_L(state) N64_BUTTON_PRESSED(state, buttons2, 5)
#define N64_BUTTON_R(state) N64_BUTTON_PRESSED(state, buttons2, 4)
#define N64_BUTTON_CUP(state) N64_BUTTON_PRESSED(state, buttons2, 3)
#define N64_BUTTON_CDOWN(state) N64_BUTTON_PRESSED(state, buttons2, 2)
#define N64_BUTTON_CLEFT(state) N64_BUTTON_PRESSED(state, buttons2, 1)
#define N64_BUTTON_CRIGHT(state) N64_BUTTON_PRESSED(state, buttons2, 0)

typedef struct {
    unsigned char buttons1;
    unsigned char buttons2;
    char joystickHorizontal;
    char joystickVertical;
} n64ControllerState;

// Wait for next (microsecond) tick; call after a 2 cycle instruction
inline void waitForNextTick()
{
    for (int i = 0; i < ((F_CPU / 1000000) - 2); i++)
    {
        _NOP();
    }
}

// A short wait, just to ensure pin voltage has changed (~0.25 microseconds)
inline void waitShort()
{
    for (int i = 0; i < (((F_CPU / 2500000) > 0) ? (F_CPU / 2500000) : 1); i++)
    {
        _NOP();
    }
}

// Skip an entire (microsecond) tick
inline void skipTick()
{
    _NOP();
    _NOP();
    waitForNextTick();
}

// Write a single bit to the data line
inline void writeBit(unsigned char bit)
{
    // Drive low for 1 tick
    MODIFY_BIT(DDRB, 0, 1); // sbi (2 cycles)
    waitForNextTick();

    // Drive data value for 2 ticks
    MODIFY_BIT(DDRB, 0, bit == 0);
    waitForNextTick();
    MODIFY_BIT(DDRB, 0, bit == 0);
    waitForNextTick();

    // Drive high for 1 tick
    MODIFY_BIT(DDRB, 0, 0);
    waitForNextTick();
}

// Write the host stop bit to the data line to end a message
inline void writeStopBit()
{
    // Low for 1 tick, high for 2 ticks
    MODIFY_BIT(DDRB, 0, 1);
    waitForNextTick();
    MODIFY_BIT(DDRB, 0, 0);
    waitForNextTick();
    MODIFY_BIT(DDRB, 0, 0);
}

inline void sendMessage(unsigned char message)
{
    writeBit((message & (1 << 7)) == 0 ? 0 : 1);
    writeBit((message & (1 << 6)) == 0 ? 0 : 1);
    writeBit((message & (1 << 5)) == 0 ? 0 : 1);
    writeBit((message & (1 << 4)) == 0 ? 0 : 1);
    writeBit((message & (1 << 3)) == 0 ? 0 : 1);
    writeBit((message & (1 << 2)) == 0 ? 0 : 1);
    writeBit((message & (1 << 1)) == 0 ? 0 : 1);
    writeBit((message & (1 << 0)) == 0 ? 0 : 1);
    writeStopBit();
}

// Read a bit from the data line
inline void readBit(int n, unsigned char* value)
{
    // Low for 1 tick
    loop_until_bit_is_clear(PINB, 0);
    waitForNextTick();

    // Value for 2 ticks
    waitShort();
    MODIFY_BIT(*value, n, bit_is_set(PINB, 0));

    // High for 1 tick
    loop_until_bit_is_set(PINB, 0);
}

// Read a byte from the data line (most significant bit first)
inline void readByte(unsigned char* value)
{
    readBit(7, value);
    readBit(6, value);
    readBit(5, value);
    readBit(4, value);
    readBit(3, value);
    readBit(2, value);
    readBit(1, value);
    readBit(0, value);
}

void n64ControllerPoll(n64ControllerState* state)
{
    sendMessage(N64_MESSAGE_POLL);

    unsigned char b1 = 0;
    unsigned char b2 = 0;
    unsigned char jx = 0;
    unsigned char jy = 0;

    readByte(&b1);
    readByte(&b2);
    readByte(&jx);
    readByte(&jy);

    state->buttons1 = b1;
    state->buttons2 = b2;
    state->joystickHorizontal = (char)jx;
    state->joystickVertical = (char)jy;
}

int main()
{
    // NEVER modify PORTB! We must only drive the line low and never high.
    PORTB = 0x00;

    // Port D is for output to LEDs
    DDRD = 0xff;

    // Short delay on boot
    _delay_ms(5);

    while (1)
    {
        // Get controller state
        n64ControllerState state;
        n64ControllerPoll(&state);

        // Light up LEDs
        MODIFY_BIT(PORTD, 0, state.joystickVertical > 30);
        MODIFY_BIT(PORTD, 3, state.joystickVertical < -30);
        MODIFY_BIT(PORTD, 1, state.joystickHorizontal < -30);
        MODIFY_BIT(PORTD, 2, state.joystickHorizontal > 30);

        MODIFY_BIT(PORTD, 7, N64_BUTTON_B(state));

        _delay_ms(50);
    }
}
