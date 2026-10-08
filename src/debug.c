#include "debug.h"
#include "opcode.h"

#include <stdio.h>

void disassemble_chunk(const Chunk *chunk, const char *name) {
	printf("CHUNK: %s\n", name);

	for (size_t offset = 0; offset < chunk->len; ) {
		offset = disassemble_instruction(chunk, offset);
	}
}

size_t disassemble_instruction(const Chunk *chunk, size_t offset) {
	printf("%04zu ", offset);

	uint8_t op = chunk->bytes[offset];
	switch (op) {
		case OP_PUSH:
			if (offset + 1 >= chunk->len) {
				fprintf(stdout, "[ERROR] OP_PUSH byte not followed by operand\n");
				return chunk->len;
			}
			printf("PUSH %d\n", chunk->bytes[offset + 1]);
			return offset + 2;
		case OP_ADD:
			printf("ADD\n");
			return offset + 1;
		case OP_SUB:
			printf("SUB\n");
			return offset + 1;
		case OP_MUL:
			printf("MUL\n");
			return offset + 1;
		case OP_DIV:
			printf("DIV\n");
			return offset + 1;
		case OP_PRINT:
			printf("PRINT\n");
			return offset + 1;
		case OP_HALT:
			printf("HALT\n");
			return offset + 1;
		default:
			printf("UNKNOWN OPCODE %d\n", op);
			return offset + 1;
	}
}
