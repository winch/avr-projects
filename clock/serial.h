
#ifndef _SERIAL_H
#define _SERIAL_H

void serial_init();

void serial_do_command();

void serial_write(uint8_t data);

void serial_write_string(char *data);

#endif

