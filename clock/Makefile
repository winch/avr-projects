
CC = avr-gcc

CFLAGS = -Wall -Os -g -w -mmcu=atmega168 -DF_CPU=1000000L

LDLIBS = -Os -mmcu=atmega168

main: main.o display.o

main.o: display.h

display.o: display_values.h

hex: main
	avr-objcopy -O ihex -R .flash $< $<.hex

upload: hex
	avrdude -c usbtiny -p m168 -U flash:w:main.hex:i

.PHONY: clean
clean:
	rm -f *.o main main.hex

