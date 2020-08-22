#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#define NOP 0x00
#define DCR_B 0x05
#define MVI_B_D8 0x06
#define MVI_A_D8 0x3e
#define DCR_C 0x0d
#define MVI_C_D8 0x0e
#define RRC 0x0f
#define RAR 0x1f
#define LXI_D_D16 0x11
#define INX_D 0x13
#define DAD_D 0x19


#define ADD_A 0x87
#define ADD_B 0x80
#define CPI_D8 0xfe
#define JMP 0xc3
#define HLT 0x76
#define PRINT_A 0xfd

typedef struct cpu8080
{
	uint8_t b, c, d, h, l, e;
	uint16_t a, sp, pc;
	//sign, zero, parity, carry(cy, cf), auxillary carry
	uint8_t sf, zf, pf, cf, ac;
	bool halted;
	uint8_t* memory;
} cpu8080;

void cpu8080_init(cpu8080* cpu);
void cpu8080_load_program(cpu8080* cpu, const uint8_t* buffer, int size, uint32_t  offset);
void cpu8080_emulate(cpu8080* cpu);
void cpu8080_run(cpu8080* cpu);
void cpu8080_disassembly(const char* buffer, int size);