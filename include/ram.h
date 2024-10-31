/* Include guard */
#ifndef RAM_H
#define RAM_H

/* Includes */
#include <stdint.h>
#include <stddef.h>

/* Buffer structure */
struct buffer {
  void *data;               /* The data supplied by the user */
  uint32_t address;         /* The address to map it to */
  size_t size;              /* The size of the buffer */
};

/* RAM structure */
struct ram {
  void *data;               /* The data in RAM that isn't in a buffer */
  size_t size;              /* The size of the RAM */
  struct buffer *buffers;   /* All memory mapped to a user supplied buffer */
  size_t buffer_count;      /* The number of buffers */
};

/* Initialize the RAM */
extern void ram_init(struct ram *ram, size_t ramsize);
/* Add a buffer */
extern void ram_add_buffer(struct ram *ram, struct buffer buffer);
/* Free the RAM */
extern void ram_free(struct ram *ram);

/* Read from RAM */
extern uint32_t ram_read(struct ram *ram, uint32_t address);
/* Write to RAM */
extern void ram_write(struct ram *ram, uint32_t address, uint32_t value);

#endif /* RAM_H */
