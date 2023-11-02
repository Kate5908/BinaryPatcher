#ifdef __aarch64__

#include <stdint.h>

#define INSTRUCTION_SIZE 4

// MOV x22, #0x0 => important setup
#define MOV "\x16\x00\x80\xd2"
// ADRP x22, #0x0 => use this for PIE executables
#define ADRP "\x16\x00\x00\x90"
// BR x22
#define BR "\xc0\x02\x1f\xd6"

#define LDR "\x16\x00\x00\x58"

// encodes the instruction ADD x22, x22, [imm] in ARM64
char *add(uint32_t imm);

/**
 * The following two function signatures perform the same operation
 * but encode different values
 * 
 * For a value between 0x0 and 0xfff (inclusive) we can directly encode the immediate
 * so we set the shift bit to be 0
 * The instruction is written in assembly as ADD [Rd], [Rn], [Imm]
 * 
 * For a value greater than 0xfff, we need to apply a bit shift in order to fit
 * the immediate into a 2 byte encoding
 * 
 * We set the shift bit to 1 (hence the need for separate functions)
 * The assembly instruction would be ADD [Rd], [Rn], [Imm / 0x1000], lsl #12
 * 
 * It is impossible to use values like 0x400580 in an add instruction directly
 * We would instead need to break it up into two separate ADD instructions
 *      ADD x0, x0, #0x400000, lsl #12
 *      ADD x0, x0, #0x580
 */

// calculates the immediate for an unshifted ADD instruction
char *immediateUnshifted(uint32_t imm);
// calculates the immediate for a shifted ADD instruction
char *immediateShifted(uint32_t imm);
// encodes the instruction SUB x22, x22, [imm] in ARM64
char *sub(uint32_t imm);

#endif