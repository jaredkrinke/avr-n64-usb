# avr-n64

Read state from a Nintendo 64 controller using an ATmega32 microcontroller and light up LEDs.

![Demo](../data/demo.gif)

## Library notes
The communication library is implemented a single header file: `avr-n64.h`. To get the state of a controller, call this function:

```c
void n64_controller_get_state(n64_controller_state_t* state);
```

The library assumes the controller data line is connected to port B, bit 0.

Note: while the code is written in C, it assumes specific optimized compiler output (e.g. loop unrolling). See `CFLAGS` in `Makefile` for more detail.

## Protocol notes
The controller uses 3 wires: two for power, one for half-duplex serial communication based on a 1 MHz clock with roughly 31 kbps of bandwidth.

### Connector pin out
Facing the "plug" side of the controller connector, the pins are (left to right): ground, data, 3.3 volts.

### Data line
The data line is an open collector with (as measured) an 800 ohm built-in pull-up resistor (which keeps the line high, at 3.3 volts). Communication is initiated by connecting to ground (driving voltage to zero), and eventually disconnecting and letting the line go high again. Note: the host should never drive this line high.

### Encoding
Bits are sent (always most significant bit first) using the following encoding:

* 1 microsecond low
* 2 microseconds at either low (for a zero) or high (for a one)
* 1 microsecond high

Communication is terminated by 3 microsecond stop bit:

* 1 microsecond low, 2 microseconds high for the host
* 2 microseconds low, 1 microsecond high for the controller

### Message passing
The host initiates communication by sending 1 byte messages. The 0x01 message polls the controller for state. The controller responds with 4 bytes (plus a stop bit):

* A, B, Z, Start, Up, Down, Left, Right
* Reset, Reserved, L, R, C Up, C Down, C Left, C Right
* Analog joystick horizontal position (signed two's complement integer)
* Analog joystick vertical position

## Implementation notes
This implementation uses an ATmega32 (DIP package) running at >= 4 MHz (tested using the internal oscillator), with a power supply of 5 volts (tested with a repurposed cell phone charger). A 3.3 volt linear regulator supplies power to the controller.

### Timing
This setup uses a software implementation of the protocol ("bit banging"). The code assumes specific optimized compiler output (e.g. loop unrolling and using "cbi" and "sbi" instructions for manipulating the data pin), and uses "nop" instructions to ensure a 1 MHz data clock.

### Open collector output pin
The ATmega32 doesn't have an obvious "open collector output" mode, but one can be implemented by:

* Initially set PORTxn to 0x00
* To write a zero, set DDRxn to 1 (this drives the line low)
* To write a one, set DDRxn to 0 (this puts the pin in a disconnected/tri-state/high-impedance state)
* While DDRxn is 0, read PINxn for input (3.3 volts is above the sensitivity of the input pin)

## External resources
These resources go into more detail about the controller protocol:

- http://www.qwertymodo.com/hardware-projects/n64/n64-controller
- https://kthompson.gitlab.io/2016/07/26/n64-controller-protocol.html
- http://www.mixdown.ca/n64dev/
- https://os.mbed.com/users/fomartin/notebook/n64-controller-interface/
- https://www.instructables.com/id/Use-an-Arduino-with-an-N64-controller/
