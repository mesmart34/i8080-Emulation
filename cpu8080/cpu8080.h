#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#define NOP 0x00
#define LXI_B_D16 0x01
#define DCR_B 0x05
#define MVI_B_D8 0x06
#define DAD_B 0x09
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

#define MOV_A_A 0x7F
#define MOV_A_B 0x78
#define MOV_A_C 0x79
#define MOV_A_D 0x7a
#define MOV_A_E 0x7b
#define MOV_A_H 0x7c
#define MOV_A_L 0x7D
#define MOV_A_M 0x7e

#define MOV_B_A 0x47
#define MOV_B_B 0x40
#define MOV_B_C 0x41
#define MOV_B_D 0x42
#define MOV_B_E 0x43
#define MOV_B_H 0x44
#define MOV_B_L 0x45
#define MOV_B_M 0x46

#define MOV_C_A 0x4F
#define MOV_C_B 0x48
#define MOV_C_C 0x49
#define MOV_C_D 0x4A
#define MOV_C_E 0x4B
#define MOV_C_H 0x4C
#define MOV_C_L 0x4D
#define MOV_C_M 0x4E

#define MOV_D_A 0x57
#define MOV_D_B 0x50
#define MOV_D_C 0x51
#define MOV_D_D 0x52
#define MOV_D_E 0x53
#define MOV_D_H 0x54
#define MOV_D_L 0x55
#define MOV_D_M 0x56

#define MOV_E_A 0x5F	
#define MOV_E_B 0x58
#define MOV_E_C 0x59
#define MOV_E_D 0x5A
#define MOV_E_E 0x5B
#define MOV_E_H 0x5C
#define MOV_E_L 0x5D
#define MOV_E_M 0x5E

#define MOV_H_A 0x67	
#define MOV_H_B 0x60
#define MOV_H_C 0x61
#define MOV_H_D 0x62
#define MOV_H_E 0x63
#define MOV_H_H 0x64
#define MOV_H_L 0x65
#define MOV_H_M 0x66

#define MOV_L_A 0x6F	
#define MOV_L_B 0x68
#define MOV_L_C 0x69
#define MOV_L_D 0x6A
#define MOV_L_E 0x6B
#define MOV_L_H 0x6C
#define MOV_L_L 0x6D
#define MOV_L_M 0x6E

#define MOV_M_A 0x77	
#define MOV_M_B 0x70
#define MOV_M_C 0x71
#define MOV_M_D 0x72
#define MOV_M_E 0x73
#define MOV_M_H 0x74
#define MOV_M_L 0x75


#define ANA_A 0xa7
#define XRA_A 0xaf
#define POP_B 0xc1
#define JNZ_addr 0xc2
#define JMP_addr 0xc3
#define PUSH_B 0xc5
#define ADI_D8 0xc6
#define JZ_addr 0xca
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
#define RET 0xc9
#define ADD_A 0x87
#define ADD_B 0x80
#define ADD_C 0x81
#define ADD_D 0x82
#define ADD_E 0x83
#define ADD_H 0x84
#define ADD_L 0x85
#define ADD_M 0x86
#define SUB_A 0x97
#define SUB_B 0x90
#define SUB_C 0x91
#define SUB_D 0x92
#define SUB_E 0x93
#define SUB_H 0x94
#define SUB_L 0x95
#define SBB_A 0x9f
#define SBB_B 0x98
#define SBB_C 0x99
#define SBB_D 0x9a
#define SBB_E 0x9b
#define SBB_H 0x9c
#define SBB_L 0x9d
#define CNZ 0xc4
#define CZ 0xcc
#define DCR_A 0x3d
#define RNC 0xd0
#define RC 0xd8
#define CNC 0xd4
#define JC 0xda
#define RPO 0xe0
#define INR_A 0x3c
#define INR_B 0x04
#define INR_C 0x0c
#define INR_D 0x14
#define INR_E 0x1c
#define INR_H 0x24
#define INR_L 0x2c
#define INR_M 0x34
#define RZ 0xc8
#define RNZ 0xc0

#define ADD_A 0x87
#define ADD_B 0x80
#define CPI_D8 0xfe
#define HLT 0x76
#define PRINT_A 0x10
#define PRINT_B 0x20
#define PRINT_C 0x30


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
void cpu8080_disassembly(cpu8080* cpu, const char* buffer, int size);