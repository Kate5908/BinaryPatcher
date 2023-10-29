// CodeCave.h
// Kate Rodionoff
//  07/10/23

#ifdef __linux__

#include <elf.h>
#include <stddef.h>

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