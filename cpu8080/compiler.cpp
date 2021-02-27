#include "compiler.h"

const static std::map<std::string, uint32_t> instruction_sizes{
	std::map<std::string, uint32_t> {
		{"lxi", 3},
		{"mvi", 2},
		{"shld", 3},
		{"lhld", 3},
		{"sta", 3},
		{"lda", 3},
		{"jnz", 3},
		{"jmp", 3},
		{"cnz", 3},
		{"adi", 2},
		{"jz", 3},
		{"cz", 3},
		{"call", 3},
		{"aci", 2},
		{"jnz", 3},
		{"out", 2},
		{"cnc", 3},
		{"sui", 2},
		{"jc", 3},
		{"in", 2},
		{"cc", 3},
		{"sbi", 2},
		{"jpo", 3},
		{"cpo",	3},
		{"ani",	2},
		{"jpe",	3},
		{"cpe",	3},
		{"xri",	2},
		{"jp",	2},
		{"cp",	3},
		{"ori",	2},
		{"jm",	3},
		{"cm",	3},
		{"cpi",	2}
	}
};


void Compiler::LowerCase(char* p)
{
	for (; *p; ++p) *p = tolower(*p);
}

void Compiler::Compile(const char* filename, const char* name)
{
	std::map<std::string, uint32_t> labels;
	std::ifstream input;
	input.open(filename, std::ios::in | std::ios::binary);
	std::string line;
	uint32_t pc = 0;
	uint8_t* code = new uint8_t(512);
	FindLabeles(input, labels);
	input.clear();
	input.seekg(0);
	while (std::getline(input, line))
	{
		if(line.size() < 2 || line.empty() || line.at(0) == ';')
			continue;
		std::vector<std::string> splited;
		char* pch = NULL;
		pch = strtok((char*)line.c_str(), "\r\t ,");
		while (pch != NULL)
		{
			LowerCase(pch);
			splited.push_back(pch);
			pch = strtok(NULL, "\r\t ,");
		}
		if (splited[0].find(":") != std::string::npos)
		{
			splited = std::vector<std::string>(splited.begin() + 1, splited.end());
		}
		ProcessInstruction(code, splited, labels, pc);
	}
	input.close();
	WriteToFile(code, name, pc);
}

void Compiler::Disassembly(uint8_t* buffer, const size_t size)
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
		case PRINT_A:
			printf("PRINT\tA");
			break;
		case PRINT_B:
			printf("PRINT\tB");
			break;
		case PRINT_C:
			printf("PRINT\tC");
			break;
		case 0x18:
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
			printf("\tB, #$0x%02x", opcode[1]);
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

void Compiler::DisassemblyFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Couldn't able to load the file: %s\n", filename);
		return;
	}
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	uint8_t* buffer = (uint8_t*)malloc(size);
	fread(buffer, size, 1, file);
	buffer[size] = '\0';
	fclose(file);

	Disassembly(buffer, size);

}

void CPU8080::debug(const uint32_t pc)
{
	uint8_t* opcode = &memory[pc];
	printf("%x, %x, %x, %x, %x, %x\n ", a, get_bc(), get_de(), get_hl(), pc, sp);
	switch (*opcode)
	{
	case NOP:
		printf("NOP");
		break;
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
	case 0x38:
	case 0xcb:
	case 0xd9:
	case 0xdd:
	case 0xfd:

		break;
	case 0x01:
		printf("LXI\tB, #$0x%02x%02x", opcode[2], opcode[1]);
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
		break;
	case 0x0f:
		printf("RRC");
		break;
	case 0x11:
		printf("LXI\tD, #$0x%02x%02x", opcode[2], opcode[1]);
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
		break;
	case 0x1f:
		printf("RAR");
		break;
		/*case 0x20:
			printf("RIM");
			break;*/
	case 0x21:
		printf("LXI\tH, #$0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0x22:
		printf("SHLD\t0x%02x%02x", opcode[2], opcode[1]);
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
		break;
	case 0x27:
		printf("DAA");
		break;
	case 0x29:
		printf("DAD\tH");
		break;
	case 0x2a:
		printf("LHLD\t 0x%02x%02x", opcode[2], opcode[1]);
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
		break;
	case 0x2f:
		printf("CMA");
		break;
	case 0x31:
		printf("LXI\tSP, #$0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0x32:
		printf("STA\t0x%02x%02x", opcode[2], opcode[1]);
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
		break;
	case 0x37:
		printf("STC");
		break;
	case 0x39:
		printf("DAD\tSP");
		break;
	case 0x3a:
		printf("LDA\t0x%02x%02X", opcode[2], opcode[1]);
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
		break;
	case 0xc3:
		printf("JMP\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xc4:
		printf("CNZ\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xc5:
		printf("PUSH\tB");
		break;
	case 0xc6:
		printf("ADI\t#$0x%02x", opcode[1]);
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
		break;
	case 0xcc:
		printf("CZ\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xcd:
		printf("CALL\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xce:
		printf("ACI\t#$0x%02x", opcode[1]);
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
		break;
	case 0xd3:
		printf("OUT\t#$0x%02x", opcode[1]);
		break;
	case 0xd4:
		printf("CNC\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xd5:
		printf("PUSH\tD");
		break;
	case 0xd6:
		printf("SUI\t#$0x%02x", opcode[1]);
		break;
	case 0xd7:
		printf("RST\t2");
		break;
	case 0xd8:
		printf("RC");
		break;
	case 0xda:
		printf("JC\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xdb:
		printf("IN\t#$0x%02x", opcode[1]);
		break;
	case 0xdc:
		printf("CC\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xde:
		printf("SBI\t#$0x%02x", opcode[1]);
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
		break;
	case 0xe3:
		printf("XTHL");
		break;
	case 0xe4:
		printf("CPO\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xe5:
		printf("PUSH\tH");
		break;
	case 0xe6:
		printf("ANI\t#$0x%02x", opcode[1]);
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
		break;
	case 0xeb:
		printf("XCHG");
		break;
	case 0xec:
		printf("CPE\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xee:
		printf("XRI\t#$0x%02x", opcode[1]);
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
		break;
	case 0xf3:
		printf("DI");
		break;
	case 0xf4:
		printf("CP\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xf5:
		printf("PUSH\tPSW");
		break;
	case 0xf6:
		printf("ORD\t#$0x%02x", opcode[1]);
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
		break;
	case 0xfb:
		printf("EI");
		break;
	case 0xfc:
		printf("CM\t0x%02x%02x", opcode[2], opcode[1]);
		break;
	case 0xfe:
		printf("CPI\t#$0x%02x", opcode[1]);
		break;
	case 0xff:
		printf("RST\t7");
		break;
	default:
		printf("Unknown Instruction: 0x%02x", *opcode);
		break;
	}
	printf("\n");
}

void Compiler::FindLabeles(std::ifstream& const input, std::map<std::string, uint32_t>& labels)
{
	std::string line;
	uint32_t pc = 0;
	while (std::getline(input, line))
	{
		if (line.size() < 2 || line.empty() || line.at(0) == ';')
			continue;
		std::vector<std::string> splited;
		char* pch = NULL;
		pch = strtok((char*)line.c_str(), "\r\t ,");
		while (pch != NULL)
		{
			LowerCase(pch);
			splited.push_back(pch);
			pch = strtok(NULL, "\r\t ,");
		}
		auto offset = 0;
		if (splited[0].find(":") != std::string::npos)
		{
			auto label = std::string(splited[0].substr(0, splited[0].size() - 1));
			offset = 1;
			labels[label] = pc;
		}
		
		if (instruction_sizes.find(splited[offset]) != instruction_sizes.end())
			pc += instruction_sizes.at(splited[offset]);
		else
			pc += 1;
		
 	}
}

void Compiler::WriteToFile(const uint8_t* code, const char* name, const size_t size)
{
	std::ofstream wf(name, std::ios::out | std::ios::binary);
	wf.write((char*)code, size);
	wf.close();
}

void Compiler::ProcessInstruction(uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc)
{
	if (opcodes[0] == "mvi")
	{
		MVI(code, opcodes, pc);
	}
	else if (opcodes[0] == "mov")
	{
		MOV(code, opcodes, pc);
	}
	else if (opcodes[0] == "jnz")
	{
		JMP(JNZ_addr ,code, opcodes, labels, pc);
	}
	else if (opcodes[0] == "jmp")
	{
		JMP(JMP_addr, code, opcodes, labels, pc);
	}
	else if (opcodes[0] == "dcr")
	{
		DCR(code, opcodes, pc);
	}
	else if (opcodes[0] == "ret")
	{
		RETURN(code, opcodes, pc);
	}
	else if (opcodes[0] == "call")
	{
		CALL(code, opcodes, labels, pc);
	}
	else if (opcodes[0] == "print")
	{
		PRINT(code, opcodes, pc);
	}
	else if (opcodes[0] == "hlt")
	{
		HALT(code, opcodes, pc);
	}
}

void Compiler::MVI(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	if (opcodes[1] == "a")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_A_D8;
		code[pc + 1] = val;
		pc += 2;
	} 
	else if (opcodes[1] == "b")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_B_D8;
		code[pc + 1] = val;
		pc += 2;
	}
	else if (opcodes[1] == "c")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_C_D8;
		code[pc + 1] = val;
		pc += 2;
	}
	else if (opcodes[1] == "h")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_H_D8;
		code[pc + 1] = val;
		pc += 2;
	}
	else if (opcodes[1] == "m")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_M_D8;
		code[pc + 1] = val;
		pc += 2;
	}
	else if (opcodes[1] == "l")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_L_D8;
		code[pc + 1] = val;
		pc += 2;
	}
	else if (opcodes[1] == "m")
	{
		auto val = (uint8_t)strtol(opcodes[2].c_str(), NULL, 16);
		code[pc + 0] = MVI_M_D8;
		code[pc + 1] = val;
		pc += 2;
	}
}

void Compiler::MOV(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	if (opcodes[1] == "a")
	{
		if (opcodes[2] == "a")
			code[pc + 0] = MOV_A_A;
		else if (opcodes[2] == "b")
			code[pc + 0] = MOV_A_B;
		else if (opcodes[2] == "d")
			code[pc + 0] = MOV_A_D;
		else if (opcodes[2] == "c")
			code[pc + 0] = MOV_A_C;
		else if (opcodes[2] == "e")
			code[pc + 0] = MOV_A_E;
		else if (opcodes[2] == "h")
			code[pc + 0] = MOV_A_H;
		else if (opcodes[2] == "l")
			code[pc + 0] = MOV_A_L;
		else if (opcodes[2] == "m")
			code[pc + 0] = MOV_A_M;
		pc += 1;
	}
	else if (opcodes[1] == "b")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_B_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_B_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_B_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_B_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_B_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_B_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_B_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_B_M;
		pc += 1;
	}
	else if (opcodes[1] == "c")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_C_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_C_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_C_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_C_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_C_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_C_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_C_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_C_M;
		pc += 1;
	}
	else if (opcodes[1] == "d")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_D_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_D_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_D_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_D_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_D_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_D_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_D_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_D_M;
		pc += 1;
	}
	else if (opcodes[1] == "e")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_E_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_E_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_E_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_E_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_E_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_E_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_E_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_E_M;
		pc += 1;
	}
	else if (opcodes[1] == "h")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_H_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_H_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_H_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_H_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_H_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_H_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_H_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_H_M;
		pc += 1;
	}
	else if (opcodes[1] == "l")
	{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_L_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_L_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_L_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_L_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_L_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_L_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_L_L;
		else if (opcodes[1] == "m")
			code[pc + 0] = MOV_L_M;
		pc += 1;
	}
	else if (opcodes[1] == "m")
		{
		if (opcodes[1] == "a")
			code[pc + 0] = MOV_M_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = MOV_M_B;
		else if (opcodes[1] == "d")
			code[pc + 0] = MOV_M_D;
		else if (opcodes[1] == "c")
			code[pc + 0] = MOV_M_C;
		else if (opcodes[1] == "e")
			code[pc + 0] = MOV_M_E;
		else if (opcodes[1] == "h")
			code[pc + 0] = MOV_M_H;
		else if (opcodes[1] == "l")
			code[pc + 0] = MOV_M_L;
		pc += 1;
	}
}

void Compiler::JMP(uint32_t type, uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc)
{
	code[pc + 0] = type;
	uint16_t val = labels[opcodes[1]];
	uint8_t a = val << 8;
	uint8_t b = val;
	code[pc + 1] = b;
	code[pc + 2] = a;
	pc += 3;
}

void Compiler::HALT(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	code[pc + 0] = HLT;
	pc += 1;
}

void Compiler::RETURN(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	code[pc + 0] = RET;
	pc += 1;
}

void Compiler::DCR(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	if (opcodes[1] == "a")
		code[pc + 0] = DCR_A;
	if (opcodes[1] == "b")
		code[pc + 0] = DCR_B;
	if (opcodes[1] == "c")
		code[pc + 0] = DCR_C;
	pc += 1;
}

void Compiler::PRINT(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc)
{
	if (opcodes[0] == "print")
	{
		if(opcodes[1] == "a")
			code[pc + 0] = PRINT_A;
		else if (opcodes[1] == "b")
			code[pc + 0] = PRINT_B;
		else if (opcodes[1] == "c")
			code[pc + 0] = PRINT_C;
		pc += 1;
	}
}

void Compiler::CALL(uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc)
{
	if (opcodes[0] == "call")
	{
		uint8_t val = labels[opcodes[1]];
		code[pc + 0] = CALL_addr;
		uint8_t a = val << 8;
		uint8_t b = val;
		code[pc + 1] = b;
		code[pc + 2] = a;
		pc += 3;
	}
}
