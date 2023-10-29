#ifdef __aarch64__

#include <stdlib.h>
#include "include/Arm.h"

// File for operations related to ARM CPU
// Mainly to convert instructions to ARM Machine code

// Assume size of all instructions is 4
// Use -1 to

char *add(int r1, int r2, int imm) {
    char *ret = malloc(sizeof(char) * INSTRUCTION_SIZE);
    if (r1 != INVALID_REG && r2 != INVALID_REG) {
        ret[3] = '\x8b';
    }
}

void encodeRegister1(char buf[INSTRUCTION_SIZE], int r1) {
    if (r1 == 0) {
        buf[0] = '\x00';
        buf[1] = '\x00';
    } else if (r1 == 1) {
        buf[0] = '\x21';
        buf[1] = '\x00';
    } else if (r1 == 2) {
        buf[0] = '\x42';
        buf[1] = '\x00';
    } else if (r1 == 3) {
        buf[0] = '\x63';
        buf[1] = '\x00';
    } else if (r1 == 4) {
        buf[0] = '\x84';
        buf[1] = '\x00';
    } else if (r1 == 5) {
        buf[0] = '\x'
    }
}

void encodeRegister2(char buf[INSTRUCTION_SIZE], int r2) {
    if (r2 == 0) {
        buf[2] = '\x00'; 
    } else if (r2 == 1) {
        buf[2] = '\x01';
    } else if (r2 == 2) {
        buf[2] = '\x02';
    } else if (r2 == 3) {
        buf[2] = '\x03';
    }
}
#endif