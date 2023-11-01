#include <stdint.h>

// returns the immediate for an unshifted ADD instruction
char *immediateUnshiftedAdd(uint32_t imm) {
    char *res = malloc(sizeof(char) * 2);
    res[0] = '\x02';
    res[1] = '\x00';

    if (imm >= 0x0 && imm <= 0x3) {
        res[0] = '\x02' + '\x4' * imm;
    }
}