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

// define the page size
#define PAGE_SIZE getpagesize()

bool isCodeCave(char buf[PAGE_SIZE]);
uint64_t headerSize(Elf64_Ehdr ehdr);

// TODO:
// Create a makefile
// ElfMarkExecutable doesn't work properly

CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr) {
    CodeCave maxCave;
    maxCave.size = 0;
    CodeCave cur = {0, 0, 0};
    lseek(fd, ehdr.e_phoff + ehdr.e_phentsize * ehdr.e_phnum, SEEK_SET);

    char buf[PAGE_SIZE];
    bool isConsecutive = false;
    while (read(fd, buf, PAGE_SIZE) > 0) {
        if (cur.size == 0 && isCodeCave(buf)) {
            cur.offset = lseek(fd, 0, SEEK_CUR);
            cur.size += PAGE_SIZE;
            cur.vaddr = phdr.p_vaddr;
            isConsecutive = true;
        } else if (isConsecutive && isCodeCave(buf)) {
            cur.size += PAGE_SIZE;
        } else {
            isConsecutive = false;

            if (cur.size > maxCave.size) {
                maxCave = cur;
                cur.offset = 0;
                cur.size = 0;
                cur.vaddr = 0;
            }
        }
    }

    if (maxCave.size == 0) {
        fprintf(stderr, "Couldn't find code cave\n");
        exit(1);
    }

    return maxCave;
}   

bool isCodeCave(char buf[PAGE_SIZE]) {
    for (int i = 0; i < PAGE_SIZE; i++) {
        // if we encounter a non-null byte then the region is not a
        // code cave
        if (buf[i]) return false;
    }

    return true;
}

uint64_t headerSize(Elf64_Ehdr ehdr) {
    return ehdr.e_shoff + ehdr.e_shnum * ehdr.e_shentsize;
}

#endif