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
#define LDAX_D 0x1a
#define LXI_H_D16 0x21
#define INX_H 0x23
#define MVI_H_D8 0x26
#define DAD_H 0x29
#define LXI_SP_D16 0x31
#define STA_addr 0x32
#define MVI_M_D8 0x36
#define LDA_addr 0x3a
#define MVI_A_D8 0x3e
#define MOV_D_M 0x56
#define MOV_E_M 0x5e
#define MOV_H_M 0x66
#define MOV_L_A 0x6f
#define MOV_M_A 0x77
#define MOV_A_D 0x7a
#define MOV_A_E 0x7b
#define MOV_A_H 0x7c
#define MOV_A_M 0x7e
#define ANA_A 0xa7
#define XRA_A 0xaf
#define POP_B 0xc1
#define JNZ_addr 0xc2
#define JMP_addr 0xc3
#define PUSH_B 0xc5
#define ADI_D8 0xc6
#define CALL_addr 0xcd
#define POP_D 0xd1
#define OUT_D8 0xd3
#define PUSH_D 0xd5
#define POP_H 0xe1
#define PUSH_H 0xe5
#define ANI_D8 0xe6
#define XCHG 0xeb
#define POP_PSW 0xf1
#define PUSH_PSW 0xf5
#define DI 0xf3
#define EI 0xfb
#define CPI_D8 0xfe

#define ADD_A 0x87
#define ADD_B 0x80
#define CPI_D8 0xfe
#define HLT 0x76
#define PRINT_B 0xfd

typedef struct cpu8080
{
	uint8_t b, c, d, h, l, e;
	uint16_t a, sp, pc;
	//sign, zero, parity, carry(cy, cf), auxillary carry
	uint8_t sf, zf, pf, cf, ac;
	bool halted, enable_int;
	uint8_t* memory;
} cpu8080;

void cpu8080_init(cpu8080* cpu);
void cpu8080_load_program(cpu8080* cpu, const uint8_t* buffer, int size, uint32_t  offset);
void cpu8080_emulate(cpu8080* cpu);
void cpu8080_run(cpu8080* cpu);
void cpu8080_disassembly(const char* buffer, int size);