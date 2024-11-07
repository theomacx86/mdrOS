/*
lolOS Kernel memory lib header file
Copyright Théo JEHL - 2024
*/

#ifndef KMEM_H
#define KMEM_H

/*
    Copies n bytes from origin to dest.
*/
void kmemcpy(char * dest, char * origin, int n);

/*
    Fills the destination buffer with value n times
*/
void kmemset(char * dest, char value, int n);
#endif
