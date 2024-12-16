/*
mdrOS second stage bootloader serial header file
Copyright 2024 Théo Jehl
*/

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdarg.h>

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

uint8_t serial_transmit_empty();

void outb(uint16_t port, uint8_t value);


int serial_init();

void serial_write(char a);

void serial_write_str(char * str);

void serial_printf(char * s, ...);

#endif
