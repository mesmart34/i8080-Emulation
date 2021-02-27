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
	sp = MEM_SIZE - 1;
	cf = 0;
	pf = 0;
	zf = 0;
	sf = 0;
	memory = (uint8_t*)malloc(MEM_SIZE);
	memset(memory, 0, MEM_SIZE);
}

void CPU8080::load_program(const uint8_t* buffer, size_t size, uint32_t offset)
{
	memcpy(memory + offset, buffer, size);
}

void CPU8080::run(const uint32_t start_addr, bool debugging)
{
	pc = start_addr;
	while (!halted)
	{
		if(debugging)
			debug(pc);
		emulate();
	}
}

void CPU8080::emulate()
{
	auto opcode = &memory[pc];
	switch (*opcode)
	{
	case LXI_B_D16:
	{
		set_bc((opcode[2] << 8) | opcode[1]);
		pc += 3;
	} break;
	case STAX_B:
	{
		auto addr = get_bc();
		wb(a, addr);
		pc += 1;
	} break;
	case STAX_D:
	{
		auto addr = get_de();
		wb(a, addr);
		pc += 1;
	} break;
	case INX_SP:
	{
		sp++;
		pc++;
		break;
	} break;
	case INX_B:
	{
		auto val = get_bc();
		val += 1;
		set_bc(val);
		pc += 1;
	} break;
	case INX_D:
	{
		auto val = get_de();
		val += 1;
		set_de(val);
		pc += 1;
	} break;
	case INR_M:
	{
		auto val = rb(get_hl());
		set_hl(val + 1);
		pc++;
	} break;
	case DCR_M:
	{
		auto val = rb(get_hl());
		set_hl(val - 1);
		pc++;
	} break;
	case INR_D:
	{
		d += 1;
		pc += 1;
	} break;
	case DCR_D:
	{
		d -= 1;
		pc += 1;
	} break;
	
	case INR_B:
	{
		b += 1;
		pc += 1;
	} break;
	case MVI_B_D8:
	{
		b = opcode[1];
		pc += 2;
	} break;
	case RLC:
	{
		cf = a >> 7;
		a = (a << 1) | cf;
		pc++;
	} break;

	case RRC:
	{
		cf = a & 1;
		a = (cf << 7) | (a >> 1);
		pc++;
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
	case RAR:
	{
		bool cy = cf;
		cf = a & 1;
		a = (a >> 1) | (cy << 7);
		pc++;
	} break;
	case LXI_D_D16:
	{
		set_de((opcode[2] << 8) | opcode[1]);
		pc += 3;
	} break;
	case DAD_SP:
	{
		sp += get_hl();
		pc++;
		break;
	}
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
	case LDAX_B:
	{
		a = rb(get_bc());
		pc++;
	} break;
	case DCX_SP:
	{
		sp--;
		pc++;
		break;
	}
	case DCX_D:
	{
		set_de(get_de() - 1);
		pc += 1;
	} break;
	case LDAX_D:
	{
		a = rb(get_de());
		pc++;
	} break;
	case SHLD_addr:
	{
		wb(get_hl(), (opcode[2] << 8) | (opcode[1]));
		pc += 3;
	}
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
	case DCX_B:
	{
		set_bc(get_bc() - 1);
		pc += 1;
	}
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
	case RAL:
	{
		bool cy = cf;
		cf = a & 1;
		a = (cy >> 7) | (a << 1);
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
	case MOV_B_E:
	{
		b = e;
		pc++;
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
	case MOV_L_A:
	{
		l = a;
		pc++;  
	} break;
	case MOV_L_B:
	{
		l = b;
		pc++;
	} break;
	case MOV_L_C:
	{
		l = c;
		pc++;
	} break;
	case MOV_L_D:
	{
		l = d;
		pc++;
	} break;
	case MOV_L_E:
	{
		l = e;
		pc++;
	} break;
	case MOV_L_H:
	{
		l = h;
		pc++;
	} break;
	case MOV_L_M:
	{
		l = rb(get_hl());
		pc++;
	} break;
	case MOV_M_L:
	{
		wb(l, get_hl());
	} break;
	case MOV_M_B:
	{
		wb(b, get_hl());
		pc++;
	} break;
	case MOV_M_A:
	{
		wb(a, get_hl());
		pc++;
	} break;
	case ADD_B:
	{
		b += a;
		pc++;
	} break;
	case ADD_C:
	{
		c += a;
		pc++;
	} break;
	case ADD_D:
	{
		d += a;
		pc++;
	} break;
	case ADD_E:
	{
		e += a;
		pc++;
	} break;
	case ADC_C:
	{
		auto val = a << 8;
		a = c + a + val;
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
	case XRA_B:
	{
		a ^= b;
		set_logic_flags();
		pc++;
	} break;
	case ORA_A:
	{
		a |= a;
		pc++;
	} break;
	case XRA_C:
	{
		a ^= c;
		set_logic_flags();
		pc++;
	} break;
	case XRA_D:
	{
		a ^= d;
		set_logic_flags();
		pc++;
	} break;
	case XRA_E:
	{
		a ^= e;
		set_logic_flags();
		pc++;
	} break;
	case XRA_H:
	{
		a ^= h;
		set_logic_flags();
		pc++;
	} break;
	case XRA_L:
	{
		a ^= l;
		set_logic_flags();
		pc++;
	} break;
	case ORA_B:
	{
		a |= b;
		cf = 0;
		pc++;
	} break;
	case ORA_C:
	{
		a |= c;
		cf = 0;
		pc++;
	} break;
	case ORA_D:
	{
		a |= d;
		cf = 0;
		pc++;
	} break;
	case ORA_E:
	{
		a |= e;
		cf = 0;
		pc++;
	} break;
	case ORA_H:
	{
		a |= h;
		cf = 0;
		pc++;
	} break;
	case ORA_L:
	{
		a |= l;
		cf = 0;
		pc++;
	} break;
	case ORA_M:
	{
		auto val = rb(get_hl());
		a |= val;
		cf = 0;
		pc++;
	} break;
	case ORI_D8:
	{
		a |= opcode[1];
		cf = 0;
		pc++;
	} break;
	case POP_B:
	{
		set_bc((rb(sp + 1) << 8) | rb(sp) << 8);
		sp += 2;
		pc++;
	} break;
	case RST_0:
	{
		auto addr = 0x00;
		call(addr);
	} break;
	case RST_1:
	{
		auto addr = 0x08;
		call(addr);
	} break;
	case RST_2:
	{
		auto addr = 0x10;
		call(addr);
	} break;
	case RST_3:
	{
		auto addr = 0x18;
		call(addr);
	} break;
	case RST_4:
	{
		auto addr = 0x20;
		call(addr);
	} break;
	case RST_5:
	{
		auto addr = 0x28;
		call(addr);
	} break;
	case RST_6:
	{
		auto addr = 0x30;
		call(addr);
	} break;
	case RST_7:
	{
		auto addr = 0x38;
		call(addr);
	} break;
	case RM:
	{
		if (sf == 1)
		{
			ret();
		}
		else {
			pc++;
		}
	} break;
	case JNC_addr:
	{
		if (cf != 1)
		{
			jump(((opcode[2] << 8) | opcode[1]));
		}
		else {
			pc += 3;
		}
	} break;
	case JNZ_addr:
	{
		if (zf == 0)
			jump(((opcode[2] << 8) | opcode[1]));
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
	case CMP_A:
	{
		cf = 1;
		pc++;
	} break;
	case SPHL:
	{
		sp = get_hl();
		pc++;
	} break;
	case CMP_B:
	{
		auto res = a - b;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_C:
	{
		auto res = a - c;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_D:
	{
		auto res = a - d;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_E:
	{
		auto res = a - e;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_H:
	{
		auto res = a - h;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_L:
	{
		auto res = a - l;
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CMP_M:
	{
		auto res = a - rb(get_hl());
		zf = ((res & 0xff) == 0);
		sf = ((res & 0x80) != 0);
		cf = (res > 0xff);
		pf = parity(res & 0xff, 8);
		pc++;
	} break;
	case CALL_addr:
	{
#if CPU_DEBUG
		if (5 == ((opcode[2] << 8) | opcode[1]))
		{
			if (c == 9)
			{
				uint16_t offset = (d << 8) | (e);
				auto str = &memory[offset + 3];  //пропускаем байты-префиксы
				while (*str != '$')
					printf("%c", *str++);
				printf("\n");
			}
			else if (c == 2)
			{
				//видел это в исследованном коде, никогда не замечал, чтобы оно вызывалось
				printf("print char routine called\n");
			}
		}
		else if (0 == ((opcode[2] << 8) | opcode[1]))
		{
			exit(0);
		}
		else
#endif
		{
			call((opcode[2] << 8) | opcode[1]);
		}
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
	case SUI_D8:
	{
		a -= opcode[1];
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
	case CMC:
	{
		cf = !cf;
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
	case CP_addr:
	{
		if (sf == 0)
		{
			call(opcode[1] | (opcode[2] << 8));
		}
		else {
			pc += 3;
		}
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
	case CM_addr:
	{
		if (this->sf == 1)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
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
	case XRI_D8:
	{
		a = a ^ opcode[1];
		pc += 2;
	} break;
	case CPE_addr:
	{
		if (this->pf == 1)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case CPO_addr:
	{
		if (this->pf == 0)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case ADD_A:
	{
		add(a);
		pc++;
	} break;
	case LHLD_addr:
	{
		auto val = rb((opcode[2] << 8) | opcode[1]);
		set_hl(val);
		pc += 3;
	} break;
	case ADD_H:
	{
		add(h);
		pc++;
	} break;
	case SBB_E:
	{
		sbb(e);
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
	case DAA:
	{
		//fuck you
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
	case SUB_M:
	{
		add(~rb(get_hl()));
		pc++;
	} break;
	case SBB_C:
	{
		sbb(c);
		pc++;
	} break;
	case RPE:
	{
		if (pf == 1)
		{
			ret();
		}
		else {
			pc++;
		}
	} break;
	case CMA:
	{
		a = ~a;
		pc++;
	} break;
	case SBB_D:
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
	case SBB_M:
	{
		sbb(rb(get_hl()));
		pc++;
	} break;
	case SBB_H:
	{
		sbb(h);
		pc++;
	} break;
	case ANA_B:
	{
		a &= b;
		set_logic_flags();
		pc++;
	} break;
	case ANA_C:
	{
		a &= c;
		set_logic_flags();
		pc++;
	} break;
	case ANA_D:
	{
		a &= d;
		set_logic_flags();
		pc++;
	} break;
	case ANA_E:
	{
		a &= e;
		set_logic_flags();
		pc++;
	} break;
	case ANA_H:
	{
		a &= h;
		set_logic_flags();
		pc++;
	} break;
	case ANA_L:
	{
		a &= l;
		set_logic_flags();
		pc++;
	} break;
	case ACI_D8:
	{
		a = opcode[1] + a + cf;
		pc++;
	}break;
	case SBI_D8:
	{
		sbb(c);
		pc += 2;
	} break;
	case ANA_M:
	{
		a &= rb(get_hl());
		set_logic_flags();
		pc++;
	} break;
	case CZ_addr:
	{
		if (zf == 1)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case PCHL:
	{
		auto addr = rb(get_hl());
		jump(addr);
	} break;
	
	case CNZ_addr:
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
	case CNC_addr:
	{
		if (cf == 0)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case CC_addr:
	{
		if (cf == 1)
			call(opcode[1] | (opcode[2] << 8));
		else
			pc += 3;
	} break;
	case IN_D8:
	{
		pc++;
	} break;
	case JC_addr:
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
	case INR_C:
	{
		c++;
		pc++;
	} break;
	case DCR_L:
	{
		l--;
		pc++;
	} break;
	case DCR_E:
	{
		e--;
		pc++;
	} break;
	case MVI_E_D8:
	{
		e = opcode[1];
		pc += 2;
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
	case STC:
	{
		cf = 1;
		pc++;
	} break;
	case RNZ:
	{
		if (zf == 1)
			ret();
		else
			pc++;
	} break;
	case JP_addr:
	{
		auto addr = opcode[1] | (opcode[2] << 8);
		if (sf == 0)
			jump(addr);
		else
			pc += 3;
	} break;
	case JPO_addr:
	{
		auto addr = opcode[1] | (opcode[2] << 8);
		if (pf == 0)
			jump(addr);
		else
			pc += 3;
	} break;
	case JM_addr:
	{
		auto addr = opcode[1] | (opcode[2] << 8);
		if (sf == 1)
			jump(addr);
		else
			pc += 3;
	} break;
	case JPE_addr:
	{
		auto addr = opcode[1] | (opcode[2] << 8);
		if(pf == 1)
			jump(addr);
		else
			pc += 3;
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
	default:
	{
		//printf("0x%04X:\t Unknown", pc);
		pc++;
	} break;
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