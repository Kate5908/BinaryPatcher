#ifdef __aarch64__

#include <stdlib.h>
#include "include/Arm.h"

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

// returns the immediate for an unshifted ADD instruction
char *immediateUnshifted(uint32_t imm) {
    char *res = malloc(sizeof(char) * 2);

    res[1] = (imm / 0x40) * '\x01';
    imm = imm % 0x40;
    res[0] = '\x10' * (imm / 0x4);
    res[0] |= ('\x02' + '\x04' * (imm % 4));

    return res;
}

char *immediateShifted(uint32_t imm) {
    imm = imm / 0x1000;
    char *res = malloc(sizeof(char) * 2);

    res[1] = (imm / 0x40) * '\x01' + 0x40;
    imm = imm % 0x40;
    res[0] = '\x10' * (imm / 0x4);
    res[0] |= ('\x02' + '\x04' * (imm % 4));

    return res;
}

// ldr instructions can only be multiples of 4 :3
// encodes LDR x22, IMMEDIATE
// assumes that imm will be 3 digits max
char *ldr(uint32_t imm) {
    char *res = malloc(sizeof(char) * 4);

    uint32_t mask = 0xf;
    res[0] = '\x16' + '\x20' * (imm & mask);
    res[1] = '\x01' * (imm / 0x20);
    res[2] = '\x58';
}

#endif