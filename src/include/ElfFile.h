// ElfExec.h
// Header file for file operations on ELF Executable
// Kate Rodionoff
// 07/10/23

#include <elf.h>
#include <stdbool.h>
#include <stddef.h>

// Checks if the file specified by fd is an ELF executable
bool Elf(int fd);

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