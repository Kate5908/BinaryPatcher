// ElfFile.c
// Kate Rodionoff
// 07/10/23

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/ElfFile.h"

#define FAILURE 1
#define MAGIC_BYTES "\x7f\x45\x4c\x46"
#define MAGIC_BYTES_SIZE 4
#define SUCCESS 0

bool Elf(int fd) {
    // reset the file pointer
    int err = lseek(fd, 0, SEEK_SET);
    // if lseek fails return false
    if (err < 0) {
        fprintf(stderr, "Elf lseek failed\n");
        return false;
    }

    // allocate a buffer to read into
    char buf[MAGIC_BYTES_SIZE];

    // if read does not read MAGIC_BYTES_SIZE, flag an error
    err = read(fd, buf, MAGIC_BYTES_SIZE);
    if (err != MAGIC_BYTES_SIZE) {
        fprintf(stderr, "Elf read failed\n");
    }
    // if the magic bytes are the same as the bytes read from buf
    // the file is an ELF executable file
    return strncmp(buf, MAGIC_BYTES, MAGIC_BYTES_SIZE) == 0;
}

// Extracts the Elf header of the file specified by fd
Elf64_Ehdr ElfExtractHeader(int fd) {
    Elf64_Ehdr elfHeader;

    // lseek fails, exit the program
    int err = lseek(fd, 0, SEEK_SET);
    if (err < 0) {
        fprintf(stderr, "lseek failed\n");
        exit(FAILURE);
    }

    // read into the elf header
    int count = read(fd, &elfHeader, sizeof(elfHeader));
    if (count != sizeof(elfHeader)) {
        fprintf(stderr, "Reading elf header failed\n");
        exit(FAILURE);
    }

    // elfHeader was successfully read
    return elfHeader;
}

// Extracts the entry point from an ELF header
Elf64_Addr ElfExtractEntry(Elf64_Ehdr elf) {
    return elf.e_entry;
}

// Extracts the program header from an ELF header
Elf64_Phdr ElfExtractProgramHeader(int fd, Elf64_Ehdr ehdr) {
    Elf64_Phdr phdr;
    int numHeaders = ehdr.e_phnum;

    // set position to right after the ELF header
    int err = lseek(fd, ehdr.e_phoff, SEEK_SET);
    if (err < 0) {
        fprintf(stderr, "lseek failed\n");
        exit(FAILURE);
    }

    // read in the program header
    int count = read(fd, &phdr, sizeof(phdr));
    for (int i = 0; i < numHeaders; i++) {
        if (count < sizeof(phdr)) {
            fprintf(stderr, "Reading program header failed\n");
            exit(FAILURE);
        } else if (phdr.p_type == PT_LOAD && phdr.p_vaddr > 0) {
            break;
        }

        count = read(fd, &phdr, sizeof(phdr));
    }
    // if read was successful, return the program header
    return phdr;
}

// Extracts virtual address from an ELF header
Elf64_Addr ElfExtractVaddr(Elf64_Phdr elf) {
    return elf.p_vaddr;
}

// marks the section offset resides in as executable
// returns 0 on success, 1 on failure
int ElfMarkExecutable(Elf64_Ehdr elf, Elf64_Addr offset, int fd) {
    int err = lseek(fd, elf.e_shoff, SEEK_SET);
    if (err < 0) return FAILURE;

    int success = 0;
    for (int i = 0; i < elf.e_shnum; i++) {
        Elf64_Shdr shdr;
        int count = read(fd, &shdr, sizeof(shdr));

        if (count < sizeof(shdr)) {
            fprintf(stderr, "ElfMarkExecutable couldn't read file\n");
            return FAILURE;
        } else if (shdr.sh_addr < offset && !success) {
            shdr.sh_flags |= SHF_EXECINSTR;
            shdr.sh_flags |= SHF_ALLOC;

            err = lseek(fd, -sizeof(shdr), SEEK_CUR);
            if (err < 0) {
                fprintf(stderr, "ElfMarkExecutable lseek failed\n");
                return FAILURE;
            }

            err = write(fd, &shdr, sizeof(shdr));
            if (err < 0) {
                fprintf(stderr, "ElfMarkExecutable couldn't write to file\n");
                return FAILURE;
            }
            success = 1;
        }

        if (shdr.sh_name == 13) {
            shdr.sh_addr = offset;
            lseek(fd, -sizeof(shdr), SEEK_CUR);
            write(fd, &shdr, sizeof(shdr));
        }
        return SUCCESS;
    }

    // couldn't find the corresponding section, return failure
    fprintf(stderr, "ElfMarkExecutable couldn't find section\n");
    return FAILURE;
}

// overwrites section specified by offset with contents of buffer
int ElfOverwriteSection(size_t offset, char *buf, int bufSize, int fd) {
    int err = lseek(fd, offset, SEEK_SET);
    if (err < 0) {
        fprintf(stderr, "ElfOverwriteSection, couldn't lseek\n");
        return FAILURE;
    }

    err = write(fd, buf, bufSize);
    if (err < 0) {
        fprintf(stderr, "ElfOverwriteSection, couldn't write to file");
        return FAILURE;
    }

    return SUCCESS;
}

bool isPie(Elf64_Ehdr ehdr) {
    return ehdr.e_type == ET_DYN;
}

#endif