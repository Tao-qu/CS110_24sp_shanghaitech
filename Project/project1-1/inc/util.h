#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define ASSEMBLER_ERROR -1
#define ASSEMBLER_CORRECT 0

typedef enum
{
    INSTR_R_TYPE,
    INSTR_I_TYPE,
    INSTR_S_TYPE,
    INSTR_SB_TYPE,
    INSTR_U_TYPE,
    INSTR_UJ_TYPE,
    INSTR_PSEUDO,
    INSTR_INVALID = -1
} InstructionType;

bool is_valid(uint32_t num);
bool is_num(char *str);
bool check_register(uint32_t num);

InstructionType get_instruction_type(char *instruction);
uint32_t get_register(char *name);
uint32_t get_code_R(char *instruction, char *token1, char *token2, char *token3);
uint32_t get_code_I(char *instruction, char *token1, char *token2, char *token3);
uint32_t get_code_S(char *instruction, char *token1, char *token2, char *token3);
uint32_t get_code_SB(char *instruction, char *token1, char *token2, char *token3);
uint32_t get_code_U(char *instruction, char *token1, char *token2);
uint32_t get_code_UJ(char *instruction, char *token1, char *token2);

void dump_instruction(FILE *file_, uint32_t instruction);

void dump_code(FILE *file_, uint32_t code);

void dump_error_information(FILE *file_);

#endif
