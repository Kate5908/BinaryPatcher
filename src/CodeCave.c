// CodeCave.c
// Kate Rodionoff
// 07/10/23
// Implementation file for CodeCave.h

#ifdef __linux__

#include "include/CodeCave.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 16

bool isCodeCave(char buf[SIZE]);

CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr, int min) {
    CodeCave cave;
    cave.offset = 0;
    cave.size = 0;
    cave.vaddr = phdr.p_vaddr;

    lseek(fd, 0, SEEK_SET);
    char buf[SIZE];

    while (read(fd, buf, SIZE) > 0) {
        if (isCodeCave(buf)) {
            cave.offset = lseek(fd, -16, SEEK_CUR);
            cave.size = SIZE;
            cave.vaddr = phdr.p_vaddr + cave.offset;
            return cave;
        }
    }

    fprintf(stderr, "Couldn't find code cave!\n");
    exit(1);
}

bool isCodeCave(char buf[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (buf[i]) return false;
    }

    return true;
}

#endif