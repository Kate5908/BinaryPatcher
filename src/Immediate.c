#include <stdint.h>
#include <stdlib.h>
#include "include/Immediate.h"

// returns the immediate for an unshifted ADD instruction
char *immediateUnshiftedAdd(uint32_t imm) {
    char *res = malloc(sizeof(char) * 2);

    res[1] = (imm / 0x40) * '\x01';
    imm = imm % 0x40;
    res[0] = '\x10' * (imm / 0x4);
    res[0] |= ('\x02' + '\x04' * (imm % 4));

    return res;
}

char *immediateShiftedAddress(uint32_t imm) {
    imm = imm / 0x1000;
    char *res = malloc(sizeof(char) * 2);

    res[1] = (imm / 0x40) * '\x01' + 0x40;
    imm = imm % 0x40;
    res[0] = '\x10' * (imm / 0x4);
    res[0] |= ('\x02' + '\x04' * (imm % 4));

    return res;
}
