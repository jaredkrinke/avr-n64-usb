# avr-n64-usb
Convert a Nintendo 64 controller to a USB HID game pad using an ATmega32 microcontroller.

## Implementation notes
The controller is reported as a 4 axis (joystick + d-pad), 10 button USB HID game pad.

![USB dialog](../data/avr-n64-windows.jpg)

### Compilation
This project has only been tested with version vusb-20121206.zip of V-USB, which is available here: https://www.obdev.at/products/vusb/download.html

Unzip the file into the `usbdrv` folder and then update `Makefile` as needed (for clock speed, compiler, flash command, etc.).

Note that if you build on this code, you should update `USB_CFG_SERIAL_NUMBER` (and its length) in `usbdrv\usbconfig.h` (following the guidance in V-USB's `USB-IDs-for-free.txt` file).

### Wiring
The circuit used for testing this project was built around an ATmega32 (DIP) running at 5V, using an external 12 MHz crystal oscillator.

The Nintendo 64 controller is powered at 3.3V using a linear regulator. The N64 controller data line is connected to PORTB0 (open collector output).

The USB D+ and D- lines (connected to PORTD2/INT0 and PORTD4, respectively) are run at 3.3V using zener diodes and resistors (including a pull-up resistor on D-) as suggested by [V-USB's sample circuit](https://www.obdev.at/products/vusb/index.html).

#### Schematic
* [Schematic SVG](../../raw/data/schematic.svg)
* [Schematic PNG](../../raw/data/schematic.png)
* [Source (KiCad schematic file)](../../raw/data/avr-n64-usb.sch)

#### Picture
![Breadboard](../data/avr-n64-usb-breadboard.jpg)

## External resources
This project is built on the following libraries:

- N64: https://github.com/jaredkrinke/avr-n64
- USB: https://www.obdev.at/products/vusb/index.html
