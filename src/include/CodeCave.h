// CodeCave.h
// Kate Rodionoff
//  07/10/23

#include <stdint.h>
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
    #ifdef __linux__
    // virtual address of the code cave
    int64_t vaddr;
    #endif
    // size of the code cave
    size_t size;
} CodeCave;

// finds a code cave in file specified by fd
// fd must already be opened
#ifdef __linux__
CodeCave FindCodeCave(int fd, Elf64_Phdr phdr, Elf64_Ehdr ehdr, int min);
#endif

#ifdef __APPLE__
CodeCave FindCodeCave(int fd);
#endif
