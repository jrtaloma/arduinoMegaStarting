PREFIX=.
CC=avr-gcc
CC_OPTS=-Wall --std=gnu99 -DF_CPU=16000000UL  -O3 -funsigned-char -funsigned-bitfields  -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega2560 -I$(PREFIX)/../include

AVRDUDE=avrdude

# com1 = serial port. Use lpt1 to connect to parallel port.
AVRDUDE_PORT = /dev/ttyUSB0    # programmer connected to serial device
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET):i
AVRDUDE_FLAGS = -p m2560 -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 115200
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)
AVRDUDE_FLAGS += -D -q -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf
AVRDUDE_FLAGS += -c wiring

HEADERS=delay.h\
	digio.h\
	eeprom.h\
	encoder.h\
	pins.h\
	pwm.h\
	timer.h\
	uart.h\
	uart1.h\
	uart2.h\

INCLUDES=$(addprefix $(PREFIX)/../include/, $(HEADERS))

OBJS=delay.o\
     digio.o\
     eeprom.o\
     encoder.o\
     pins.o\
     pwm.o\
     timer.o\
     uart.o\
     uart1.o\
     uart2.o\

BINS=digio_test.elf\
	 eeprom_test.elf\
	 encoder_test.elf\
	 pwm_test.elf\
	 timer_test.elf\
	 uart_test.elf\
         uart1_test.elf\
         uart2_test.elf\

.phony:	clean all

all:	$(BINS)

#common objects
%.o:	$(PREFIX)/../src/%.c $(INCLUDES)
	$(CC) $(CC_OPTS) -c  $<

%.o:	$(PREFIX)/%.c $(INCLUDES)
	$(CC) $(CC_OPTS) -c  $<

%.elf:	%.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS)

clean:
	rm -rf $(OBJS) $(BINS) *.hex *~ *.o

%.hex:	%.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$@:i 
	rm $@
