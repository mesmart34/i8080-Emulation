#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <memory>

enum Instructions
{
	NOP = 0x00,
	LXI_B_D16 = 0x01,
	DCR_B = 0x05,
	DAD_B = 0x09,
	DCR_C = 0x0d,
	RRC = 0x0f,
	RAR = 0x1f,
	LXI_D_D16 = 0x11,
	INX_D = 0x13,
	DAD_D = 0x19,
	LDAX_D = 0x1a,
	LXI_H_D16 = 0x21,
	INX_H = 0x23,
	DAD_H = 0x29,
	LXI_SP_D16 = 0x31,
	STA_addr = 0x32,
	LDA_addr = 0x3a,
	MOV_A_A = 0x7F,
	MOV_A_B = 0x78,
	MOV_A_C = 0x79,
	MOV_A_D = 0x7a,
	MOV_A_E = 0x7b,
	MOV_A_H = 0x7c,
	MOV_A_L = 0x7D,
	MOV_A_M = 0x7e,

	MOV_B_A = 0x47,
	MOV_B_B = 0x40,
	MOV_B_C = 0x41,
	MOV_B_D = 0x42,
	MOV_B_E = 0x43,
	MOV_B_H = 0x44,
	MOV_B_L = 0x45,
	MOV_B_M = 0x46,

	MOV_C_A = 0x4F,
	MOV_C_B = 0x48,
	MOV_C_C = 0x49,
	MOV_C_D = 0x4A,
	MOV_C_E = 0x4B,
	MOV_C_H = 0x4C,
	MOV_C_L = 0x4D,
	MOV_C_M = 0x4E,

	MOV_D_A = 0x57,
	MOV_D_B = 0x50,
	MOV_D_C = 0x51,
	MOV_D_D = 0x52,
	MOV_D_E = 0x53,
	MOV_D_H = 0x54,
	MOV_D_L = 0x55,
	MOV_D_M = 0x56,

	MOV_E_A = 0x5F,
	MOV_E_B = 0x58,
	MOV_E_C = 0x59,
	MOV_E_D = 0x5A,
	MOV_E_E = 0x5B,
	MOV_E_H = 0x5C,
	MOV_E_L = 0x5D,
	MOV_E_M = 0x5E,

	MOV_H_A = 0x67,
	MOV_H_B = 0x60,
	MOV_H_C = 0x61,
	MOV_H_D = 0x62,
	MOV_H_E = 0x63,
	MOV_H_H = 0x64,
	MOV_H_L = 0x65,
	MOV_H_M = 0x66,

	MOV_L_A = 0x6F,
	MOV_L_B = 0x68,
	MOV_L_C = 0x69,
	MOV_L_D = 0x6A,
	MOV_L_E = 0x6B,
	MOV_L_H = 0x6C,
	MOV_L_L = 0x6D,
	MOV_L_M = 0x6E,

	MOV_M_A = 0x77,
	MOV_M_B = 0x70,
	MOV_M_C = 0x71,
	MOV_M_D = 0x72,
	MOV_M_E = 0x73,
	MOV_M_H = 0x74,
	MOV_M_L = 0x75,

	MVI_A_D8 = 0x3E,
	MVI_B_D8 = 0x06,
	MVI_C_D8 = 0x0E,
	MVI_D_D8 = 0x16,
	MVI_E_D8 = 0x1E,
	MVI_H_D8 = 0x26,
	MVI_L_D8 = 0x2E,
	MVI_M_D8 = 0x36,
	ANA_A = 0xa7,
	XRA_A = 0xaf,
	POP_B = 0xc1,
	JNZ_addr = 0xc2,
	JMP_addr = 0xc3,
	PUSH_B = 0xc5,
	ADI_D8 = 0xc6,
	JZ_addr = 0xca,
	CALL_addr = 0xcd,
	POP_D = 0xd1,
	OUT_D8 = 0xd3,
	PUSH_D = 0xd5,
	POP_H = 0xe1,
	PUSH_H = 0xe5,
	ANI_D8 = 0xe6,
	XCHG = 0xeb,
	POP_PSW = 0xf1,
	PUSH_PSW = 0xf5,
	DI = 0xf3,
	EI = 0xfb,
	CPI_D8 = 0xfe,
	RET = 0xc9,
	ADD_A = 0x87,
	ADD_B = 0x80,
	ADD_C = 0x81,
	ADD_D = 0x82,
	ADD_E = 0x83,
	ADD_H = 0x84,
	ADD_L = 0x85,
	ADD_M = 0x86,
	SUB_A = 0x97,
	SUB_B = 0x90,
	SUB_C = 0x91,
	SUB_D = 0x92,
	SUB_E = 0x93,
	SUB_H = 0x94,
	SUB_L = 0x95,
	SBB_A = 0x9f,
	SBB_B = 0x98,
	SBB_C = 0x99,
	SBB_D = 0x9a,
	SBB_E = 0x9b,
	SBB_H = 0x9c,
	SBB_L = 0x9d,
	CNZ = 0xc4,
	CZ = 0xcc,
	DCR_A = 0x3d,
	RNC = 0xd0,
	RC = 0xd8,
	CNC = 0xd4,
	JC = 0xda,
	RPO = 0xe0,
	INR_A = 0x3c,
	INR_B = 0x04,
	INR_C = 0x0c,
	INR_D = 0x14,
	INR_E = 0x1c,
	INR_H = 0x24,
	INR_L = 0x2c,
	INR_M = 0x34,
	RZ = 0xc8,
	RNZ = 0xc0,
	HLT = 0x76,
	PRINT_A = 0x10,
	PRINT_B = 0x20,
	PRINT_C = 0x30,
};

class CPU8080
{
public:
	CPU8080();

	void load_program(const uint8_t* buffer, const size_t size, const uint32_t offset);
	void run(const uint32_t start_addr);
	void disassembly(uint8_t* buffer, const size_t size);

private:
	void emulate();
	inline void set_logic_flags();
	inline void set_arithmetic_flags(const uint16_t res);
	inline void set_bc(const uint16_t value);
	inline void set_hl(const uint16_t value);
	inline void set_de(const uint16_t value);
	inline void push_stack(const uint16_t value);
	inline void jump(const uint16_t addr);
	inline void call(const uint16_t addr);
	inline void ret();
	inline void add(const uint8_t value);
	inline void sub(const uint8_t value);
	inline void wb(const uint8_t value, const uint16_t addr);
	inline void ww(const uint16_t value, const uint16_t addr);
	inline void sbb(const uint8_t reg);
	inline uint8_t dec(const uint8_t reg);
	inline uint8_t rb(const uint16_t addr) const;
	inline uint16_t get_de() const;
	inline uint16_t rw(const uint16_t addr) const;
	inline uint16_t get_hl() const;
	inline uint16_t get_bc() const;
	inline int32_t parity(int x, const int size);

private:
	uint8_t b, c, d, h, l, e;
	uint16_t a, sp, pc;
	//sign, zero, parity, carry(cy, cf), auxillary carry
	uint8_t sf, zf, pf, cf, ac;
	bool halted, enable_int;
	uint8_t* memory;
};