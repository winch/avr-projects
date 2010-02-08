
CC = avr-gcc

CFLAGS = -Wall -Os -g -w -mmcu=attiny2313 -DF_CPU=1000000L -ffunction-sections -fdata-sections -lm

LDLIBS = -Os -mmcu=attiny2313 --relax --gc-sections -lm

main: main.o led.o button.o motor.o

hex: main
	avr-objcopy -O ihex -R .flash $< $<.hex

upload: hex
	avrdude -c usbtiny -p t2313 -U flash:w:main.hex:i

.PHONY: fuse
fuse:
	avrdude -c usbtiny -p t2313 -U lfuse:w:0x64:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

.PHONY: clean
clean:
	rm -f *.o main main.hex

