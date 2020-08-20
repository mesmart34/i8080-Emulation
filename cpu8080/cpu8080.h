#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct cpu8080
{
	uint8_t a, b, c, d, h, l;
	uint16_t sp, pc;
	uint8_t sf, zf, pf, cf, ac;
	bool halted;
	uint8_t* memory;
} cpu8080;

void cpu8080_init(cpu8080* cpu);
void cpu8080_emulate(cpu8080* cpu);