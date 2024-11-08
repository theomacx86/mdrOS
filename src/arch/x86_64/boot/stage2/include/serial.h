#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

uint8_t inb(uint16_t port);

uint8_t serial_transmit_empty();

void outb(uint16_t port, uint8_t value);

void outb(uint16_t port, uint8_t value);

int serial_init();

void serial_write(char a);

void serial_write_str(char * str);

#endif