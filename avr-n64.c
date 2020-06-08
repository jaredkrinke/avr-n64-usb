// NOTE: This implementation assumes >= 4 MHz clock speed (F_CPU symbol) and optimized output (including loop unrolling, i.e. -funroll-loops for GCC)
#define F_CPU 4000000UL // 4 MHz clock speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

// Controller interface
typedef struct {
    signed char joystick_horizontal;
    signed char joystick_vertical;

    unsigned a: 1;
    unsigned b: 1;
    unsigned z: 1;
    unsigned start: 1;
    unsigned up: 1;
    unsigned down: 1;
    unsigned left: 1;
    unsigned right: 1;
    unsigned reset: 1;
    unsigned reserved: 1;
    unsigned l: 1;
    unsigned r: 1;
    unsigned c_up: 1;
    unsigned c_down: 1;
    unsigned c_left: 1;
    unsigned c_right: 1;
} n64_controller_state_t;

void n64_controller_get_state(n64_controller_state_t* state);

// "Get state" implementation
#define _N64_MODIFY_BIT(p, b, v) (p) = ((p) & ~_BV(b)) | ((v) ? _BV(b) : 0)
#define _N64_NOP() asm("nop;\n");

#define _N64_MESSAGE_GET_STATE    0x01

// Wait for next (microsecond) tick; call after a 2 cycle instruction
inline void _n64_wait_for_next_tick()
{
    for (int i = 0; i < ((F_CPU / 1000000) - 2); i++)
    {
        _N64_NOP();
    }
}

// A short wait, just to ensure pin voltage has changed (~0.25 microseconds)
inline void _n64_wait_short()
{
    for (int i = 0; i < (((F_CPU / 2500000) > 0) ? (F_CPU / 2500000) : 1); i++)
    {
        _N64_NOP();
    }
}

// Skip an entire (microsecond) tick
inline void _n64_skip_tick()
{
    _N64_NOP();
    _N64_NOP();
    _n64_wait_for_next_tick();
}

// Write a single bit to the data line
inline void _n64_write_bit(unsigned char bit)
{
    // Drive low for 1 tick
    _N64_MODIFY_BIT(DDRB, 0, 1); // sbi (2 cycles)
    _n64_wait_for_next_tick();

    // Drive data value for 2 ticks
    _N64_MODIFY_BIT(DDRB, 0, bit == 0);
    _n64_wait_for_next_tick();
    _N64_MODIFY_BIT(DDRB, 0, bit == 0);
    _n64_wait_for_next_tick();

    // Drive high for 1 tick
    _N64_MODIFY_BIT(DDRB, 0, 0);
    _n64_wait_for_next_tick();
}

// Write the host stop bit to the data line to end a message
inline void _n64_write_stop_bit()
{
    // Low for 1 tick, high for 2 ticks
    _N64_MODIFY_BIT(DDRB, 0, 1);
    _n64_wait_for_next_tick();
    _N64_MODIFY_BIT(DDRB, 0, 0);
    _n64_wait_for_next_tick();
    _N64_MODIFY_BIT(DDRB, 0, 0);
}

inline void _n64_send_message(unsigned char message)
{
    _n64_write_bit((message & (1 << 7)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 6)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 5)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 4)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 3)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 2)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 1)) == 0 ? 0 : 1);
    _n64_write_bit((message & (1 << 0)) == 0 ? 0 : 1);
    _n64_write_stop_bit();
}

// Read a bit from the data line
inline void _n64_read_bit(int n, unsigned char* value)
{
    // Low for 1 tick
    loop_until_bit_is_clear(PINB, 0);
    _n64_wait_for_next_tick();

    // Value for 2 ticks
    _n64_wait_short();
    _N64_MODIFY_BIT(*value, n, bit_is_set(PINB, 0));

    // High for 1 tick
    loop_until_bit_is_set(PINB, 0);
}

// Read a byte from the data line (most significant bit first)
inline void _n64_read_byte(unsigned char* value)
{
    _n64_read_bit(7, value);
    _n64_read_bit(6, value);
    _n64_read_bit(5, value);
    _n64_read_bit(4, value);
    _n64_read_bit(3, value);
    _n64_read_bit(2, value);
    _n64_read_bit(1, value);
    _n64_read_bit(0, value);
}

void n64_controller_get_state(n64_controller_state_t* state)
{
    PORTB = 0x00; // NEVER modify PORTB! We must only drive the line low and never high.
    DDRB = 0x00;

    _n64_send_message(_N64_MESSAGE_GET_STATE);

    unsigned char b1 = 0;
    unsigned char b2 = 0;
    unsigned char jx = 0;
    unsigned char jy = 0;

    _n64_read_byte(&b1);
    _n64_read_byte(&b2);
    _n64_read_byte(&jx);
    _n64_read_byte(&jy);

    state->a = bit_is_set(b1, 7) ? 1 : 0;
    state->b = bit_is_set(b1, 6) ? 1 : 0;
    state->z = bit_is_set(b1, 5) ? 1 : 0;
    state->start = bit_is_set(b1, 4) ? 1 : 0;
    state->up = bit_is_set(b1, 3) ? 1 : 0;
    state->down = bit_is_set(b1, 2) ? 1 : 0;
    state->left = bit_is_set(b1, 1) ? 1 : 0;
    state->right = bit_is_set(b1, 0) ? 1 : 0;

    state->reset = bit_is_set(b2, 7) ? 1 : 0;
    state->reserved = bit_is_set(b2, 6) ? 1 : 0;
    state->l = bit_is_set(b2, 5) ? 1 : 0;
    state->r = bit_is_set(b2, 4) ? 1 : 0;
    state->c_up = bit_is_set(b2, 3) ? 1 : 0;
    state->c_down = bit_is_set(b2, 2) ? 1 : 0;
    state->c_left = bit_is_set(b2, 1) ? 1 : 0;
    state->c_right = bit_is_set(b2, 0) ? 1 : 0;

    state->joystick_horizontal = (signed char)jx;
    state->joystick_vertical = (signed char)jy;
}

int main()
{
    // Port D is for output to LEDs
    DDRD = 0xff;

    // Short delay on boot
    _delay_ms(5);

    while (1)
    {
        // Get controller state
        n64_controller_state_t state;
        n64_controller_get_state(&state);

        // Light up LEDs
        PORTD = ((state.joystick_vertical > 30) ? _BV(0) : 0)
            | ((state.joystick_vertical < -30) ? _BV(3) : 0)
            | ((state.joystick_horizontal < -30) ? _BV(1) : 0)
            | ((state.joystick_horizontal > 30) ? _BV(2) : 0)
            | ((state.b) ? _BV(7) : 0);

        _delay_ms(50);
    }
}
