PROJECT = avr-n64-usb

MCU = atmega32
F_CPU = 12000000

ELF = $(PROJECT).elf
TARGET = $(PROJECT).hex

CC = avr-gcc
COMMON = -mmcu=$(MCU)
CFLAGS = $(COMMON)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -funroll-loops

LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map

HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

OBJECTS = n64-usb.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o
LINKONLYOBJECTS = 

all: $(ELF) $(TARGET)

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(ELF)

$(TARGET): $(ELF)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

dump: $(ELF)
	avr-objdump -z -d $(ELF)

flash: $(TARGET)
	avrdude -c jtag1 -p m32 -P com3 -U flash:w:$(PROJECT).hex
