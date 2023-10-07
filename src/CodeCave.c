// CodeCave.c
// Kate Rodionoff
// 07/10/23
// Implementation file for CodeCave.h

#include "include/CodeCave.h"
#include <stdbool.h>
#include <unistd.h>

// define the page size
#define PAGE_SIZE getpagesize()

CodeCave FindCodeCave(int fd, Elf64_Phdr phdr) {
    CodeCave codeCave;
    codeCave.offset = 0;
    codeCave.vaddr = NULL;
    codeCave.size = 0;

    char buf[PAGE_SIZE];
    bool isConsecutive = false;
    while (read(fd, buf, PAGE_SIZE) > 0) {
        if (isCodeCave(buf) && !codeCave.vaddr) {
            codeCave.offset = lseek(fd, -PAGE_SIZE, SEEK_SET);
            codeCave.size = PAGE_SIZE;
            codeCave.vaddr = phdr.p_vaddr + codeCave.offset;
            isConsecutive = true;
        } else if (isCodeCave(buf) && isConsecutive) {
            codeCave.size += PAGE_SIZE;
        }
    }
    return codeCave;
}   

bool isCodeCave(char buf[PAGE_SIZE]) {
    for (int i = 0; i < PAGE_SIZE; i++) {
        // if we encounter a non-null byte then the region is not a
        // code cave
        if (buf[i]) return false;
    }

    return true;
}