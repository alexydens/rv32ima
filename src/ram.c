/* Implements ram.h */
#include <ram.h>
#include <stdlib.h>

/* Initialize the RAM */
void ram_init(struct ram *ram, size_t ramsize) {
  ram->data = malloc(ramsize);
  ram->size = ramsize;
  ram->buffers = NULL;
  ram->buffer_count = 0;
}
/* Add a buffer */
void ram_add_buffer(struct ram *ram, struct buffer buffer) {
  if (!ram->buffers) {
    ram->buffers = malloc(sizeof(struct buffer));
    ram->buffers[0] = buffer;
    ram->buffer_count = 1;
    return;
  }
  ram->buffers = realloc(
      ram->buffers,
      sizeof(struct buffer) * (++(ram->buffer_count))
  );
  ram->buffers[ram->buffer_count - 1] = buffer;
}
/* Free the RAM */
void ram_free(struct ram *ram) {
  if (ram->buffers) free(ram->buffers);
  ram->buffers = NULL;
  ram->buffer_count = 0;
  free(ram->data);
  ram->data = NULL;
  ram->size = 0;
}

/* Read from RAM */
uint32_t ram_read(struct ram *ram, uint32_t address) {
  /* Check if value in any of the buffers */
  for (size_t i = 0; i < ram->buffer_count; i++) {
    if (
        address >= ram->buffers[i].address
        && address < ram->buffers[i].address + ram->buffers[i].size
    ) {
      return *(uint32_t *)
        ((uint8_t *)(ram->buffers[i].data)
         + (address - ram->buffers[i].address)
      );
    }
  }
  return *(uint32_t *)((uint8_t *)ram->data + address);
}
/* Write to RAM */
void ram_write(struct ram *ram, uint32_t address, uint32_t value) {
  /* Check if value in any of the buffers */
  for (size_t i = 0; i < ram->buffer_count; i++) {
    if (
        address >= ram->buffers[i].address
        && address < ram->buffers[i].address + ram->buffers[i].size
    ) {
      *(uint32_t *)
        ((uint8_t *)(ram->buffers[i].data)
         + (address - ram->buffers[i].address)
      ) = value;
      return;
    }
  }
  *(uint32_t *)((uint8_t *)ram->data + address) = value;
}
