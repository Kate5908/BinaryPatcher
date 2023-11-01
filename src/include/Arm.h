#ifdef __aarch64__

#include <stdint.h>

#define INSTRUCTION_SIZE 4

// MOV x22, #0x0 => important setup
#define MOV "\x16\x00\x80\xd2"
// ADRP x22, #0x0 => use this for PIE executables
#define ADRP "\x16\x00\x00\x90"
// BR x22
#define BR "\xc0\x02\x1f\xd6"

char *add(uint32_t imm);
char *immediateUnshifted(uint32_t imm);
char *immediateShifted(uint32_t imm);

#endif