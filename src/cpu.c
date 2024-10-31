/* Implements cpu.h */
#include <cpu.h>
#include <string.h>

/* Instruction formats */
/* R-type */
struct r_type_instr {
  uint32_t opcode:    7;
  uint32_t rd:        5;
  uint32_t funct3:    3;
  uint32_t rs1:       5;
  uint32_t rs2:       5;
  uint32_t funct7:    7;
} __attribute__((packed));
/* I-type */
struct i_type_instr {
  uint32_t opcode:    7;
  uint32_t rd:        5;
  uint32_t funct3:    3;
  uint32_t rs1:       5;
  uint32_t imm_0_11:  12;
} __attribute__((packed));
/* S-type */
struct s_type_instr {
  uint32_t opcode:    7;
  uint32_t imm_0_4:   5;
  uint32_t funct3:    3;
  uint32_t rs1:       5;
  uint32_t rs2:       5;
  uint32_t imm_5_11:  7;
} __attribute__((packed));
/* B-type */
struct b_type_instr {
  uint32_t opcode:    7;
  uint32_t imm_11:    1;
  uint32_t imm_1_4:   4;
  uint32_t funct3:    3;
  uint32_t rs1:       5;
  uint32_t rs2:       5;
  uint32_t imm_5_10:  6;
  uint32_t imm_12:    1;
} __attribute__((packed));
/* U-type */
struct u_type_instr {
  uint32_t opcode:    7;
  uint32_t rd:        5;
  uint32_t imm_12_31: 20;
} __attribute__((packed));
/* J-type */
struct j_type_instr {
  uint32_t opcode:    7;
  uint32_t rd:        5;
  uint32_t imm_12_19: 8;
  uint32_t imm_11:    1;
  uint32_t imm_1_10:  11;
  uint32_t imm_20:    1;
} __attribute__((packed));

/* Get immediate from I-type instruction */
static inline uint32_t i_type_imm(struct i_type_instr instr) {
  /* Bit 11 determines bits 12-31 */
  return instr.imm_0_11 | ((instr.imm_0_11 >> 11) ? 0xFFFFF000 : 0);
}
/* Get immediate from S-type instruction */
static inline uint32_t s_type_imm(struct s_type_instr instr) {
  /* Bit 11 determines bits 12-31 */
  return 
    (instr.imm_0_4 | (instr.imm_5_11 << 5))
    | ((instr.imm_5_11 >> 7) ? 0xFFFFF000 : 0);
}
/* Get immediate from B-type instruction */
static inline uint32_t b_type_imm(struct b_type_instr instr) {
  /* Bit 12 determines bits 13-31 */
  return
    (instr.imm_1_4 | (instr.imm_5_10 << 5) | (instr.imm_12 << 11))
    | ((instr.imm_12) ? 0xFFFFE000 : 0);
}
/* Get immediate from U-type instruction */
static inline uint32_t u_type_imm(struct u_type_instr instr) {
  return instr.imm_12_31 << 12;
}
/* Get immediate from J-type instruction */
static inline uint32_t j_type_imm(struct j_type_instr instr) {
  return
    (instr.imm_1_10 | (instr.imm_11 << 11) | (instr.imm_20 << 20))
    | ((instr.imm_20) ? 0xFFC00000 : 0);
}

/* Initialize a CPU */
void cpu_init(struct cpu *cpu, struct ram *ram) {
  /* Set it all to zero */
  memset(cpu, 0, sizeof(struct cpu));
  /* Set the RAM */
  cpu->ram = ram;
}

/* Step one cycle with CPU */
void cpu_step(struct cpu *cpu) {
  /* Get the instruction */
  uint32_t instr = ram_read(cpu->ram, cpu->regs.pc);
  /* Get opcode */
  uint8_t opcode = instr & 0x7F;

  /* Find instruction */
  switch (opcode) {
    case 0x37:  /* LUI rd, imm12 */
      {
        struct u_type_instr u = *(struct u_type_instr *)&instr;
        cpu->regs.r[u.rd] = u_type_imm(u);
      } break;
    case 0x17:  /* AUIPC rd, imm12 */
      {
        struct u_type_instr u = *(struct u_type_instr *)&instr;
        cpu->regs.r[u.rd] = u_type_imm(u) + cpu->regs.pc;
      } break;
    case 0x6F:  /* JAL rd, imm20 */
      {
        struct j_type_instr j = *(struct j_type_instr *)&instr;
        cpu->regs.r[j.rd] = cpu->regs.pc + 4;
        cpu->regs.pc += j_type_imm(j);
        return; /* Do not increment program counter */
      } break;
    case 0x67:  /* JALR rd, rs1, imm12 */
      {
        struct i_type_instr i = *(struct i_type_instr *)&instr;
        cpu->regs.r[i.rd] = cpu->regs.pc + 4;
        cpu->regs.pc = cpu->regs.r[i.rs1] + i_type_imm(i);
        return; /* Do not increment program counter */
      } break;
    case 0x63:  /* Branches */
      {
        struct b_type_instr b = *(struct b_type_instr *)&instr;
        uint32_t rs1_u = cpu->regs.r[b.rs1];
        uint32_t rs2_u = cpu->regs.r[b.rs2];
        int32_t rs1_i = *((int32_t*)&rs1_u);
        int32_t rs2_i = *((int32_t*)&rs2_u);
        switch (b.funct3) {
          case 0x0:  /* BEQ */
            if (rs1_u == rs2_u) cpu->regs.pc += b_type_imm(b);
            break;
          case 0x1:  /* BNE */
            if (rs1_u != rs2_u) cpu->regs.pc += b_type_imm(b);
            break;
          case 0x4:  /* BLT */
            if (rs1_i < rs2_i) cpu->regs.pc += b_type_imm(b);
            break;
          case 0x5:  /* BGE */
            if (rs1_i >= rs2_i) cpu->regs.pc += b_type_imm(b);
            break;
          case 0x6:  /* BLTU */
            if (rs1_u < rs2_u) cpu->regs.pc += b_type_imm(b);
            break;
          case 0x7:  /* BGEU */
            if (rs1_u >= rs2_u) cpu->regs.pc += b_type_imm(b);
            break;
        }
        return; /* Do not increment program counter */
      } break;
    case 0x03:  /* Loads */
      {
        struct i_type_instr i = *(struct i_type_instr *)&instr;
        switch (i.funct3) {
          case 0:    /* LB */
            cpu->regs.r[i.rd] =
              ram_read(cpu->ram, cpu->regs.r[i.rs1] + i_type_imm(i)) & 0xFF;
            break;
          case 1:    /* LH */
            cpu->regs.r[i.rd] =
              ram_read(cpu->ram, cpu->regs.r[i.rs1] + i_type_imm(i)) & 0xFFFF;
            break;
          case 2:    /* LW */
            cpu->regs.r[i.rd] =
              ram_read(cpu->ram, cpu->regs.r[i.rs1] + i_type_imm(i));
            break;
          case 4:    /* LBU */
            cpu->regs.r[i.rd] =
              ram_read(cpu->ram, cpu->regs.r[i.rs1] + i_type_imm(i)) & 0xFF;
            if (cpu->regs.r[i.rd] >> 7) cpu->regs.r[i.rd] |= 0xFFFFFF00;
            break;
          case 5:    /* LHU */
            cpu->regs.r[i.rd] =
              ram_read(cpu->ram, cpu->regs.r[i.rs1] + i_type_imm(i)) & 0xFFFF;
            if (cpu->regs.r[i.rd] >> 15) cpu->regs.r[i.rd] |= 0xFFFF0000;
            break;
        }
      } break;
  }

  /* Increment program counter */
  cpu->regs.pc += 4;
}

/* Free any resources used by the CPU */
void cpu_free(struct cpu *cpu) {
  /* Nothing to do here atm */
}
