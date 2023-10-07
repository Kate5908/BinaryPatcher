// virus.c
// written by Kate Rodionoff
// on 06/10/23
// virus that infects ELF executable files on Linux

// verify that file is an ELF executable
// find entry point
// inject simple code (to start with)

#include "ElfFile.h"
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

        
    }
}

