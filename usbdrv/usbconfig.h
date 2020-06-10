#ifndef __USBCONFIG_H__
#define __USBCONFIG_H__

#include "usbconfig-prototype.h"

// Changes from the defaults in usbconfig-prototype.h follow

#undef USB_CFG_HAVE_INTRIN_ENDPOINT
#define USB_CFG_HAVE_INTRIN_ENDPOINT 1
#undef USB_CFG_DEVICE_ID
#define USB_CFG_DEVICE_ID 0xdc, 0x27

#undef USB_CFG_VENDOR_NAME
#undef USB_CFG_VENDOR_NAME_LEN
#undef USB_CFG_DEVICE_NAME
#undef USB_CFG_DEVICE_NAME_LEN

#define USB_CFG_SERIAL_NUMBER   's', 'c', 'h', 'e', 'm', 'e', 's', 'c', 'a', 'p', 'e', '.', 'c', 'o', 'm', ':', '1'
#define USB_CFG_SERIAL_NUMBER_LEN   17

#undef USB_CFG_DEVICE_CLASS
#define USB_CFG_DEVICE_CLASS 0
#undef USB_CFG_INTERFACE_CLASS
#define USB_CFG_INTERFACE_CLASS 3

#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    42

#endif
