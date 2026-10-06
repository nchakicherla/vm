#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

void disassemble_chunk(const Chunk *chunk, const char *name);
size_t disassemble_instruction(const Chunk *chunk, size_t offset);

#endif // DEBUG_H
