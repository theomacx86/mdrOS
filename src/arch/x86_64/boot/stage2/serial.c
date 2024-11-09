#include <stdint.h>
#include "serial.h"

#define COM1_PORT 0x3F8

#define PORT 0x3f8

uint16_t inw(uint16_t port)
{
    volatile uint16_t value = 0;
    //inb expects the port 
    asm volatile ("inw %%dx, %0" : "=a" (value) : "d" (port) );
    return value;
}

uint8_t inb(uint16_t port)
{
    volatile uint8_t value = 0;
    //inb expects the port 
    asm volatile ("inb %%dx, %0" : "=a" (value) : "d" (port) );
    return value;
}

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %%dx" : : "a" (value), "d" (port));
}

int serial_init()
{

   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   serial_write_str("[INFO] Serial port initialized successfully.\n");
   return 0;
}

uint8_t serial_is_transmit_empty()
{
    return (inb(PORT + 5) & 0x20);
}

void serial_write(char a)
{
    while(!serial_is_transmit_empty());
    outb(PORT, a);
}

void serial_write_str(char *str)
{
    while(*str != '\0')
    {
        serial_write(*str);
        ++str;
    }
}

void itoh(uint32_t i)
{
    if(i == 0)
    {
        return;
    }

    itoh( (uint32_t) (i/16) );

    if( i%16 >  9)
    {
        serial_write((char) ((i%16)+55) );
    }
    else
    {
        serial_write((char) ((i%16)+48));
    }
}

void itoc(int i)
{
    if(i == 0)
    {
        return;
    }

    itoc( (int) (i/10) );

    serial_write((char) ((i%10)+48));
}

void utoc(uint32_t i)
{
    if(i == 0)
    {
        return;
    }

    utoc( (uint32_t) (i/10) );

    serial_write((char) ((i%10)+48));
}

void serial_printf(char * s, ...)
{
    va_list list;
    va_start(list, s);

    while(*s != '\0')
    {
        switch (*s) {
            case '%':
                switch (*(s+1)) {
                    case '\0' :
                        ++s;
                        break;
                    case 'u' : //unsigned integer
                        utoc((uint32_t) va_arg( list, uint32_t ));
                        s+=2;
                        break;  
                    case 'd' : //integer
                        {
                            int value = (int) va_arg( list, int );
                            if(value == 0)
                            {
                                serial_write('0');
                            }
                            else
                            {
                                itoc(value);
                            }
                        }
                        s+=2;
                        break;
                    case 'x' : //hex (unsigned)
                        {
                            uint32_t value = (uint32_t) va_arg( list, uint32_t );
                            if(value == 0)
                            {
                                serial_write('0');
                            }
                            else
                            {
                                itoh(value);
                            }
                        }
                        s+=2;
                        break;
                    case 'l' : //long (integer or x)
                        if(*(s+2) == 'x')
                        {
                            uint64_t value = (uint64_t) va_arg( list, uint64_t );
                            itoh( (uint32_t) (value >> 32));
                            itoh( (uint32_t) (value));
                        }
                        s+=3;
                }
                break;
            default:
                serial_write(*s);
                ++s;
                break;
        }
    }
}