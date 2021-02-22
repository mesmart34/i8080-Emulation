#include "CPU8080.h"

CPU8080::CPU8080()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	h = 0;
	l = 0;
	e = 0;
	ac = 0;
	halted = 0;
	enable_int = 0;
	pc = 0;
	sp = 0;
	cf = 0;
	pf = 0;
	zf = 0;
	sf = 0;
	memory = (uint8_t*)malloc(0x10000);
}

void CPU8080::load_program(const uint8_t* buffer, size_t size, uint32_t offset)
{
	memcpy(memory + offset, buffer, size);
}

void CPU8080::run(const uint32_t start_addr)
{
	pc = start_addr;
	while (!halted)
	{
		emulate();
	}
}

void CPU8080::emulate()
{
	auto opcode = &memory[pc];
	switch (*opcode)
	{
	case NOP:
	{
		pc++;
	} break;
	case LXI_B_D16:
	{
		set_bc((opcode[2] << 8) | opcode[1]);
		pc += 3;
	} break;
	case MVI_B_D8:
	{
		b = opcode[1];
		pc += 2;
	} break;
	case DCR_A:
	{
		a = dec(a);
		pc++;
	} break;
	case DCR_B:
	{
		b = dec(b);
		pc++;
	} break;
	case MVI_C_D8:
	{
		c = opcode[1];
		pc += 2;
	} break;
	case DCR_C:
	{
		c = dec(c);
		pc++;
	} break;
	case RRC:
	{
		cf = a & 1;
		a = (cf << 7) | (a >> 1);
		pc++;
	} break;
	case RAR:
	{
		bool cy = cf;
		cf = a & 1;
		a = (a >> 1) | (cy << 7);
	} break;
	case LXI_D_D16:
	{
		set_de((opcode[2] << 8) | opcode[1]);
		pc += 3;
	} break;
	case INX_D:
	{
		e++;
		uint8_t res = d + 1;
		ac = (res & 0xF) == 0;
		if (e == 0)
			d = res;
		pc++;
	} break;
	case DAD_D:
	{
		uint16_t de = get_de();
		uint16_t hl = get_hl();
		set_de(de + hl);
		pc++;
	} break;
	case DAD_B:
	{
		uint16_t bc = get_bc();
		uint16_t hl = get_hl();
		set_bc(bc + hl);
		pc++;
	} break;
	case LDAX_D:
	{
		a = rb(get_de());
		pc++;
	} break;
	case LXI_H_D16:
	{
		set_hl(opcode[2] << 8 | opcode[1]);
		pc += 3;
	} break;
	case INX_H:
	{
		l++;
		if (l == 0)
			h++;
		pc++;
	} break;
	case MVI_H_D8:
	{
		h = opcode[1];
		pc += 2;
	} break;
	case DAD_H:
	{
		uint16_t hl = get_hl();
		set_hl(hl + hl);
		pc++;
	} break;
	case LXI_SP_D16:
	{
		sp = (opcode[2] << 8) | opcode[1];
		pc += 3;
	} break;
	case STA_addr:
	{
		wb(a, (opcode[2] << 8) | (opcode[1]));
		pc += 3;
	} break;
	case MVI_M_D8:
	{
		wb(opcode[1], get_hl());
		pc += 2;
	} break;
	case LDA_addr:
	{
		a = rb((opcode[2] << 8) | opcode[1]);
		pc += 3;
	} break;
	case MVI_A_D8:
	{
		a = opcode[1];
		pc += 2;
	} break;

	case MOV_A_A:
	{
		a = a;
		pc += 1;
	} break;
	case MOV_A_B:
	{
		a = b;
		pc += 1;
	} break;
	case MOV_A_C:
	{
		a = c;
		pc += 1;
	} break;
	case MOV_A_D:
	{
		a = d;
		pc += 1;
	} break;
	case MOV_A_E:
	{
		a = e;
		pc += 1;
	} break;
	case MOV_A_H:
	{
		a = h;
		pc += 1;
	} break;
	case MOV_A_L:
	{
		a = l;
		pc += 1;
	} break;
	case MOV_A_M:
	{
		a = rb(get_hl());
		pc += 1;
	} break;

	case MOV_B_A:
	{
		b = a;
		pc += 1;
	} break;
	case MOV_B_B:
	{
		b = b;
		pc += 1;
	} break;
	case MOV_B_C:
	{
		b = c;
		pc += 1;
	} break;
	case MOV_B_D:
	{
		b = d;
		pc += 1;
	} break;
	case MOV_B_E:
	{
		b = e;
		pc += 1;
	} break;
	case MOV_B_H:
	{
		b = h;
		pc += 1;
	} break;
	case MOV_B_L:
	{
		b = l;
		pc += 1;
	} break;
	case MOV_B_M:
	{
		b = rb(get_hl());
		pc += 1;
	} break;

	case MOV_C_A:
	{
		c = a;
		pc += 1;
	} break;
	case MOV_C_B:
	{
		c = b;
		pc += 1;
	} break;
	case MOV_C_C:
	{
		c = c;
		pc += 1;
	} break;
	case MOV_C_D:
	{
		c = d;
		pc += 1;
	} break;
	case MOV_C_E:
	{
		c = e;
		pc += 1;
	} break;
	case MOV_C_H:
	{
		c = h;
		pc += 1;
	} break;
	case MOV_C_L:
	{
		c = l;
		pc += 1;
	} break;
	case MOV_C_M:
	{
		c = rb(get_hl());
		pc += 1;
	} break;

	case MOV_D_A:
	{
		d = a;
		pc += 1;
	} break;
	case MOV_D_B:
	{
		d = b;
		pc += 1;
	} break;
	case MOV_D_C:
	{
		c = c;
		pc += 1;
	} break;
	case MOV_D_D:
	{
		d = d;
		pc += 1;
	} break;
	case MOV_D_E:
	{
		d = e;
		pc += 1;
	} break;
	case MOV_D_H:
	{
		d = h;
		pc += 1;
	} break;
	case MOV_D_L:
	{
		d = l;
		pc += 1;
	} break;
	case MOV_D_M:
	{
		d = rb(get_hl());
		pc += 1;
	} break;

	case MOV_E_A:
	{
		e = a;
		pc += 1;
	} break;
	case MOV_E_B:
	{
		e = b;
		pc += 1;
	} break;
	case MOV_E_C:
	{
		e = c;
		pc += 1;
	} break;
	case MOV_E_D:
	{
		e = d;
		pc += 1;
	} break;
	case MOV_E_E:
	{
		e = e;
		pc += 1;
	} break;
	case MOV_E_H:
	{
		e = h;
		pc += 1;
	} break;
	case MOV_E_L:
	{
		e = l;
		pc += 1;
	} break;
	case MOV_E_M:
	{
		e = rb(get_hl());
		pc += 1;
	} break;

	case MOV_H_A:
	{
		h = a;
		pc += 1;
	} break;
	case MOV_H_B:
	{
		h = b;
		pc += 1;
	} break;
	case MOV_H_C:
	{
		h = c;
		pc += 1;
	} break;
	case MOV_H_D:
	{
		h = d;
		pc += 1;
	} break;
	case MOV_H_E:
	{
		h = e;
		pc += 1;
	} break;
	case MOV_H_H:
	{
		h = h;
		pc += 1;
	} break;
	case MOV_H_L:
	{
		h = l;
		pc += 1;
	} break;
	case MOV_H_M:
	{
		h = rb(get_hl());
		pc += 1;
	} break;

	case ANA_A:
	{
		a &= a;
		set_logic_flags();
		pc++;
	} break;
	case XRA_A:
	{
		a ^= a;
		set_logic_flags();
		pc++;
	} break;
	case POP_B:
	{
		set_bc((rb(sp + 1) << 8) | rb(sp) << 8);
		sp += 2;
		pc++;
	} break;
	case JNZ_addr:
	{
		if (zf == 0)
			jump(((opcode[2] << 8) | opcode[1]) + 1);
		else
			pc += 3;
	} break;
	case JMP_addr:
	{
		jump((opcode[2] << 8) | opcode[1]);
	} break;
	case PUSH_B:
	{
		push_stack(get_bc());
		pc++;
	} break;
	case ADI_D8:
	{
		add((uint16_t)opcode[1]);
		pc += 2;
	} break;
	case JZ_addr:
	{
		if (zf == 1)
			jump((opcode[2] << 8) | opcode[1]);
		else
			pc += 3;
	} break;
	case CALL_addr:
	{
		call((opcode[2] << 8) | opcode[1]);
	} break;
	case RET:
	{
		ret();
	} break;
	case POP_D:
	{
		set_de((rb(sp + 1) << 8) | rb(sp) << 8);
		sp += 2;
		pc++;
	} break;
	case OUT_D8:
	{
		pc += 2;
	} break;
	case PUSH_D:
	{
		push_stack(get_de());
		pc++;
	} break;
	case POP_H:
	{
		set_hl((rb(sp + 1) << 8) | rb(sp) << 8);
		sp += 2;
		pc++;
	} break;
	case PUSH_H:
	{
		push_stack(get_hl());
		pc++;
	} break;
	case ANI_D8:
	{
		a &= opcode[1];
		set_logic_flags();
		pc += 2;
	} break;
	case XCHG:
	{
		set_hl(get_de());
		set_de(get_hl());
		pc++;
	} break;
	case POP_PSW:
	{
		a = rb(sp + 1);
		uint8_t psw = rb(sp);
		zf = 0x01 == (psw & 0x01);
		sf = 0x02 == (psw & 0x02);
		pf = 0x04 == (psw & 0x04);
		cf = 0x05 == (psw & 0x05);
		ac = 0x010 == (psw & 0x010);
		sp += 2;
		pc++;
	} break;
	case PUSH_PSW:
	{
		memory[sp - 1] = a;
		uint8_t psw = zf | sf << 1 | pf << 2 | cf << 3 | ac << 4;
		memory[sp - 2] = psw;
		sp -= 2;
		pc++;
	} break;
	case DI:
	{
		enable_int = false;
		pc++;
	} break;
	case EI:
	{
		enable_int = true;
		pc++;
	} break;
	case CPI_D8:
	{
		uint8_t x = a - opcode[1];
		zf = (x == 0);
		sf = (0x80 == (x & 0x80));
		pf = parity(x, 8);
		cf = x > 0;
		pc += 2;
	} break;
	case ADD_A:
	{
		add(a);
		pc++;
	} break;
	case ADD_B:
	{
		add(b);
		pc++;
	} break;
	case ADD_C:
	{
		add(c);
		pc++;
	} break;
	case ADD_D:
	{
		add(d);
		pc++;
	} break;
	case ADD_E:
	{
		add(e);
		pc++;
	} break;
	case ADD_H:
	{
		add(h);
		pc++;
	} break;
	case SUB_A:
	{
		add(~a);
		pc++;
	} break;
	case SUB_B:
	{
		add(~b);
		pc++;
	} break;
	case SUB_C:
	{
		add(~c);
		pc++;
	} break;
	case SUB_D:
	{
		add(~d);
		pc++;
	} break;
	case SUB_E:
	{
		add(~e);
		pc++;
	} break;
	case SUB_H:
	{
		add(~h);
		pc++;
	} break;
	case SUB_L:
	{
		add(~l);
		pc++;
	} break;
	case SBB_C:
	{
		sbb(c);
		pc++;
	} break;
	case SBB_D:
	{
		sbb(d);
		pc++;
	} break;
	case SBB_E:
	{
		sbb(d);
		pc++;
	} break;
	case SBB_A:
	{
		sbb(a);
		pc++;
	} break;
	case SBB_B:
	{
		sbb(b);
		pc++;
	} break;
	case SBB_H:
	{
		sbb(h);
		pc++;
	} break;
	case SBB_L:
	{
		sbb(l);
		pc++;
	} break;
	case CZ:
	{
		if (zf == 1)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case CNZ:
	{
		if (zf == 0)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case RC:
	{
		if (cf == 1)
			ret();
		else
			pc++;
	} break;
	case RNC:
	{
		if (cf == 0)
			ret();
		else
			pc++;
	} break;
	case CNC:
	{
		if (cf == 0)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case JC:
	{
		if (cf == 1)
			jump((opcode[2] << 8) | opcode[1]);
		else
			pc += 3;
	} break;
	case RPO:
	{
		if (pf == 0)
			ret();
		else
			pc++;
	} break;
	case INR_A:
	{
		a++;
		pc++;
	} break;
	case INR_B:
	{
		b++;
		pc++;
	} break;
	case INR_C:
	{
		c++;
		pc++;
	} break;
	case INR_D:
	{
		d++;
		pc++;
	} break;
	case INR_E:
	{
		e++;
		pc++;
	} break;
	case INR_H:
	{
		h++;
		pc++;
	} break;
	case INR_L:
	{
		l++;
		pc++;
	} break;
	case RZ:
	{
		if (zf == 0)
			ret();
		else
			pc++;
	} break;
	case RNZ:
	{
		if (zf == 1)
			ret();
		else
			pc++;
	} break;
	case PRINT_A:
	{
		printf("HEX #$0x%02x, DEC %d\n", a, a);
		pc++;
	} break;
	case PRINT_B:
	{
		printf("HEX #$0x%02x, DEC %d\n", b, b);
		pc++;
	} break;
	case PRINT_C:
	{
		printf("HEX #$0x%02x, DEC %d\n", c, c);
		pc++;
	} break;
	case HLT:
	{
		halted = true;
	} break;
	}
}

void CPU8080::disassembly(uint8_t* buffer, const size_t size)
{
	int pc = 0;
	while (pc < size)
	{
		uint8_t* opcode = &buffer[pc];
		printf("0x%04X:\t", pc);
		switch (*opcode)
		{
		case NOP:
			printf("NOP");
			break;
		case 0x08:
		case PRINT_A:
			printf("PRINT\tA", a);
			break;
		case PRINT_B:
			printf("PRINT\tB", b);
			break;
		case PRINT_C:
			printf("PRINT\tC", c);
			break;
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
			/*case 0x20:
				printf("RIM");
				break;*/
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
			/*case 0x30:
				printf("SIM");
				break;*/
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
		default:
			printf("Unknown Instruction: 0x%02x", *opcode);
			break;
		}
		printf("\n");
		pc++;
	}
}

inline void CPU8080::set_logic_flags()
{
	cf = ac = 0;
	zf = (a == 0);
	sf = (0x80 == (a & 0x80));
	pf = parity(a, 8);
}

inline void CPU8080::set_arithmetic_flags(const uint16_t res)
{
	cf = res > 0xff;
	zf = (res & 0xff) == 0;
	sf = (0x80 == (res & 0x80));
	pf = parity(res & 0xff, 8);
}

inline void CPU8080::set_bc(const uint16_t value)
{
	b = value >> 8;
	c = value & 0xff;
}

inline void CPU8080::set_hl(const uint16_t value)
{
	h = value >> 8;
	l = value & 0xff;
}

inline void CPU8080::set_de(const uint16_t value)
{
	d = value >> 8;
	e = value & 0xff;
}

inline uint16_t CPU8080::get_de() const
{
	return (d << 8) | e;
}

inline void CPU8080::push_stack(const uint16_t value)
{
	memory[sp - 1] = (value >> 8) & 0xff;
	memory[sp - 2] = value & 0xff;
	sp -= 2;
}

inline void CPU8080::jump(const uint16_t addr)
{
	pc = addr;
}

inline void CPU8080::call(const uint16_t addr)
{
	push_stack(pc + 3);
	jump(addr);
}

inline void CPU8080::ret()
{
	pc = memory[sp] | (memory[sp + 1] << 8);
	sp += 2;
}

inline void CPU8080::add(const uint8_t value)
{
	uint16_t answer = (uint16_t)a + (uint16_t)value;
	zf = ((answer & 0xff) == 0);
	sf = ((answer & 0x80) != 0);
	cf = (answer > 0xff);
	pf = parity(answer & 0xff, 8);
	a = answer & 0xff;
	pc++;
}

inline void CPU8080::sub(const uint8_t value)
{
	add(~value);
}

inline void CPU8080::wb(const uint8_t value, const uint16_t addr)
{
	memory[addr] = value;
}

inline void CPU8080::ww(const uint16_t value, const uint16_t addr)
{
	memory[addr + 1] = value >> 8;
	memory[addr] = value & 0xff;
}

inline void CPU8080::sbb(const uint8_t reg)
{
	uint16_t answer = (uint16_t)a + ~reg;
	zf = ((answer & 0xff) == 0);
	sf = ((answer & 0x80) != 0);
	cf = !cf;
	pf = parity(answer & 0xff, 8);
	a = answer & 0xff;
}

inline uint8_t CPU8080::dec(const uint8_t reg)
{
	uint8_t res = reg - 1;
	zf = (res == 0);
	sf = (0x80 == (res & 0x80));
	pf = parity(res, 8);
	ac = !((res & 0xF) == 0xF);
	return res;
}

inline uint8_t CPU8080::rb(const uint16_t addr) const
{
	return memory[addr];
}

inline uint16_t CPU8080::rw(const uint16_t addr) const
{
	return (rb(addr + 1) << 8) | rb(addr);
}

inline uint16_t CPU8080::get_hl() const
{
	return (h << 8) | l;
}

inline uint16_t CPU8080::get_bc() const
{
	return (b << 8) | c;
}

inline int32_t CPU8080::parity(int x, const int size)
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
