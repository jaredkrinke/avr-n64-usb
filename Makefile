PROJECT = BlinkTest
MCU = atmega32
TARGET = $(PROJECT).elf
CC = avr-gcc

COMMON = -mmcu=$(MCU)
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map

HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

OBJECTS = BlinkTest.o 
LINKONLYOBJECTS = 

all: $(TARGET) $(PROJECT).hex

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@
