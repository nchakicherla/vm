#ifndef VM_H
#define VM_H

#include <stddef.h>
#include <stdint.h>

#include "chunk.h"

#define STACK_MAX 256

typedef int64_t Value;

typedef enum {
	VM_OK,
	VM_ERR_STACK_OVERFLOW,
	VM_ERR_STACK_UNDERFLOW,
	VM_ERR_UNKNOWN_OPCODE,
	VM_ERR_MISSING_OPERAND,
	VM_ERR_DIV_BY_ZERO,
	VM_ERR_INT_OVERFLOW,
	VM_ERR_NO_HALT,
} VMResult;

typedef struct VM {
	const Chunk *chunk;
	size_t ip;
	Value stack[STACK_MAX];
	size_t sp;
} VM;

const char *vm_result_str(VMResult r);
void vm_init(VM *vm);
VMResult vm_push(VM *vm, Value v);
VMResult vm_pop(VM *vm, Value *out);
VMResult vm_run(VM *vm, const Chunk *chunk);

#endif // VM_H
