#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "avr-n64.h"

int main()
{
    // Port D is for output to LEDs
    DDRD = 0xff;

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
