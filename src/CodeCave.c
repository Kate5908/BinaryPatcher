// CodeCave.c
// Kate Rodionoff
// 07/10/23
// Implementation file for CodeCave.h

#include "include/CodeCave.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// define the page size
#define PAGE_SIZE getpagesize()

bool isCodeCave(char buf[PAGE_SIZE]);
uint64_t headerSize(Elf64_Ehdr ehdr);

// TODO:
//  Define minimum size for code cave
//  Maybe try to find the biggest code cave
// Create a makefile

CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr) {
    // increment the file pointer so we don't accidentally overwrite
    // the headers
    int err = lseek(fd, headerSize(ehdr), SEEK_SET);
    if (err < 0) {
        fprintf(stderr, "lseek couldn't move file pointer");
        exit(1);
    }
    printf("Virtual address offset = %d\n", phdr.p_vaddr);
    CodeCave codeCave;
    codeCave.offset = 0;
    codeCave.vaddr = 0;
    codeCave.size = 0;

    char buf[PAGE_SIZE];
    bool isConsecutive = false;
    while (read(fd, buf, PAGE_SIZE) > 0) {
        if (isCodeCave(buf) && !codeCave.vaddr) {
            codeCave.offset = lseek(fd, -PAGE_SIZE, SEEK_CUR);
            codeCave.size = PAGE_SIZE;
            codeCave.vaddr = phdr.p_vaddr + codeCave.offset;

            isConsecutive = true;

            printf("Found code cave!!\n");
            lseek(fd, PAGE_SIZE, SEEK_CUR);
        } else if (isCodeCave(buf) && isConsecutive) {
            codeCave.size += PAGE_SIZE;
        } else {
            isConsecutive = false;
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

uint64_t headerSize(Elf64_Ehdr ehdr) {
    return ehdr.e_shoff + ehdr.e_shnum * ehdr.e_shentsize;
}