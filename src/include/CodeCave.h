// CodeCave.h
// Kate Rodionoff
//  07/10/23

#ifdef __linux__

#include <elf.h>
#include <stddef.h>

/**
 * A code cave is a region in an executable comprised entirely of zeroes
 * We want to find the code cave closest to the original entry point of the
 * executable. This ensures we won't segfault because the patched entry
 * point is outside the program's address space and that we won't overwrite
 * the program's headers
 */
typedef struct codeCave {
    // physical offset of the codeCave
    size_t offset;
    // virtual address of the code cave
    Elf64_Addr vaddr;
    // size of the code cave
    size_t size;
} CodeCave;

// finds a code cave in file specified by fd
// fd must already be opened
CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr, int min);

#endif