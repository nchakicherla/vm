# How the VM Works

A walkthrough of what the VM does, step by step, using the test chunk `PUSH 5, PUSH 3, ADD, PRINT, HALT`.

## The idea

The VM is a loop that repeats three steps:

1. **Fetch:** read the byte at `ip`.
2. **Decode:** work out which instruction it is (the `switch`).
3. **Execute:** do what that instruction means, which usually changes the stack and moves `ip` forward.

It stops when it reaches `HALT` or hits an error. A real CPU works the same way; its fetch, decode and execute are done in hardware.

## The three pieces of state

### 1. The chunk (read-only)

These are the bytes the program is made of:

```
index:  0     1   2     3   4    5      6
byte:   PUSH  5   PUSH  3   ADD  PRINT  HALT
```

The VM never changes them. It only reads them.

### 2. `ip` (instruction pointer)

This is an index into the chunk: "the next byte I'll read". It starts at 0. Every time the VM reads a byte, whether that byte is an opcode or an operand, `ip` moves forward by one. Later, when you add jumps, a jump is just setting `ip` to a different number.

### 3. The stack

This is where values live while the program computes with them:

```
stack:  [ _ | _ | _ | _ | ... ]   (256 slots)
sp = 0  → points at the next free slot
```

- **Push:** write the value at `stack[sp]`, then `sp++`.
- **Pop:** `sp--`, then read `stack[sp]`.
- `sp` is also the number of values on the stack. `sp == 0` means it's empty, and `sp == 256` means it's full.

## Tracing the test program

**Start:** `ip = 0`, stack `[ ]`, `sp = 0`

**Loop 1:** fetch `bytes[0]` = `PUSH`, then `ip` → 1.
`PUSH` means "the next byte is a value; push it." Read `bytes[1]` = `5`, then `ip` → 2. Push 5.

```
stack: [ 5 ]          sp = 1      ip = 2
```

**Loop 2:** fetch `bytes[2]` = `PUSH`, then `ip` → 3. Read the operand `bytes[3]` = `3`, then `ip` → 4. Push 3.

```
stack: [ 5 | 3 ]      sp = 2      ip = 4
```

**Loop 3:** fetch `bytes[4]` = `ADD`, then `ip` → 5. `ADD` has no operand: its inputs are already on the stack.

- pop → `b = 3`, giving stack `[ 5 ]`
- pop → `a = 5`, giving stack `[ ]`
- push `a + b` = 8

```
stack: [ 8 ]          sp = 1      ip = 5
```

**Loop 4:** fetch `bytes[5]` = `PRINT`, then `ip` → 6. Pop 8 and print it.

```
stack: [ ]            sp = 0      ip = 6        output: 8
```

**Loop 5:** fetch `bytes[6]` = `HALT`, then `ip` → 7. Return `VM_OK`.

That's the whole run. The disassembler and the VM step through the bytes the same way: both read the opcode, then any operands. The difference is that the disassembler *prints* what each instruction means and the VM *does* it.

## Why a stack?

Look at what `ADD` doesn't need to know. It doesn't say *where* its inputs are; it just takes the top two values. That's what keeps the bytecode simple: most instructions need no operands at all.

It also matches how expressions nest. `(1 + 2) * (3 + 4)` becomes:

```
PUSH 1   PUSH 2   ADD      → stack: [3]
PUSH 3   PUSH 4   ADD      → stack: [3, 7]
MUL                        → stack: [21]
```

Each part of the expression leaves its result on the stack, ready for the next operation to use. That's why compilers find a stack VM easy to target: they walk the expression tree and emit an instruction for each node.

## Pop order matters

The first value you pop is the one pushed **last**. For `PUSH 10, PUSH 3, SUB`, you want `10 - 3`:

- pop → `b = 3` (the right-hand side)
- pop → `a = 10` (the left-hand side)
- push `a - b` = 7

If you swap them, you get `-7`. With `ADD` the order makes no difference, so the bug stays hidden until you add `SUB` or `DIV`. Getting it right now saves you that bug later.

## What can go wrong, and where

| Error | When it happens | Example |
|---|---|---|
| **Stack underflow** | popping from an empty stack | `PUSH 1, ADD`: ADD's second pop has nothing to take |
| **Stack overflow** | pushing when `sp == 256` | 257 `PUSH`es in a row |
| **Missing operand** | `PUSH` is the last byte | `ADD, PUSH`, the same case as in the disassembler |
| **No HALT** | `ip` reaches `len` without hitting `HALT` | `PUSH 5, PRINT` with no `HALT` |
| **Unknown opcode** | the byte isn't a known instruction | a `99` in the bytecode |

Every one of these, if you don't check for it, reads or writes memory outside an array. That's undefined behavior in C, and it's exactly what valgrind catches. The VM must never trust its bytecode. Later the bytecode will come from a compiler or a file, and either one can be wrong.

## Why `HALT` is needed

Couldn't the VM just stop when `ip` reaches the end? It could, but having an explicit `HALT` instruction means:

- "The program finished" and "the bytecode ended early" become two different outcomes, and the second one is a bug you want to hear about.
- Once you add jumps, the end of the chunk isn't necessarily where the program finishes. Code can jump over the last bytes, or a `HALT` can sit in the middle.

## The two-level picture

Two "programs" are running at once, and keeping them apart helps:

- **The C code** (the VM) is the *interpreter*. It always runs the same loop.
- **The bytecode** is the *program being interpreted*. It's just data that the loop reads.

When you debug, ask which level the bug is at. Either the VM is wrong (a C bug) or the bytecode is wrong (bad input that the VM should reject cleanly). The disassembler shows you the bytecode level, and a trace mode, which prints `ip` and the stack each step, shows you the VM working through it.

## Check yourself

Without running anything, trace this program on paper:

```
PUSH 2, PUSH 3, PUSH 4, ADD, ADD, PRINT, HALT
```

Write out the stack and `ip` after each instruction. If you get `9` printed and an empty stack at the end, you understand it well enough to write the VM.
