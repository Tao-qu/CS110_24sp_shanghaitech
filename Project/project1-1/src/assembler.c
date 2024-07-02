#include "../inc/assembler.h"
#include "../inc/util.h"

/* DO NOT MODIFY THE GIVEN API*/
int assembler(FILE *input_file,
              FILE *output_file)
{
  /*YOUR CODE HERE*/
  char line[100];
  while (fgets(line, sizeof(line), input_file))
  {
    char *instruction = strtok(line, " ()\r\n");
    char *token1 = strtok(NULL, " ()\r\n");
    char *token2 = strtok(NULL, " ()\r\n");
    char *token3 = strtok(NULL, " ()\r\n");
    InstructionType type = get_instruction_type(instruction);
    uint32_t code = -1;

    if (type == INSTR_R_TYPE)
    {
      code = get_code_R(instruction, token1, token2, token3);
    }
    else if (type == INSTR_I_TYPE)
    {
      code = get_code_I(instruction, token1, token2, token3);
    }
    else if (type == INSTR_S_TYPE)
    {
      code = get_code_S(instruction, token1, token2, token3);
    }
    else if (type == INSTR_SB_TYPE)
    {
      code = get_code_SB(instruction, token1, token2, token3);
    }
    else if (type == INSTR_U_TYPE)
    {
      code = get_code_U(instruction, token1, token2);
    }
    else if (type == INSTR_UJ_TYPE)
    {
      code = get_code_UJ(instruction, token1, token2);
    }
    else if (type == INSTR_PSEUDO)
    {
      if (!strcmp(instruction, "beqz"))
      {
        code = get_code_SB("beq", token1, "zero", token2);
      }
      else if (!strcmp(instruction, "bnez"))
      {
        code = get_code_SB("bne", token1, "zero", token2);
      }
      else if (!strcmp(instruction, "j"))
      {
        code = get_code_UJ("jal", "zero", token1);
      }
      else if (!strcmp(instruction, "jr"))
      {
        code = get_code_I("jalr", "zero", token1, "0");
      }
      else if (!strcmp(instruction, "li"))
      {
        int32_t imm = atoi(token2);

        if (imm >= -2048 && imm <= 2047)
        {
          code = get_code_I("addi", token1, "zero", token2);
        }
        else
        {
          int32_t upper = (imm >> 12) & 0xFFFFF;
          int32_t lower = imm & 0xFFF;
          if (imm & 0x800)
          {
            upper += 1;
            lower -= 0x1000;
          }

          char upper_str[21];
          sprintf(upper_str, "%d", upper);
          code = get_code_U("lui", token1, upper_str);
          dump_instruction(output_file, code);
          if (!is_valid(code))
          {
            continue;
          }

          char lower_str[13];
          sprintf(lower_str, "%d", lower);
          code = get_code_I("addi", token1, token1, lower_str);
        }
      }
      else if (!strcmp(instruction, "mv"))
      {
        code = get_code_I("addi", token1, token2, "0");
      }
    }
    dump_instruction(output_file, code);
  }

  return 0;
}
