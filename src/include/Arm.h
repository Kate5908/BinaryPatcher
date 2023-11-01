#ifdef __aarch64__

#include <stdint.h>

#define INSTRUCTION_SIZE 4
#define INVALID_REG -1

char *encodeImm(uint32_t n);
void binaryToHex(char *binary, int shift, char *hex);

#endif