#ifdef __aarch64__

#include <stdint.h>

#define INSTRUCTION_SIZE 4
#define INVALID_REG -1

const char *adrp(uint32_t imm);
char *add(uint32_t imm);
const char *mov(void);

#endif