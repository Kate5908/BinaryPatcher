#ifdef __aarch64__

#include <stdint.h>

#define INSTRUCTION_SIZE 4

#define MOV "\x16\x00\x80\xd2"
#define ADRP "\x16\x00\x00\x90"

char *add(uint32_t imm);
char *immediateUnshifted(uint32_t imm);
char *immediateShifted(uint32_t imm);

#endif