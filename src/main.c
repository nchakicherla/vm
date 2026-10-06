#include "chunk.h"
#include "opcode.h"
#include "debug.h"

#include <stdio.h>

int main(void) {
	Chunk chunk;
	chunk_init(&chunk);

	chunk_write(&chunk, OP_PUSH);
	chunk_write(&chunk, 5);
	chunk_write(&chunk, OP_PUSH);
	chunk_write(&chunk, 3);
	chunk_write(&chunk, OP_ADD);
	chunk_write(&chunk, OP_PRINT);
	chunk_write(&chunk, OP_HALT);

	disassemble_chunk(&chunk, "TEST CHUNK");

	chunk_free(&chunk);

	Chunk chunk_broken;
	chunk_init(&chunk_broken);

	chunk_write(&chunk_broken, OP_ADD);
	chunk_write(&chunk_broken, OP_PUSH);

	disassemble_chunk(&chunk_broken, "BROKEN CHUNK");

	chunk_free(&chunk_broken);

	return 0;
}