// ElfFile.c
// Kate Rodionoff
// 07/10/23

#include <string.h>
#include <unistd.h>
#include "include/ElfFile.h"

#define MAGIC_BYTES "\x7f\x45\x4c\x46"
#define MAGIC_BYTES_SIZE 4

bool Elf(int fd) {
    // reset the file pointer
    lseek(fd, 0, SEEK_SET);

    // allocate a buffer to read into
    char buf[MAGIC_BYTES_SIZE];

    // if read does not read MAGIC_BYTES_SIZE, flag an error
    int count = read(fd, buf, MAGIC_BYTES_SIZE);
    if (count != MAGIC_BYTES_SIZE) return false;

    // if the magic bytes are the same as the bytes read from buf
    // the file is an ELF executable file
    return strncmp(buf, MAGIC_BYTES, MAGIC_BYTES_SIZE) == 0;
}

// Extracts the Elf header of the file specified by fd
Elf64_Ehdr ElfExtractHeader(int fd);

// Extracts the entry point from an ELF header
Elf64_Addr ElfExtractEntry(Elf64_Ehdr elf);

// Extracts the program header from an ELF header
Elf64_Phdr ElfExtractProgramHeader(int fd);

// Extracts virtual address from an ELF header
Elf64_Addr ElfExtractVaddr(Elf64_Phdr elf);

// marks the section offset resides in as executable
// returns 1 on success, zero on failure
int ElfMarkExecutable(Elf64_Ehdr elf, Elf64_Addr offset);

// overwrites section specified by offset with contents of buffer
int ElfOverwriteSection(size_t offset, char *buf, int bufSize);