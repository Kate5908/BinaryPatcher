#ifdef __aarch64__

#include <stdlib.h>
#include "include/Arm.h"
#include "include/Immediate.h"

// Arm64 can only use immediates up to 32 bits
#define INT_BITS 32
#define IMM_BITS 12
#define ROTATION_BITS 4
#define NUMBER_BITS 8

// File for operations related to ARM CPU
// Mainly to convert instructions to ARM Machine code

// Assume size of all instructions is 4
// Use register 22 for returning from functions
// just makes my life easier to hard code that :)
const char *adrp(uint32_t imm) {
    return "\x16\x00\x00\x90";
}

char *add(uint32_t imm) {
    char *hex = malloc(sizeof(char) * INSTRUCTION_SIZE);

    char *i;
    if (imm / 0x1000 > 0) i = immediateShiftedAdd(i);
    else imm = immediateUnshiftedAdd(i);

    hex[0] = '\xd6';
    hex[1] = i[0];
    hex[2] = i[1];
    hex[3] = '\x91';

    free(i);

    return hex;
}

// moves immediate 0 into register 22
// ensures that the register is reset
const char *mov(void) {
    return "\x16\x00\x80\xd2";
}

#endif