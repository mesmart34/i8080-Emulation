#include "cpu8080.h"

void cpu8080_init(cpu8080* cpu)
{
	cpu->a = 0;
	cpu->b = 0;
	cpu->c = 0;
	cpu->d = 0;
	cpu->h = 0;
	cpu->l = 0;
	cpu->ac = 0;
	cpu->halted = 0;
	cpu->pc = 0;
	cpu->sp = 0;
	cpu->cf = 0;
	cpu->pf = 0;
	cpu->zf = 0;
	cpu->sf = 0;
}

void cpu8080_emulate(cpu8080* cpu)
{
	uint8_t* opcode = &cpu->memory[cpu->pc];
	switch (*opcode)
	{

	default:
		break;
	}
	cpu->pc++;
}
