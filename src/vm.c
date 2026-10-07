#include "vm.h"
#include "opcode.h"

#include <inttypes.h>
#include <stdio.h>

const char *vm_result_str(VMResult r) {
	switch (r) {
		case VM_OK:
			return "VM_OK";
		case VM_ERR_STACK_OVERFLOW:
			return "VM_ERR_STACK_OVERFLOW";
		case VM_ERR_STACK_UNDERFLOW:
			return "VM_ERR_STACK_UNDERFLOW";
		case VM_ERR_UNKNOWN_OPCODE:
			return "VM_ERR_UNKNOWN_OPCODE";
		case VM_ERR_MISSING_OPERAND:
			return "VM_ERR_MISSING_OPERAND";
		case VM_ERR_NO_HALT:
			return "VM_ERR_NO_HALT";
		default:
			return "UNKNOWN VMResult";
	}
}

void vm_init(VM *vm) {
	vm->chunk = NULL;
	vm->ip = 0;
	vm->sp = 0;
}

VMResult vm_push(VM *vm, Value v) {
	if (vm->sp == STACK_MAX) {
		return VM_ERR_STACK_OVERFLOW;
	}
	vm->stack[vm->sp] = v;
	vm->sp++;
	return VM_OK;
}

VMResult vm_pop(VM *vm, Value *out) {
	if (vm->sp == 0) {
		return VM_ERR_STACK_UNDERFLOW;
	}
	vm->sp--;
	*out = vm->stack[vm->sp];
	return VM_OK;
}

VMResult vm_run(VM *vm, const Chunk *chunk) {
	vm->chunk = chunk;
	vm->ip = 0;
	vm->sp = 0;

	VMResult res;

	while (vm->ip < chunk->len) {
		uint8_t op = chunk->bytes[vm->ip];
		vm->ip++;

		switch (op) {
			case OP_PUSH: {
				if (vm->ip >= chunk->len) {
					return VM_ERR_MISSING_OPERAND;
				}
				Value v = chunk->bytes[vm->ip];
				vm->ip++;
				if ((res = vm_push(vm, v)) != VM_OK) {
					return res;
				}
				break;
			}
			case OP_ADD: {
				Value a, b;
				if ((res = vm_pop(vm, &b)) != VM_OK) {
					return res;
				}
				if ((res = vm_pop(vm, &a)) != VM_OK) {
					return res;
				}
				if ((res = vm_push(vm, a + b)) != VM_OK) {
					return res;
				}
				break;
			}
			case OP_PRINT: {
				Value v;
				if ((res = vm_pop(vm, &v)) != VM_OK) {
					return res;
				}
				printf("%" PRId64 "\n", v);
				break;
			}
			case OP_HALT:
				return VM_OK;
			default:
				return VM_ERR_UNKNOWN_OPCODE;
		}
	}
	return VM_ERR_NO_HALT;
}