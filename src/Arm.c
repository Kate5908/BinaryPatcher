#ifdef __aarch64__

#include <stdlib.h>
#include "include/Arm.h"

// Arm64 can only use immediates up to 32 bits
#define INT_BITS 32
#define IMM_BITS 12
#define ROTATION_BITS 4
#define NUMBER_BITS 8

uint32_t rightRotate(uint32_t n);
uint8_t getHex(char *bits);
// File for operations related to ARM CPU
// Mainly to convert instructions to ARM Machine code

// Assume size of all instructions is 4
// Use register 12 for returning from functions
// just makes my life easier to hard code that :)
char *adrp(uint32_t imm) {

}

char *add(uint32_t imm) {

}

// moves immediate 0 into register 12
// ensures that the register is reset
char *mov(void) {

}

// encodes 32 bit integer n into an arm encoding
char *encodeImm(uint32_t n) {
    uint32_t significant = n;
    uint32_t mask = 0x0001;
    
    // we want the last 1 bit to be positioned at 0
    while (!(mask & significant)) significant >>= 1;

    int rotations = 0;
    int rotated = significant;

    while (rotated != n) {
        rotated = rightRotate(rotated);
        rotations++;
    }

    // arm cpus only use even numbers from 0 to 30 for rotations
    rotations /= 2;

    char *imm = calloc(sizeof(char) * (IMM_BITS + 4));
    uint16_t mask = 0x0800;
    // top 4 bits should be the rotation
    for (int i = 4; i < ROTATION_BITS + 4; i++) {
        if (rotations & mask) imm[i] = '1';
        else imm[i] = '0';

        mask >>= 1;
    }

    // bottom 8 bits should be the number itself
    for (int i = 8; i < NUMBER_BITS + 8; i++) {
        if (significant & mask) imm[i] = '1';
        else imm[i] = '0';

        mask >>= 1;
    }

    return imm; 
}

// rotates a bit n by 1 unit
uint32_t rightRotate(uint32_t n) {
    return (n >> 1) | (n << (INT_BITS - 1));
}

void binaryToHex(char *binary, int level, char *hex) {
    // only want to recurse twice
    if (level == 2) return;
    char bit = '\x00';

    bit |= (getHex(binary) << 4);
    bit |= (getHex(binary + 4));

    hex[0] = bit;

    binaryToHex(binary + 8, shift + 1, hex + 1);
}

uint8_t getHex(char *bits) {
    // 0x0 = 0000
    if (bits[0] == '0' && bits[1] == '0' && bits[2] == '0' && bits[3] == '0') 
        return 0x0;
    // 0x1 = 0001
    else if (bits[0] == '0' && bits[1] == '0' && bits[2] == '0' && bits[3] == '1')
        return 0x1;
    // 0x2 = 0010
    else if (bits[0] == '0' && bits[1] == '0' && bits[2] == '1' && bits[3] == '0')
        return 0x2;
    // 0x3 = 0011
    else if (bits[0] == '0' && bits[1] == '0' && bits[2] == '1' && bits[3] == '1')
        return 0x3;
    // 0x4 = 0100
    else if (bits[0] == '0' && bits[1] == '1' && bits[2] == '0' && bits[3] == '0')
        return 0x4;
    // 0x5 = 0101
    else if (bits[0] == '0' && bits[1] == '1' && bits[2] == '0' && bits[3] == '1')
        return 0x5;
    // 0x6 = 0110
    else if (bits[0] == '0' && bits[1] == '1' && bits[2] == '1' && bits[3] == '0')
        return 0x6;
    // 0x7 = 0111
    else if (bits[0] == '0' && bits[1] == '1' && bits[2] == '1' && bits[3] == '1')
        return 0x7;
    // 0x8 = 1000
    else if (bits[0] == '1' && bits[1] == '0' && bits [2] == '0' && bits[3] == '0')
        return 0x8;
    // 0x9 = 1001
    else if (bits[0] == '1' && bits[1] == '0' && bits[2] == '0' && bits[3] == '1')
        return 0x9;
    // 0xa = 1010
    else if (bits[0] == '1' && bits[1] == '0' && bits[2] == '1' && bits[3] == '0')
        return 0xa;
    // 0xb = 1011
    else if (bits[0] == '1' && bits[1] == '0' && bits[2] == '1' && bits[3] == '1')
        return 0xb;
    // 0xc = 1100
    else if (bits[0] == '1' && bits[1] == '1' && bits[2] == '0' && bits[3] == '0')
        return 0xc;
    // 0xd = 1101
    else if (bits[0] == '1' && bits[1] == '1' && bits[2] == '0' && bits[3] == '1')
        return 0xd;
    // 0xe = 1110
    else if (bits[0] == '1' && bits[1] == '1' && bits[2] == '1' && bits[3] == '0')
        return 0xe;
    // 0xf = 1111
    else return 0xf;
}
#endif