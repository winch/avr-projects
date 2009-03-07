
#ifndef _TWI_H
#define _TWI_H

enum twi_read_mode {more, finished};

void twi_init();

int twi_start_read(uint8_t address);

int twi_start_write(uint8_t address);

uint8_t twi_read(enum twi_read_mode);

int twi_write(uint8_t data);

void twi_stop();

#endif

