#include "chunk.h"

#include <stdio.h>
#include <stdlib.h>

void chunk_init(Chunk *chunk) {
	chunk->bytes = NULL;
	chunk->len = 0;
	chunk->cap = 0;
}

bool chunk_write(Chunk *chunk, uint8_t byte) {
	if (chunk->len == chunk->cap) {
		size_t new_cap = (chunk->cap == 0) ? 8 : chunk->cap * 2;
		uint8_t *new_ptr = realloc(chunk->bytes, new_cap);
		if (!new_ptr) {
			fprintf(stderr, "[ERROR] chunk->bytes realloc failed\n");
			return false;
		}
		chunk->bytes = new_ptr;
		chunk->cap = new_cap;
	}
	chunk->bytes[chunk->len] = byte;
	chunk->len++;
	return true;
}

void chunk_free(Chunk *chunk) {
	if (chunk->bytes) {
		free(chunk->bytes);
	}
	chunk->bytes = NULL;
	chunk->len = 0;
	chunk->cap = 0;
}
