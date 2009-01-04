
CC = avr-gcc

CFLAGS = -Wall -Os -g -w -mmcu=attiny2313 -DF_CPU=1000000L

LDLIBS = -Os -mmcu=attiny2313

main: main.o led.o button.o motor.o

hex: main
	avr-objcopy -O ihex -R .flash $< $<.hex

upload: hex
	sudo avrdude -c usbtiny -p t2313 -U flash:w:main.hex:i

.PHONY: clean
clean:
	rm -f *.o main main.hex

