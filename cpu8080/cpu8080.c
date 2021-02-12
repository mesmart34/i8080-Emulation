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
	cpu->memory = malloc(0x10000);
}

void cpu8080_load_program(cpu8080* cpu, const uint8_t* buffer, int size, uint32_t  offset)
{
	memcpy(cpu->memory + offset, buffer, size);
}

static inline int32_t parity(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1 << size) - 1));
	for (i = 0; i < size; i++)
	{
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

static inline void set_logic_flags(cpu8080* cpu)
{
	cpu->cf = cpu->ac = 0;
	cpu->zf = (cpu->a == 0);
	cpu->sf = (0x80 == (cpu->a & 0x80));
	cpu->pf = parity(cpu->a, 8);
}

static inline void set_arithmetic_flags(cpu8080* cpu, uint16_t res)
{
	cpu->cf = res > 0xff;
	cpu->zf = (res & 0xff) == 0;
	cpu->sf = (0x80 == (res & 0x80));
	cpu->pf = parity(res & 0xff, 8);
}

static void debug(cpu8080* cpu)
{
	cpu8080_disassembly(cpu, cpu->memory, cpu->pc);
	printf("\tA = %04X, B = %04X, C = %04X, D = %04X, E = %04X\n",
		cpu->a, cpu->b, cpu->c, cpu->d, cpu->e);
}

static inline void cpu8080_set_bc(cpu8080* cpu, uint16_t value)
{
	cpu->b = value >> 8;
	cpu->c = value & 0xff;
}

static inline void cpu8080_set_hl(cpu8080* cpu, uint16_t value)
{
	cpu->h = value >> 8;
	cpu->l = value & 0xff;
}

static inline void cpu8080_set_de(cpu8080* cpu, uint16_t value)
{
	cpu->d = value >> 8;
	cpu->e = value & 0xff;
}

static inline uint16_t cpu8080_get_hl(cpu8080* cpu)
{
	return (cpu->h << 8) | cpu->l;
}

static inline uint16_t cpu8080_get_bc(cpu8080* cpu)
{
	return (cpu->b << 8) | cpu->c;
}

static inline uint16_t cpu8080_get_de(cpu8080* cpu)
{
	return (cpu->d << 8) | cpu->e;
}

static inline void cpu8080_push_stack(cpu8080* cpu, uint16_t value)
{
	cpu->memory[cpu->sp - 1] = (value >> 8) & 0xff;
	cpu->memory[cpu->sp - 2] = value & 0xff;
	cpu->sp -= 2;
}

static inline void cpu8080_jump(cpu8080* cpu, uint16_t addr)
{
	cpu->pc = addr;
}

static inline void cpu8080_call(cpu8080* cpu, uint16_t addr)
{
	cpu8080_push_stack(cpu, cpu->pc + 3);
	cpu8080_jump(cpu, addr);
}

static inline void cpu8080_return(cpu8080* cpu)
{
	cpu->pc = cpu->memory[cpu->sp] | (cpu->memory[cpu->sp + 1] << 8);
	cpu->sp += 2;
}

static inline void cpu8080_add(cpu8080* cpu, uint8_t value)
{
	uint16_t answer = (uint16_t)cpu->a + (uint16_t)value;
	cpu->zf = ((answer & 0xff) == 0);
	cpu->sf = ((answer & 0x80) != 0);
	cpu->cf = (answer > 0xff);
	cpu->pf = parity(answer & 0xff, 8);
	cpu->a = answer & 0xff;
	cpu->pc++;
}

static inline void cpu8080_sub(cpu8080* cpu, uint8_t value)
{
	cpu8080_add(cpu, ~value);
}

static inline uint8_t cpu8080_rb(cpu8080* cpu, uint16_t addr)
{
	return cpu->memory[addr];
}

static inline uint16_t cpu8080_rw(cpu8080* cpu, uint16_t addr)
{
	return (cpu8080_rb(cpu, addr + 1) << 8) | cpu8080_rb(cpu, addr);
}

static inline void cpu8080_wb(cpu8080* cpu, uint8_t value, uint16_t addr)
{
	cpu->memory[addr] = value;
}

static inline void cpu8080_ww(cpu8080* cpu, uint16_t value, uint16_t addr)
{
	cpu->memory[addr + 1] = value >> 8;
	cpu->memory[addr] = value & 0xff;
}

static inline uint8_t cpu8080_dec(cpu8080* cpu, uint8_t reg)
{
	uint8_t res = reg - 1;
	cpu->zf = (res == 0);
	cpu->sf = (0x80 == (res & 0x80));
	cpu->pf = parity(res, 8);
	cpu->ac = !((res & 0xF) == 0xF);
	return res;
}

static inline void cpu8080_sbb(cpu8080* cpu, uint8_t reg)
{
	uint16_t answer = (uint16_t)cpu->a + ~reg;
	cpu->zf = ((answer & 0xff) == 0);
	cpu->sf = ((answer & 0x80) != 0);
	cpu->cf = !cpu->cf;
	cpu->pf = parity(answer & 0xff, 8);
	cpu->a = answer & 0xff;
}

void cpu8080_emulate(cpu8080* cpu)
{
	uint8_t* opcode = &cpu->memory[cpu->pc];
	//debug(cpu);
	switch (*opcode)
	{
		case NOP:
		{
			cpu->pc++;
		} break;
		case LXI_B_D16:
		{
			cpu8080_set_bc(cpu, (opcode[2] << 8) | opcode[1]);
			cpu->pc += 3;
		} break;
		case MVI_B_D8:
		{
			cpu->b = opcode[1];
			cpu->pc += 2;
		} break;
		case DCR_A:
		{
			cpu->a = cpu8080_dec(cpu, cpu->a);
			cpu->pc++;
		} break;
		case DCR_B:
		{
			cpu->b = cpu8080_dec(cpu, cpu->b);
			cpu->pc++;
		} break;
		case MVI_C_D8:
		{
			cpu->c = opcode[1];
			cpu->pc += 2;
		} break;
		case DCR_C:
		{
			cpu->c = cpu8080_dec(cpu, cpu->c);
			cpu->pc++;
		} break;
		case RRC:
		{
			cpu->cf = cpu->a & 1;
			cpu->a = (cpu->cf << 7) | (cpu->a >> 1);
			cpu->pc++;
		} break;
		case RAR:
		{
			uint8_t a = cpu->a;
			cpu->a = (cpu->cf << 7) | (a >> 1);
			cpu->cf = (1 == (a & 1));
			cpu->pc++;
		} break;
		case LXI_D_D16:
		{
			cpu8080_set_de(cpu, (opcode[2] << 8) | opcode[1]);
			cpu->pc += 3;
		} break;
		case INX_D:
		{
			cpu->e++;
			uint8_t res = cpu->d + 1;
			cpu->ac = (res & 0xF) == 0;
			if (cpu->e == 0)
				cpu->d = res;
			cpu->pc++;
		} break;
		case DAD_D:
		{
			uint16_t de = cpu8080_get_de(cpu);
			uint16_t hl = cpu8080_get_hl(cpu);
			cpu8080_set_de(cpu, de + hl);
			cpu->pc++;
		} break;
		case DAD_B:
		{
			uint16_t bc = cpu8080_get_bc(cpu);
			uint16_t hl = cpu8080_get_hl(cpu);
			cpu8080_set_bc(cpu, bc + hl);
			cpu->pc++;
		} break;
		case LDAX_D:
		{
			cpu->a = cpu8080_rb(cpu, cpu8080_get_de(cpu));
			cpu->pc++;
		} break;
		case LXI_H_D16:
		{
			cpu8080_set_hl(cpu, opcode[2] << 8 | opcode[1]);
			cpu->pc += 3;
		} break;
		case INX_H:
		{
			cpu->l++;
			if (cpu->l == 0)
				cpu->h++;
			cpu->pc++;
		} break;
		case MVI_H_D8:
		{
			cpu->h = opcode[1];
			cpu->pc += 2;
		} break;
		case DAD_H:
		{
			uint16_t hl = cpu8080_get_hl(cpu);
			cpu8080_set_hl(cpu, hl + hl);
			cpu->pc++;
		} break;
		case LXI_SP_D16:
		{
			cpu->sp = (opcode[2] << 8) | opcode[1];
			cpu->pc += 3;
		} break;
		case STA_addr:
		{
			cpu8080_wb(cpu, cpu->a, (opcode[2] << 8) | (opcode[1]));
			cpu->pc += 3;
		} break;
		case MVI_M_D8:
		{
			cpu8080_wb(cpu, opcode[1], cpu8080_get_hl(cpu));
			cpu->pc += 2;
		} break;	
		case LDA_addr:
		{
			cpu->a = cpu8080_rb(cpu, (opcode[2] << 8) | opcode[1]);
			cpu->pc += 3;
		} break;
		case MVI_A_D8:
		{
			cpu->a = opcode[1];
			cpu->pc += 2;
		} break;
		case MOV_D_M:
		{
			cpu->d = cpu8080_rb(cpu, cpu8080_get_hl(cpu));
			cpu->pc += 1;
		} break;
		case MOV_E_M:
		{
			cpu->e = cpu8080_rb(cpu, cpu8080_get_hl(cpu));
			cpu->pc += 1;
		} break;
		case MOV_H_M:
		{
			cpu->h = cpu8080_rb(cpu, cpu8080_get_hl(cpu));
			cpu->pc += 1;
		} break;
		case MOV_L_A:
		{
			cpu->l = cpu->a;
			cpu->pc += 1;
		} break;
		case MOV_M_A:
		{
			cpu8080_wb(cpu, cpu->a, cpu8080_get_hl(cpu));
			cpu->pc += 1;
		} break;
		case MOV_A_B:
		{
			cpu->a = cpu->b;
			cpu->pc += 1;
		} break;
		case MOV_A_D:
		{
			cpu->a = cpu->d;
			cpu->pc += 1;
		} break;
		case MOV_A_E:
		{
			cpu->a = cpu->e;
			cpu->pc += 1;
		} break;
		case MOV_A_H:
		{
			cpu->a = cpu->h;
			cpu->pc += 1;
		} break;
		case MOV_A_M:
		{
			cpu->a = cpu8080_rb(cpu, cpu8080_get_hl(cpu));
			cpu->pc += 1;
		} break;
		case ANA_A:
		{
			cpu->a &= cpu->a;
			set_logic_flags(cpu);
			cpu->pc++;
		} break;
		case XRA_A:
		{
			cpu->a ^= cpu->a;
			set_logic_flags(cpu);
			cpu->pc++;
		} break;
		case POP_B:
		{
			cpu8080_set_bc(cpu, (cpu8080_rb(cpu, cpu->sp + 1) << 8) | cpu8080_rb(cpu, cpu->sp) << 8);
			cpu->sp += 2;
			cpu->pc++;
		} break;
		case JNZ_addr:
		{
			if (cpu->zf == 0)
				cpu8080_jump(cpu, ((opcode[2] << 8) | opcode[1]) + 1);
			else
				cpu->pc += 3;
		} break;
		case JMP_addr:
		{
			cpu8080_jump(cpu, (opcode[2] << 8) | opcode[1]);
		} break;
		case PUSH_B:
		{
			cpu8080_push_stack(cpu, cpu8080_get_bc(cpu));
			cpu->pc++;
		} break;
		case ADI_D8:
		{
			cpu8080_add(cpu, (uint16_t)opcode[1]);
			cpu->pc += 2;
		} break;
		case JZ_addr:
		{
			if (cpu->zf == 1)
				cpu8080_jump(cpu, (opcode[2] << 8) | opcode[1]);
			else
				cpu->pc += 3;
		} break;
		case CALL_addr:
		{
			cpu8080_call(cpu, (opcode[2] << 8) | opcode[1]);
		} break;
		case RET:
		{
			cpu8080_return(cpu);
		} break;
		case POP_D:
		{
			cpu8080_set_de(cpu, (cpu8080_rb(cpu, cpu->sp + 1) << 8) | cpu8080_rb(cpu, cpu->sp) << 8);
			cpu->sp += 2;
			cpu->pc++;
		} break;
		case OUT_D8:
		{
			cpu->pc += 2;
		} break;
		case PUSH_D:
		{
			cpu8080_push_stack(cpu, cpu8080_get_de(cpu));
			cpu->pc++;
		} break;
		case POP_H:
		{
			cpu8080_set_hl(cpu, (cpu8080_rb(cpu, cpu->sp + 1) << 8) | cpu8080_rb(cpu, cpu->sp) << 8);
			cpu->sp += 2;
			cpu->pc++;
		} break;
		case PUSH_H:
		{
			cpu8080_push_stack(cpu, cpu8080_get_hl(cpu));
			cpu->pc++;
		} break;
		case ANI_D8:
		{
			cpu->a &= opcode[1];
			set_logic_flags(cpu);
			cpu->pc += 2;
		} break;
		case XCHG:
		{
			cpu8080_set_hl(cpu, cpu8080_get_de(cpu));
			cpu8080_set_de(cpu, cpu8080_get_hl(cpu));
			cpu->pc++;
		} break;
		case POP_PSW:
		{
			cpu->a = cpu8080_rb(cpu, cpu->sp + 1);
			uint8_t psw = cpu8080_rb(cpu, cpu->sp);
			cpu->zf = 0x01  == (psw & 0x01);
			cpu->sf = 0x02  == (psw & 0x02);
			cpu->pf = 0x04  == (psw & 0x04);
			cpu->cf = 0x05  == (psw & 0x05);
			cpu->ac = 0x010 == (psw & 0x010);
			cpu->sp += 2;
			cpu->pc++;
		} break;
		case PUSH_PSW:
		{
			cpu->memory[cpu->sp - 1] = cpu->a;
			uint8_t psw = cpu->zf | cpu->sf << 1 | cpu->pf << 2 | cpu->cf << 3 | cpu->ac << 4 ;
			cpu->memory[cpu->sp - 2] = psw;
			cpu->sp -= 2;
			cpu->pc++;
		} break;
		case DI:
		{
			cpu->enable_int = false;
			cpu->pc++;
		} break;
		case EI:
		{
			cpu->enable_int = true;
			cpu->pc++;
		} break;
		case CPI_D8:
		{
			uint8_t x = cpu->a - opcode[1];
			cpu->zf = (x == 0);
			cpu->sf = (0x80 == (x & 0x80));
			cpu->pf = parity(x, 8);
			cpu->cf = x > 0;
			cpu->pc += 2;
		} break;
		case ADD_A:
		{
			cpu8080_add(cpu, cpu->a);
			cpu->pc++;
		} break;
		case ADD_B:
		{
			cpu8080_add(cpu, cpu->b);
			cpu->pc++;
		} break;
		case ADD_C:
		{
			cpu8080_add(cpu, cpu->c);
			cpu->pc++;
		} break;
		case ADD_D:
		{
			cpu8080_add(cpu, cpu->d);
			cpu->pc++;
		} break;
		case ADD_E:
		{
			cpu8080_add(cpu, cpu->e);
			cpu->pc++;
		} break;
		case ADD_H:
		{
			cpu8080_add(cpu, cpu->h);
			cpu->pc++;
		} break;
		case SUB_A:
		{
			cpu8080_add(cpu, ~cpu->a);
			cpu->pc++;
		} break;
		case SUB_B:
		{
			cpu8080_add(cpu, ~cpu->b);
			cpu->pc++;
		} break;
		case SUB_C:
		{
			cpu8080_add(cpu, ~cpu->c);
			cpu->pc++;
		} break;
		case SUB_D:
		{
			cpu8080_add(cpu, ~cpu->d);
			cpu->pc++;
		} break;
		case SUB_E:
		{
			cpu8080_add(cpu, ~cpu->e);
			cpu->pc++;
		} break;
		case SUB_H:
		{
			cpu8080_add(cpu, ~cpu->h);
			cpu->pc++;
		} break;
		case SUB_L:
		{
			cpu8080_add(cpu, ~cpu->l); 
			cpu->pc++;
		} break;
		case SBB_C:
		{
			cpu8080_sbb(cpu, cpu->c);
			cpu->pc++;
		} break;
		case SBB_D:
		{
			cpu8080_sbb(cpu, cpu->d);
			cpu->pc++;
		} break;
		case SBB_E:
		{
			cpu8080_sbb(cpu, cpu->d);
			cpu->pc++;
		} break;
		case SBB_A:
		{
			cpu8080_sbb(cpu, cpu->a);
			cpu->pc++;
		} break;
		case SBB_B:
		{
			cpu8080_sbb(cpu, cpu->b);
			cpu->pc++;
		} break;
		case SBB_H:
		{
			cpu8080_sbb(cpu, cpu->h);
			cpu->pc++;
		} break;
		case SBB_L:
		{
			cpu8080_sbb(cpu, cpu->l);
			cpu->pc++;
		} break;
		case CZ:
		{
			if (cpu->zf == 1)
				cpu8080_call(cpu, opcode[1] | (opcode[2] << 8));
			else 
				cpu->pc += 3;
		} break;
		case CNZ:
		{
			if (cpu->zf == 0)
				cpu8080_call(cpu, opcode[1] | (opcode[2] << 8));
			else
				cpu->pc += 3;
		} break;
		case RC:
		{
			if (cpu->cf == 1)
				cpu8080_return(cpu);
			else
				cpu->pc++;
		} break;
		case RNC:
		{
			if (cpu->cf == 0)
				cpu8080_return(cpu);
			else
				cpu->pc++;
		} break;
		case CNC:
		{
			if (cpu->cf == 0)
				cpu8080_call(cpu, opcode[1] | (opcode[2] << 8));
			else
				cpu->pc += 3;
		} break;
		case JC:
		{
			if (cpu->cf == 1)
				cpu8080_jump(cpu, (opcode[2] << 8) | opcode[1]);
			else
				cpu->pc += 3;
		} break;
		case RPO:
		{
			if (cpu->pf == 0)
				cpu8080_return(cpu);
			else
				cpu->pc++;
		} break;
		case INR_A:
		{
			cpu->a++;
			cpu->pc++;
		} break;
		case INR_B:
		{
			cpu->b++;
			cpu->pc++;
		} break;
		case INR_C:
		{
			cpu->c++;
			cpu->pc++;
		} break;
		case INR_D:
		{
			cpu->d++;
			cpu->pc++;
		} break;
		case INR_E:
		{
			cpu->e++;
			cpu->pc++;
		} break;
		case INR_H:
		{
			cpu->h++;
			cpu->pc++;
		} break;
		case INR_L:
		{
			cpu->l++;
			cpu->pc++;
		} break;
		case RZ:
		{
			if (cpu->zf == 0)
				cpu8080_return(cpu);
			else
				cpu->pc++;
		} break;
		case RNZ:
		{
			if (cpu->zf == 1)
				cpu8080_return(cpu);
			else
				cpu->pc++;
		} break;

		case PRINT_B:
		{
			printf("HEX #$0x%02x, DEC %d\n", cpu->b, cpu->b);
			cpu->pc++;
		} break;
		case HLT:
		{
			cpu->halted = true;
		} break;
	}
}

void cpu8080_run(cpu8080* cpu)
{
	//cpu->pc = 0x100;
	while (!cpu->halted)
	{
		cpu8080_emulate(cpu);
	}
}


void cpu8080_disassembly(cpu8080* cpu, const char* buffer, int size)
{
	int pc = 0;
	while (pc < size)
	{
		uint8_t* opcode = &buffer[pc];
		printf("0x%04X:\t", pc);
		switch (*opcode)
		{
		case 0x00:
			printf("NOP");
			break;
		case 0x08:
		case 0x10:
		case 0x18:
		case 0x28:
		case 0x38:
		case 0xcb:
		case 0xd9:
		case 0xdd:
		case 0xfd:
			
			break;
		case 0x01:
			printf("LXI\tB, #$0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x02:
			printf("STAX\tB");
			break;
		case 0x03:
			printf("INX\tB");
			break;
		case 0x04:
			printf("INR\tB");
			break;
		case 0x05:
			printf("DCR\tB");
			break;
		case 0x06:
			printf("MVI\tB, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x07:
			printf("RLC");
			break;
		case 0x09:
			printf("DAD\tB");
			break;
		case 0x0a:
			printf("LDAX\tB");
			break;
		case 0x0b:
			printf("DCX\tB");
			break;
		case 0x0c:
			printf("INR\tC");
			break;
		case 0x0d:
			printf("DCR\tC");
			break;
		case 0x0e:
			printf("MVI\tC, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x0f:
			printf("RRC");
			break;
		case 0x11:
			printf("LXI\tD, #$0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x12:
			printf("STAX\tD");
			break;
		case 0x13:
			printf("INX\tD");
			break;
		case 0x14:
			printf("INR\tD");
			break;
		case 0x15:
			printf("DCR\tD");
			break;
		case 0x16:
			printf("DCR\tD");
			break;
		case 0x17:
			printf("RAL");
			break;
		case 0x19:
			printf("DAD\tD");
			break;
		case 0x1a:
			printf("LDAX\tD");
			break;
		case 0x1b:
			printf("DCX\tD");
			break;
		case 0x1c:
			printf("INR\tE");
			break;
		case 0x1d:
			printf("DCR\tE");
			break;
		case 0x1e:
			printf("MVI\tE, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x1f:
			printf("RAR");
			break;
		case 0x20:
			printf("RIM");
			break;
		case 0x21:
			printf("LXI\tH, #$0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x22:
			printf("SHLD\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x23:
			printf("INX\tH");
			break;
		case 0x24:
			printf("INR\tH");
			break;
		case 0x25:
			printf("DCR\tH");
			break;
		case 0x26:
			printf("MVI\tH, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x27:
			printf("DAA");
			break;
		case 0x29:
			printf("DAD\tH");
			break;
		case 0x2a:
			printf("LHLD\t 0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x2b:
			printf("DCX\tH");
			break;
		case 0x2c:
			printf("INR\tL");
			break;
		case 0x2d:
			printf("DCR\tL");
			break;
		case 0x2e:
			printf("MVI\tL, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x2f:
			printf("CMA");
			break;
		case 0x30:
			printf("SIM");
			break;
		case 0x31:
			printf("LXI\tSP, #$0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x32:
			printf("STA\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x33:
			printf("INX\tSP");
			break;
		case 0x34:
			printf("INR\tM");
			break;
		case 0x35:
			printf("DCR\tM");
			break;
		case 0x36:
			printf("MVI\tM, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x37:
			printf("STC");
			break;
		case 0x39:
			printf("DAD\tSP");
			break;
		case 0x3a:
			printf("LDA\t0x%02x%02X", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0x3b:
			printf("DCX\tSP");
			break;
		case 0x3c:
			printf("INR\tA");
			break;
		case 0x3d:
			printf("DCR\tA");
			break;
		case 0x3e:
			printf("MVI\tA, #$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0x3f:
			printf("CMC");
			break;
		case 0x40:
			printf("MOV\tB, B");
			break;
		case 0x41:
			printf("MOV\tB, C");
			break;
		case 0x42:
			printf("MOV\tB, D");
			break;
		case 0x43:
			printf("MOV\tB, E");
			break;
		case 0x44:
			printf("MOV\tB, H");
			break;
		case 0x45:
			printf("MOV\tB, L");
			break;
		case 0x46:
			printf("MOV\tB, M");
			break;
		case 0x47:
			printf("MOV\tB, A");
			break;
		case 0x48:
			printf("MOV\tC, B");
			break;
		case 0x49:
			printf("MOV\tC, C");
			break;
		case 0x4a:
			printf("MOV\tC, D");
			break;
		case 0x4b:
			printf("MOV\tC, E");
			break;
		case 0x4c:
			printf("MOV\tC, H");
			break;
		case 0x4d:
			printf("MOV\tC, L");
			break;
		case 0x4e:
			printf("MOV\tC, M");
			break;
		case 0x4f:
			printf("MOV\tC, A");
			break;
		case 0x50:
			printf("MOV\tD, B");
			break;
		case 0x51:
			printf("MOV\tD, C");
			break;
		case 0x52:
			printf("MOV\tD, D");
			break;
		case 0x53:
			printf("MOV\tD, E");
			break;
		case 0x54:
			printf("MOV\tD, H");
			break;
		case 0x55:
			printf("MOV\tD, L");
			break;
		case 0x56:
			printf("MOV\tD, M");
			break;
		case 0x57:
			printf("MOV\tD, A");
			break;
		case 0x58:
			printf("MOV\tE, B");
			break;
		case 0x59:
			printf("MOV\tE, C");
			break;
		case 0x5a:
			printf("MOV\tE, D");
			break;
		case 0x5b:
			printf("MOV\tE, E");
			break;
		case 0x5c:
			printf("MOV\tE, H");
			break;
		case 0x5d:
			printf("MOV\tE, L");
			break;
		case 0x5e:
			printf("MOV\tE, M");
			break;
		case 0x5f:
			printf("MOV\tE, A");
			break;
		case 0x60:
			printf("MOV\tH, B");
			break;
		case 0x61:
			printf("MOV\tH, C");
			break;
		case 0x62:
			printf("MOV\tH, D");
			break;
		case 0x63:
			printf("MOV\tH, E");
			break;
		case 0x64:
			printf("MOV\tH, H");
			break;
		case 0x65:
			printf("MOV\tH, L");
			break;
		case 0x66:
			printf("MOV\tH, M");
			break;
		case 0x67:
			printf("MOV\tH, A");
			break;
		case 0x68:
			printf("MOV\tL, B");
			break;
		case 0x69:
			printf("MOV\tL, C");
			break;
		case 0x6a:
			printf("MOV\tL, D");
			break;
		case 0x6b:
			printf("MOV\tL, E");
			break;
		case 0x6c:
			printf("MOV\tL, H");
			break;
		case 0x6d:
			printf("MOV\tL, L");
			break;
		case 0x6e:
			printf("MOV\tL, M");
			break;
		case 0x6f:
			printf("MOV\tL, A");
			break;
		case 0x70:
			printf("MOV\tM, B");
			break;
		case 0x71:
			printf("MOV\tM, C");
			break;
		case 0x72:
			printf("MOV\tM, D");
			break;
		case 0x73:
			printf("MOV\tM, E");
			break;
		case 0x74:
			printf("MOV\tM, H");
			break;
		case 0x75:
			printf("MOV\tM, L");
			break;
		case 0x76:
			printf("HLT");
			break;
		case 0x77:
			printf("MOV\tM, A");
			break;
		case 0x78:
			printf("MOV\tA, B");
			break;
		case 0x79:
			printf("MOV\tA, C");
			break;
		case 0x7a:
			printf("MOV\tA, D");
			break;
		case 0x7b:
			printf("MOV\tA, E");
			break;
		case 0x7c:
			printf("MOV\tA, H");
			break;
		case 0x7d:
			printf("MOV\tA, L");
			break;
		case 0x7e:
			printf("MOV\tA, M");
			break;
		case 0x7f:
			printf("MOV\tA, A");
			break;
		case 0x80:
			printf("ADD\tB");
			break;
		case 0x81:
			printf("ADD\tC");
			break;
		case 0x82:
			printf("ADD\tD");
			break;
		case 0x83:
			printf("ADD\tE");
			break;
		case 0x84:
			printf("ADD\tH");
			break;
		case 0x85:
			printf("ADD\tL");
			break;
		case 0x86:
			printf("ADD\tM");
			break;
		case 0x87:
			printf("ADD\tA");
			break;
		case 0x88:
			printf("ADC\tB");
			break;
		case 0x89:
			printf("ADC\tC");
			break;
		case 0x8a:
			printf("ADC\tE");
			break;
		case 0x8b:
			printf("ADC\tE");
			break;
		case 0x8c:
			printf("ADC\tH");
			break;
		case 0x8d:
			printf("ADC\tL");
			break;
		case 0x8e:
			printf("ADC\tM");
			break;
		case 0x8f:
			printf("ADC\tA");
			break;
		case 0x90:
			printf("SUB\tB");
			break;
		case 0x91:
			printf("SUB\tC");
			break;
		case 0x92:
			printf("SUB\tD");
			break;
		case 0x93:
			printf("SUB\tE");
			break;
		case 0x94:
			printf("SUB\tH");
			break;
		case 0x95:
			printf("SUB\tL");
			break;
		case 0x96:
			printf("SUB\tM");
			break;
		case 0x97:
			printf("SUB\tA");
			break;
		case 0x98:
			printf("SBB\tB");
			break;
		case 0x99:
			printf("SBB\tC");
			break;
		case 0x9a:
			printf("SBB\tD");
			break;
		case 0x9b:
			printf("SBB\tE");
			break;
		case 0x9c:
			printf("SBB\tH");
			break;
		case 0x9d:
			printf("SBB\tL");
			break;
		case 0x9e:
			printf("SBB\tM");
			break;
		case 0x9f:
			printf("SBB\tA");
			break;
		case 0xa0:
			printf("ANA\tB");
			break;
		case 0xa1:
			printf("ANA\tC");
			break;
		case 0xa2:
			printf("ANA\tD");
			break;
		case 0xa3:
			printf("ANA\tE");
			break;
		case 0xa4:
			printf("ANA\tH");
			break;
		case 0xa5:
			printf("ANA\tL");
			break;
		case 0xa6:
			printf("ANA\tM");
			break;
		case 0xa7:
			printf("ANA\tA");
			break;
		case 0xa8:
			printf("XRA\tB");
			break;
		case 0xa9:
			printf("XRA\tC");
			break;
		case 0xaa:
			printf("XRA\tD");
			break;
		case 0xab:
			printf("XRA\tE");
			break;
		case 0xac:
			printf("XRA\tH");
			break;
		case 0xad:
			printf("XRA\tL");
			break;
		case 0xae:
			printf("XRA\tM");
			break;
		case 0xaf:
			printf("XRA\tA");
			break;
		case 0xb0:
			printf("ORA\tB");
			break;
		case 0xb1:
			printf("ORA\tC");
			break;
		case 0xb2:
			printf("ORA\tD");
			break;
		case 0xb3:
			printf("ORA\tE");
			break;
		case 0xb4:
			printf("ORA\tH");
			break;
		case 0xb5:
			printf("ORA\tL");
			break;
		case 0xb6:
			printf("ORA\tM");
			break;
		case 0xb7:
			printf("ORA\tA");
			break;
		case 0xb8:
			printf("CMP\tB");
			break;
		case 0xb9:
			printf("CMP\tC");
			break;
		case 0xba:
			printf("CMP\tD");
			break;
		case 0xbb:
			printf("CMP\tE");
			break;
		case 0xbc:
			printf("CMP\tH");
			break;
		case 0xbd:
			printf("CMP\tL");
			break;
		case 0xbe:
			printf("CMP\tM");
			break;
		case 0xbf:
			printf("CMP\tA");
			break;
		case 0xc0:
			printf("RNZ");
			break;
		case 0xc1:
			printf("POP\tB");
			break;
		case 0xc2:
			printf("JNZ\t0x%02x%02X", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xc3:
			printf("JMP\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xc4:
			printf("CNZ\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xc5:
			printf("PUSH\tB");
			break;
		case 0xc6:
			printf("ADI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xc7:
			printf("RST\t0");
			break;
		case 0xc8:
			printf("RZ");
			break;
		case 0xc9:
			printf("RET");
			break;
		case 0xca:
			printf("JZ\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xcc:
			printf("CZ\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xcd:
			printf("CALL\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xce:
			printf("ACI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xcf:
			printf("RST\t1");
			break;
		case 0xd0:
			printf("RNC");
			break;
		case 0xd1:
			printf("POP\tD");
			break;
		case 0xd2:
			printf("JNC\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xd3:
			printf("OUT\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xd4:
			printf("CNC\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xd5:
			printf("PUSH\tD");
			break;
		case 0xd6:
			printf("SUI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xd7:
			printf("RST\t2");
			break;
		case 0xd8:
			printf("RC");
			break;
		case 0xda:
			printf("JC\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xdb:
			printf("IN\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xdc:
			printf("CC\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xde:
			printf("SBI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xdf:
			printf("RST\t3");
			break;
		case 0xe0:
			printf("RPO");
			break;
		case 0xe1:
			printf("POP\tH");
			break;
		case 0xe2:
			printf("JPO\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xe3:
			printf("XTHL");
			break;
		case 0xe4:
			printf("CPO\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xe5:
			printf("PUSH\tH");
			break;
		case 0xe6:
			printf("ANI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xe7:
			printf("RST\t4");
			break;
		case 0xe8:
			printf("RPE");
			break;
		case 0xe9:
			printf("PCHL");
			break;
		case 0xea:
			printf("JPE\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xeb:
			printf("XCHG");
			break;
		case 0xec:
			printf("CPE\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xee:
			printf("XRI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xef:
			printf("RST\t5");
			break;
		case 0xf0:
			printf("RP");
			break;
		case 0xf1:
			printf("POP\tPSW");
			break;
		case 0xf2:
			printf("JP\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xf3:
			printf("DI");
			break;
		case 0xf4:
			printf("CP\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xf5:
			printf("PUSH\tPSW");
			break;
		case 0xf6:
			printf("ORD\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xf7:
			printf("RST\t6");
			break;
		case 0xf8:
			printf("RM");
			break;
		case 0xf9:
			printf("SPHL");
			break;
		case 0xfa:
			printf("JM\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xfb:
			printf("EI");
			break;
		case 0xfc:
			printf("CM\t0x%02x%02x", opcode[2], opcode[1]);
			pc += 2;
			break;
		case 0xfe:
			printf("CPI\t#$0x%02x", opcode[1]);
			pc += 1;
			break;
		case 0xff:
			printf("RST\t7");
			break;
		case 0xed:
			printf("PRINT_B\t#$0x%02x", cpu->b);
			break;
		default:
			printf("Unknown Instruction: 0x%02x", *opcode);
			break;
		}
		printf("\n");
		pc++;
	}
}