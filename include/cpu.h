/* Include guard */
#ifndef CPU_H
#define CPU_H

/* Includes */
#include <stdint.h>
#include <stdio.h>
#include <ram.h>

/* CPU structure */
struct cpu {
  /* The registers */
  struct {
    union {
      uint32_t r[32];                         /* General purpose registers */
      struct {
        uint32_t zero;                        /* Always zero */
        uint32_t ra;                          /* Return address */
        uint32_t sp;                          /* Stack pointer */
        uint32_t gp;                          /* Global pointer */
        uint32_t tp;                          /* Thread pointer */
        uint32_t t0;                          /* Temporary register 0 */
        uint32_t t1;                          /* Temporary register 1 */
        uint32_t t2;                          /* Temporary register 2 */
        union { uint32_t s0; uint32_t fp; };  /* Saved register 0 / frame ptr */
        uint32_t s1;                          /* Saved register 1 */
        uint32_t a0;                          /* Function argument 0 */
        uint32_t a1;                          /* Function argument 1 */
        uint32_t a2;                          /* Function argument 2 */
        uint32_t a3;                          /* Function argument 3 */
        uint32_t a4;                          /* Function argument 4 */
        uint32_t a5;                          /* Function argument 5 */
        uint32_t a6;                          /* Function argument 6 */
        uint32_t a7;                          /* Function argument 7 */
        uint32_t s2;                          /* Saved register 2 */
        uint32_t s3;                          /* Saved register 3 */
        uint32_t s4;                          /* Saved register 4 */
        uint32_t s5;                          /* Saved register 5 */
        uint32_t s6;                          /* Saved register 6 */
        uint32_t s7;                          /* Saved register 7 */
        uint32_t s8;                          /* Saved register 8 */
        uint32_t s9;                          /* Saved register 9 */
        uint32_t s10;                         /* Saved register 10 */
        uint32_t s11;                         /* Saved register 11 */
        uint32_t t3;                          /* Temporary register 3 */
        uint32_t t4;                          /* Temporary register 4 */
        uint32_t t5;                          /* Temporary register 5 */
        uint32_t t6;                          /* Temporary register 6 */
      };
    };
    uint32_t pc;                              /* Program counter */
  } regs;
  struct ram *ram;
};

/* Initialize a CPU */
extern void cpu_init(struct cpu *cpu, struct ram *ram);
/* Step one cycle with CPU */
extern void cpu_step(struct cpu *cpu);
/* Free any resources used by the CPU */
extern void cpu_free(struct cpu *cpu);

#endif /* CPU_H */
