#include "../inc/util.h"

/* YOUR CODE HERE */
bool is_valid(uint32_t num)
{
  uint32_t temp = -1;
  if (num == temp)
  {
    return false;
  }
  return true;
}

bool is_num(char *str)
{
  if (str == NULL || *str == '\0')
  {
    return false;
  }
  if (*str == '-')
  {
    ++str;
  }
  if (*str == '\0')
  {
    return false;
  }
  while (*str != '\0')
  {
    if (!isdigit(*str))
    {
      return false;
    }
    ++str;
  }
  return true;
}

bool check_register(uint32_t num)
{
  return is_valid(num);
}

InstructionType get_instruction_type(char *instruction)
{
  if (!strcmp(instruction, "add") || !strcmp(instruction, "mul") || !strcmp(instruction, "sub") || !strcmp(instruction, "sll") || !strcmp(instruction, "mulh") || !strcmp(instruction, "slt") || !strcmp(instruction, "sltu") || !strcmp(instruction, "xor") || !strcmp(instruction, "div") || !strcmp(instruction, "srl") || !strcmp(instruction, "sra") || !strcmp(instruction, "or") || !strcmp(instruction, "rem") || !strcmp(instruction, "and"))
  {
    return INSTR_R_TYPE;
  }
  else if (!strcmp(instruction, "lb") || !strcmp(instruction, "lh") || !strcmp(instruction, "lw") || !strcmp(instruction, "lbu") || !strcmp(instruction, "lhu") || !strcmp(instruction, "addi") || !strcmp(instruction, "slli") || !strcmp(instruction, "slti") || !strcmp(instruction, "sltiu") || !strcmp(instruction, "xori") || !strcmp(instruction, "srli") || !strcmp(instruction, "srai") || !strcmp(instruction, "ori") || !strcmp(instruction, "andi") || !strcmp(instruction, "jalr") || !strcmp(instruction, "ecall"))
  {
    return INSTR_I_TYPE;
  }
  else if (!strcmp(instruction, "sb") || !strcmp(instruction, "sh") || !strcmp(instruction, "sw"))
  {
    return INSTR_S_TYPE;
  }
  else if (!strcmp(instruction, "beq") || !strcmp(instruction, "bne") || !strcmp(instruction, "blt") || !strcmp(instruction, "bge") || !strcmp(instruction, "bltu") || !strcmp(instruction, "bgeu"))
  {
    return INSTR_SB_TYPE;
  }
  else if (!strcmp(instruction, "auipc") || !strcmp(instruction, "lui"))
  {
    return INSTR_U_TYPE;
  }
  else if (!strcmp(instruction, "jal"))
  {
    return INSTR_UJ_TYPE;
  }
  else if (!strcmp(instruction, "beqz") || !strcmp(instruction, "bnez") || !strcmp(instruction, "j") || !strcmp(instruction, "jr") || !strcmp(instruction, "li") || !strcmp(instruction, "mv"))
  {
    return INSTR_PSEUDO;
  }
  return INSTR_INVALID;
}

uint32_t get_register(char *name)
{
  if (!strcmp(name, "zero") || !strcmp(name, "x0"))
  {
    return 0;
  }
  else if (!strcmp(name, "ra") || !strcmp(name, "x1"))
  {
    return 1;
  }
  else if (!strcmp(name, "sp") || !strcmp(name, "x2"))
  {
    return 2;
  }
  else if (!strcmp(name, "gp") || !strcmp(name, "x3"))
  {
    return 3;
  }
  else if (!strcmp(name, "tp") || !strcmp(name, "x4"))
  {
    return 4;
  }
  else if (!strcmp(name, "t0") || !strcmp(name, "x5"))
  {
    return 5;
  }
  else if (!strcmp(name, "t1") || !strcmp(name, "x6"))
  {
    return 6;
  }
  else if (!strcmp(name, "t2") || !strcmp(name, "x7"))
  {
    return 7;
  }
  else if (!strcmp(name, "s0") || !strcmp(name, "x8") || !strcmp(name, "fp"))
  {
    return 8;
  }
  else if (!strcmp(name, "s1") || !strcmp(name, "x9"))
  {
    return 9;
  }
  else if (!strcmp(name, "a0") || !strcmp(name, "x10"))
  {
    return 10;
  }
  else if (!strcmp(name, "a1") || !strcmp(name, "x11"))
  {
    return 11;
  }
  else if (!strcmp(name, "a2") || !strcmp(name, "x12"))
  {
    return 12;
  }
  else if (!strcmp(name, "a3") || !strcmp(name, "x13"))
  {
    return 13;
  }
  else if (!strcmp(name, "a4") || !strcmp(name, "x14"))
  {
    return 14;
  }
  else if (!strcmp(name, "a5") || !strcmp(name, "x15"))
  {
    return 15;
  }
  else if (!strcmp(name, "a6") || !strcmp(name, "x16"))
  {
    return 16;
  }
  else if (!strcmp(name, "a7") || !strcmp(name, "x17"))
  {
    return 17;
  }
  else if (!strcmp(name, "s2") || !strcmp(name, "x18"))
  {
    return 18;
  }
  else if (!strcmp(name, "s3") || !strcmp(name, "x19"))
  {
    return 19;
  }
  else if (!strcmp(name, "s4") || !strcmp(name, "x20"))
  {
    return 20;
  }
  else if (!strcmp(name, "s5") || !strcmp(name, "x21"))
  {
    return 21;
  }
  else if (!strcmp(name, "s6") || !strcmp(name, "x22"))
  {
    return 22;
  }
  else if (!strcmp(name, "s7") || !strcmp(name, "x23"))
  {
    return 23;
  }
  else if (!strcmp(name, "s8") || !strcmp(name, "x24"))
  {
    return 24;
  }
  else if (!strcmp(name, "s9") || !strcmp(name, "x25"))
  {
    return 25;
  }
  else if (!strcmp(name, "s10") || !strcmp(name, "x26"))
  {
    return 26;
  }
  else if (!strcmp(name, "s11") || !strcmp(name, "x27"))
  {
    return 27;
  }
  else if (!strcmp(name, "t3") || !strcmp(name, "x28"))
  {
    return 28;
  }
  else if (!strcmp(name, "t4") || !strcmp(name, "x29"))
  {
    return 29;
  }
  else if (!strcmp(name, "t5") || !strcmp(name, "x30"))
  {
    return 30;
  }
  else if (!strcmp(name, "t6") || !strcmp(name, "x31"))
  {
    return 31;
  }
  return -1;
}

uint32_t get_code_R(char *instruction, char *token1, char *token2, char *token3)
{
  uint32_t rd = get_register(token1);
  uint32_t rs1 = get_register(token2);
  uint32_t rs2 = get_register(token3);
  if (!(check_register(rd) && check_register(rs1) && check_register(rs2)))
  {
    return -1;
  }

  uint32_t opcode = 0x33;
  uint32_t funct3 = -1;
  uint32_t funct7 = -1;
  if (!strcmp(instruction, "add"))
  {
    funct3 = 0x0;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "mul"))
  {
    funct3 = 0x0;
    funct7 = 0x01;
  }
  else if (!strcmp(instruction, "sub"))
  {
    funct3 = 0x0;
    funct7 = 0x20;
  }
  else if (!strcmp(instruction, "sll"))
  {
    funct3 = 0x1;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "mulh"))
  {
    funct3 = 0x1;
    funct7 = 0x01;
  }
  else if (!strcmp(instruction, "slt"))
  {
    funct3 = 0x2;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "sltu"))
  {
    funct3 = 0x3;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "xor"))
  {
    funct3 = 0x4;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "div"))
  {
    funct3 = 0x4;
    funct7 = 0x01;
  }
  else if (!strcmp(instruction, "srl"))
  {
    funct3 = 0x5;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "sra"))
  {
    funct3 = 0x5;
    funct7 = 0x20;
  }
  else if (!strcmp(instruction, "or"))
  {
    funct3 = 0x6;
    funct7 = 0x00;
  }
  else if (!strcmp(instruction, "rem"))
  {
    funct3 = 0x6;
    funct7 = 0x01;
  }
  else if (!strcmp(instruction, "and"))
  {
    funct3 = 0x7;
    funct7 = 0x00;
  }
  else
  {
    return -1;
  }
  uint32_t code = opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20) | (funct7 << 25);

  return code;
}

uint32_t get_code_I(char *instruction, char *token1, char *token2, char *token3)
{
  if (!strcmp(instruction, "ecall"))
  {
    return 0x00000073;
  }

  uint32_t rd = get_register(token1);
  if (!check_register(rd))
  {
    return -1;
  }

  uint32_t opcode = -1;
  uint32_t funct3 = -1;
  if (!strcmp(instruction, "lb") || !strcmp(instruction, "lh") || !strcmp(instruction, "lw") || !strcmp(instruction, "lbu") || !strcmp(instruction, "lhu"))
  {
    if (!is_num(token2))
    {
      return -1;
    }
    int32_t offset = atoi(token2);
    uint32_t rs1 = get_register(token3);
    if ((offset > 2047) || (offset < -2048) || !check_register(rs1))
    {
      return -1;
    }

    opcode = 0x03;
    if (!strcmp(instruction, "lb"))
    {
      funct3 = 0x0;
    }
    else if (!strcmp(instruction, "lh"))
    {
      funct3 = 0x1;
    }
    else if (!strcmp(instruction, "lw"))
    {
      funct3 = 0x2;
    }
    else if (!strcmp(instruction, "lbu"))
    {
      funct3 = 0x4;
    }
    else if (!strcmp(instruction, "lhu"))
    {
      funct3 = 0x5;
    }
    else
    {
      return -1;
    }
    uint32_t code = opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (offset << 20);

    return code;
  }
  else if (!strcmp(instruction, "addi") || !strcmp(instruction, "slli") || !strcmp(instruction, "slti") || !strcmp(instruction, "sltiu") || !strcmp(instruction, "xori") || !strcmp(instruction, "srli") || !strcmp(instruction, "srai") || !strcmp(instruction, "ori") || !strcmp(instruction, "andi"))
  {
    if (!is_num(token3))
    {
      return -1;
    }
    uint32_t rs1 = get_register(token2);
    int32_t imm = atoi(token3);
    if (!check_register(rs1) || (imm > 2047) || (imm < -2048))
    {
      return -1;
    }

    opcode = 0x13;
    if (!strcmp(instruction, "addi"))
    {
      funct3 = 0x0;
    }
    else if (!strcmp(instruction, "slli"))
    {
      if (imm < 0 || imm > 31)
      {
        return -1;
      }
      funct3 = 0x1;
      uint32_t funct7 = 0x00;
      imm = imm | (funct7 << 5);
    }
    else if (!strcmp(instruction, "slti"))
    {
      funct3 = 0x2;
    }
    else if (!strcmp(instruction, "sltiu"))
    {
      funct3 = 0x3;
    }
    else if (!strcmp(instruction, "xori"))
    {
      funct3 = 0x4;
    }
    else if (!strcmp(instruction, "srli"))
    {
      if (imm < 0 || imm > 31)
      {
        return -1;
      }
      funct3 = 0x5;
      uint32_t funct7 = 0x00;
      imm = imm | (funct7 << 5);
    }
    else if (!strcmp(instruction, "srai"))
    {
      if (imm < 0 || imm > 31)
      {
        return -1;
      }
      funct3 = 0x5;
      uint32_t funct7 = 0x20;
      imm = imm | (funct7 << 5);
    }
    else if (!strcmp(instruction, "ori"))
    {
      funct3 = 0x6;
    }
    else if (!strcmp(instruction, "andi"))
    {
      funct3 = 0x7;
    }
    else
    {
      return -1;
    }
    uint32_t code = opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (imm << 20);

    return code;
  }
  else if (!strcmp(instruction, "jalr"))
  {
    if (!is_num(token3))
    {
      return -1;
    }
    uint32_t rs1 = get_register(token2);
    int32_t imm = atoi(token3);
    if (!check_register(rs1) || (imm > 2047) || (imm < -2048))
    {
      return -1;
    }

    opcode = 0x67;
    funct3 = 0x0;

    uint32_t code = opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (imm << 20);

    return code;
  }
  return -1;
}

uint32_t get_code_S(char *instruction, char *token1, char *token2, char *token3)
{
  if (!is_num(token2))
  {
    return -1;
  }
  uint32_t rs2 = get_register(token1);
  int32_t offset = atoi(token2);
  uint32_t rs1 = get_register(token3);
  if (!(check_register(rs2) && check_register(rs1)) || (offset > 2047) || (offset < -2048))
  {
    return -1;
  }

  uint32_t opcode = 0x23;
  uint32_t funct3 = -1;
  if (!strcmp(instruction, "sb"))
  {
    funct3 = 0x0;
  }
  else if (!strcmp(instruction, "sh"))
  {
    funct3 = 0x1;
  }
  else if (!strcmp(instruction, "sw"))
  {
    funct3 = 0x2;
  }
  else
  {
    return -1;
  }
  uint32_t code = opcode | ((offset & 0x1F) << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20) | (((offset >> 5) & 0x7F) << 25);

  return code;
}

uint32_t get_code_SB(char *instruction, char *token1, char *token2, char *token3)
{
  if (!is_num(token3))
  {
    return -1;
  }
  uint32_t rs1 = get_register(token1);
  uint32_t rs2 = get_register(token2);
  int32_t offset = atoi(token3);
  if (!(check_register(rs1) && check_register(rs2)) || (offset < -4096) || (offset > 4095))
  {
    return -1;
  }

  uint32_t opcode = 0x63;
  uint32_t funct3 = -1;
  if (!strcmp(instruction, "beq"))
  {
    funct3 = 0x0;
  }
  else if (!strcmp(instruction, "bne"))
  {
    funct3 = 0x1;
  }
  else if (!strcmp(instruction, "blt"))
  {
    funct3 = 0x4;
  }
  else if (!strcmp(instruction, "bge"))
  {
    funct3 = 0x5;
  }
  else if (!strcmp(instruction, "bltu"))
  {
    funct3 = 0x6;
  }
  else if (!strcmp(instruction, "bgeu"))
  {
    funct3 = 0x7;
  }
  else
  {
    return -1;
  }
  uint32_t code = opcode | (((offset >> 11) & 0x1) << 7) | (((offset >> 1) & 0xF) << 8) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20) | (((offset >> 5) & 0x3F) << 25) | (((offset >> 12) & 0x1) << 31);

  return code;
}

uint32_t get_code_U(char *instruction, char *token1, char *token2)
{
  if (!is_num(token2))
  {
    return -1;
  }
  uint32_t rd = get_register(token1);
  int32_t imm = atoi(token2);
  if (!check_register(rd) || (imm < 0) || (imm > 1048575))
  {
    return -1;
  }

  uint32_t opcode = -1;
  if (!strcmp(instruction, "auipc"))
  {
    opcode = 0x17;
  }
  else if (!strcmp(instruction, "lui"))
  {
    opcode = 0x37;
  }
  else
  {
    return -1;
  }
  uint32_t code = opcode | (rd << 7) | (imm << 12);

  return code;
}

uint32_t get_code_UJ(char *instruction, char *token1, char *token2)
{
  if (!is_num(token2))
  {
    return -1;
  }
  uint32_t rd = get_register(token1);
  int32_t imm = atoi(token2);
  if (!check_register(rd) || (imm < -1048576) || (imm > 1048575))
  {
    return -1;
  }

  uint32_t opcode = -1;
  if (!strcmp(instruction, "jal"))
  {
    opcode = 0x6F;
  }
  else
  {
    return -1;
  }
  uint32_t code = opcode | (rd << 7) | (((imm >> 12) & 0xFF) << 12) | (((imm >> 11) & 0x1) << 20) | (((imm >> 1) & 0x3FF) << 21) | (((imm >> 20) & 0x1) << 31);

  return code;
}

void dump_instruction(FILE *file_, uint32_t code)
{
  if (is_valid(code))
  {
    dump_code(file_, code);
  }
  else
  {
    dump_error_information(file_);
  }
}

/* DO NOT MODIFY CODES BELOW*/

/* Store code into file_ under fixed format */
void dump_code(FILE *file_, uint32_t code)
{
  fprintf(file_, "0x%.8X\n", code);
}

/* Output error information to file_ */
void dump_error_information(FILE *file_)
{
  fprintf(file_, "Assembling Error\n");
}
