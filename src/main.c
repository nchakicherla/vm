#include "chunk.h"
#include "opcode.h"
#include "debug.h"
#include "vm.h"

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

	VM vm;
	vm_init(&vm);
	VMResult r = vm_run(&vm, &chunk);
	printf("result: %s\n", vm_result_str(r));

	chunk_free(&chunk);

	return 0;
}