#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

typedef struct cpu8080
{
	uint8_t a, b, c, d, h, l, e;
	uint16_t sp, pc;
	uint8_t sf, zf, pf, cf, ac;
	bool halted;
	uint8_t* memory;
} cpu8080;

void cpu8080_init(cpu8080* cpu);
void cpu8080_load_program(cpu8080* cpu, const uint8_t* buffer, int size, int offset);
void cpu8080_emulate(cpu8080* cpu);
void cpu8080_run(cpu8080* cpu);
void cpu8080_disassembly(const char* buffer, uint16_t pc);