// CodeCave.c
// Kate Rodionoff
// 07/10/23
// Implementation file for CodeCave.h

#include "include/CodeCave.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 16

bool isCodeCave(char buf[SIZE]);
#ifdef __linux__

#include "include/ElfFile.h"

CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr, int min) {
    CodeCave cave;
    cave.offset = 0;
    cave.size = 0;
    cave.vaddr = phdr.p_vaddr;
    int offset = 0;
    if (isPie(ehdr)) offset = ehdr.e_entry;
    else offset = ehdr.e_entry - phdr.p_vaddr; 
    lseek(fd, offset, SEEK_SET);
    char buf[SIZE];

    while (read(fd, buf, SIZE) > 0) {
        if (isCodeCave(buf)) {
            cave.offset = lseek(fd, -16, SEEK_CUR);
            cave.size = SIZE;
            cave.vaddr = phdr.p_vaddr;
            return cave;
        }
    }

    fprintf(stderr, "Couldn't find code cave!\n");
    exit(1);
}
#endif

#ifdef __APPLE__
    CodeCave FindCodeCave(int fd) {
        CodeCave c;
        c.offset = 0;
        c.size = 0;

        
    }
#endif

bool isCodeCave(char buf[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (buf[i]) return false;
    }

    return true;
}
