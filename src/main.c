/* Includes */
#include <stdio.h>
#include <ram.h>
#include <cpu.h>

/* Consts */
/* The size of the RAM = 1GiB */
#define RAM_SIZE        (1024 * 1024 * 1024)

/* Entry point */
int main(int argc, char *argv[]) {
  struct ram ram;
  struct cpu cpu;
  ram_init(&ram, RAM_SIZE);
  cpu_init(&cpu, &ram);
  while (getchar() != 'q') {
    printf("\033[FINFO: Stepping...\n");
    cpu_step(&cpu);
  }
  cpu_free(&cpu);
  ram_free(&ram);
  return 0;
}
