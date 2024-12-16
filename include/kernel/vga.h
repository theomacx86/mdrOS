/*
mdrOS VGA header file
Copyright Théo JEHL - 2024
*/

#ifndef VGA_H
#define VGA_H

#include <stdarg.h>

/*
    Scroll the text one line down.
*/
void vga_scroll(void);

/*
    Prints a null-terminated string to the screen
    @param s Null-terminated string.
*/
void vga_print(char * s);

/*
    Clear the screen
*/
void vga_clear(void);

/*
Puts a character on screen
*/
void vga_putc(char c);

void vga_printf(char * s, ...);
#endif
