/*
lolOS VGA i386 source file
Copyright Théo JEHL - 2024
*/

#include "kernel.h"
#include "kmem.h"
#include <stdarg.h>
#include <stdint.h>
#include "vga.h"
#include "inttypes.h"

char * video_memory = (char*)(0xB8000);

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

int cursor_x = 0;
int cursor_y = 0;

void vga_clear(void)
{
    kmemset((char *) video_memory, 0,VGA_HEIGHT * VGA_WIDTH * 2);
}

void vga_scroll(void)
{
    if(cursor_y == 0)
    {
        //nothing to scroll
        return;
    }

    //Scroll one line up
    for(int i = 0; i < cursor_y; ++i)
    {
        kmemcpy(video_memory + (i)*(VGA_WIDTH*2), video_memory + (i+1)*(VGA_WIDTH*2), 80*2);
    }

    //Clear current line
    kmemset( (char *)video_memory + cursor_y * (VGA_WIDTH*2), 0, VGA_WIDTH * 2);
    --cursor_y;
}

void vga_putc(char c)
{
    if(c == '\n')
    {
        if(cursor_y >= VGA_HEIGHT)
        {
            vga_scroll();
        }
        else
        {
            ++cursor_y;
        }
        cursor_x = 0;
    }
    else
    {
        if(cursor_x >= VGA_WIDTH - 1)
        {
            if(cursor_y >= VGA_HEIGHT)
            {
                vga_scroll();
            }
            else
            {
                ++cursor_y;
            }
            cursor_x = 0;
        }
        video_memory[((cursor_x )* 2) + (cursor_y * VGA_WIDTH * 2)] = c;
        video_memory[((cursor_x )* 2)+1 + (cursor_y * VGA_WIDTH * 2)] = 0xF;
        ++cursor_x;
    }
}

void vga_print(char * s)
{
    //String is empty
    if(s == NULL || *s == '\0')
    {
        return;
    }

    while(*s != '\0')
    {
        switch (*s) {
            case '\n':
                if(cursor_y < VGA_HEIGHT - 1)
                {
                    ++cursor_y;
                    cursor_x = 0;
                }
                else
                {
                    vga_scroll();
                }
                ++s;
                break;
            default:
                //Check for a line overflow before printing
                if(cursor_x == VGA_WIDTH - 1)
                {
                    //Line available, print
                    if(cursor_y < VGA_HEIGHT - 1)
                    {
                        ++cursor_y;
                        cursor_x = 0;
                    }
                    //Scroll
                    else
                    {
                        vga_scroll();
                    }
                }

                //Print char
                video_memory[((cursor_x )* 2) + (cursor_y * VGA_WIDTH * 2)] = *s;
                video_memory[((cursor_x )* 2)+1 + (cursor_y * VGA_WIDTH * 2)] = 0xF;
                ++cursor_x;
                ++s;
                break;
        }
    }
}

/*
    int as hexadecimal
*/
void itoh(uint64_t i)
{
    if(i == 0)
    {
        return;
    }

    itoh( (uint64_t) (i/16) );

    if( i%16 >  9)
    {
        vga_putc((char) ((i%16)+55) );
    }
    else
    {
        vga_putc((char) ((i%16)+48));
    }
}

/*
    Print an int
*/
void itoc(int i)
{
    if(i == 0)
    {
        return;
    }

    itoc( (int) (i/10) );

    vga_putc((char) ((i%10)+48));
}

/**
Unsigned to char
 */
void utoc(uint64_t i)
{
    if(i == 0)
    {
        return;
    }

    utoc( (uint64_t) (i/10) );

    vga_putc((char) ((i%10)+48));
}

void vga_printf(char * s, ...)
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
                        utoc((uint64_t) va_arg( list, uint64_t ));
                        s+=2;
                        break;  
                    case 'd' : //integer
                        {
                            int value = (int) va_arg( list, int );
                            if(value == 0)
                            {
                                vga_putc('0');
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
                            uint64_t value = (uint64_t) va_arg( list, uint64_t );
                            if(value == 0)
                            {
                                vga_putc('0');
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
                            itoh( (uint64_t) (value));
                        }
                        s+=3;
                }
                break;
            default:
                vga_putc(*s);
                ++s;
                break;
        }
    }
}
