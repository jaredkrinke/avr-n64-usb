#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include "avr-n64.h"
#include "usbdrv/usbdrv.h"

#define CLAMP(x, min, max)  (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))
#define CLAMP_AXIS(x)       (CLAMP((x), -70, 70))

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] =
{
    0x05, 0x01,     // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,     // USAGE (Game Pad)
    0xa1, 0x01,     // COLLECTION (Application)
    0x09, 0x01,     //   USAGE (Pointer)
    0xa1, 0x00,     //   COLLECTION (Physical)
    0x09, 0x30,     //     USAGE (X)
    0x09, 0x31,     //     USAGE (Y)
    0x15, 0xb8,     //   LOGICAL_MINIMUM (-70)
    0x25, 0x48,     //   LOGICAL_MAXIMUM (70)
    0x75, 0x08,     //   REPORT_SIZE (8)
    0x95, 0x02,     //   REPORT_COUNT (2)
    0x81, 0x02,     //   INPUT (Data,Var,Abs)
    0xc0,           // END_COLLECTION
    0x05, 0x09,     // USAGE_PAGE (Button)
    0x19, 0x01,     //   USAGE_MINIMUM (Button 1)
    0x29, 0x08,     //   USAGE_MAXIMUM (Button 8)
    0x15, 0x00,     //   LOGICAL_MINIMUM (0)
    0x25, 0x01,     //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,     // REPORT_SIZE (1)
    0x95, 0x08,     // REPORT_COUNT (8)
    0x81, 0x02,     // INPUT (Data,Var,Abs)
    0xc0            // END_COLLECTION
};

static uint8_t report[3];

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t* rq = (usbRequest_t*)data;
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
    {
        switch (rq->bRequest)
        {
            case USBRQ_HID_GET_REPORT:
                usbMsgPtr = (usbMsgPtr_t)&report;
                return sizeof(report);

            default:
                return 0;
        }
    }

    return 0;
}

int main()
{
    // Status
    DDRD |= _BV(7);

    usbInit();
    sei();

    while (1)
    {
        usbPoll();
        if (usbInterruptIsReady())
        {
            // Get controller state
            n64_controller_state_t state;
            cli();
            n64_controller_get_state(&state);
            sei();

            report[0] = (uint8_t)CLAMP_AXIS(state.joystick_horizontal);
            report[1] = (uint8_t)CLAMP_AXIS(-state.joystick_vertical); // down is positive
            report[2] = (state.a ? _BV(0) : 0)
                | (state.b ? _BV(1) : 0)
                | (state.z ? _BV(2) : 0)
                | (state.start ? _BV(3) : 0)
                | (state.l ? _BV(4) : 0)
                | (state.r ? _BV(5) : 0);

            PORTD ^= _BV(7);
            usbSetInterrupt(report, sizeof(report));
        }
    }
}
