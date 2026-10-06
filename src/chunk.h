#ifndef CHUNK_H
#define CHUNK_H

#include "opcode.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Chunk {
	uint8_t *bytes;
	size_t len;
	size_t cap;
} Chunk;

void chunk_init(Chunk *chunk);
bool chunk_write(Chunk *chunk, uint8_t byte);
void chunk_free(Chunk *chunk);

#endif // CHUNK_H
