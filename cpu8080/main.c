#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu8080.h"

uint32_t disassembly(const char* buffer, uint16_t pc)
{
	uint8_t* opcode = &buffer[pc];
	uint32_t opbytes = 1;
	printf("0x%02X\t", pc);
	switch (*opcode)
	{
		case 0x00:
		{
			printf("NOP");
		} break;
		case 0x01:
		{
			printf("LXI\tB, #$0x%02x%02x", opcode[2], opcode[1]);
			opcode = 3;
		} break;
		case 0x02:
		{
			printf("STAX\tB");
		} break;
		case 0x03:
		{
			printf("INX\tB");
		} break;
		case 0x04:
		{
			printf("INR\tB");
		} break;
		case 0x05:
		{
			printf("DCR\tB");
		} break;
		case 0x06:
		{
			printf("MVI\tB, #0x%02x%02x");
			opcode = 2;
		} break;
		case 0x07:
		{
			printf("RLC");
		} break;
		case 0x09:
		{
			printf("DAD\tB");
		} break;
		case 0x0a:
		{
			printf("LDAX\tB");
		} break;
		case 0x0b:
		{
			printf("DCX\tB");
		} break;
		case 0x0c:
		{
			printf("IHR\tC");
		} break;
		case 0x0d:
		{
			printf("DCR\tC");
		} break;
		case 0x0e:
		{
			printf("MVI\tC, #0x%020x%02x");
			opcode = 2;
		} break;
		case 0x0f:
		{
			printf("RRC");
		} break;
		case 0x11:
		{
			printf("LXI\tD, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x12:
		{
			printf("STAX\tD");
		} break;
		case 0x13:
		{
			printf("INX\tD");
		} break;
		case 0x14:
		{
			printf("INR\tD");
		} break;
		case 0x15:
		{
			printf("DCR\tD");
		} break;
		case 0x16:
		{
			printf("DCR\tD");
		} break;
		case 0x17:
		{
			printf("RAL");
		} break;
		case 0x19:
		{
			printf("DAD\tD");
		} break;
		case 0x1a:
		{
			printf("LDAX\tD");
		} break;
		case 0x1b:
		{
			printf("DCX\tD");
		} break;
		case 0x1c:
		{
			printf("INR\tE");
		} break;
		case 0x1d:
		{
			printf("DCR\tE");
		} break;
		case 0x1e:
		{
			printf("MVI\tE, #$0x%02x", opcode[1]);
			opbytes = 2;
		} break;
		case 0x1f:
		{
			printf("RAR");
		} break;
		case 0x20:
		{
			printf("RIM");
		} break;
		case 0x21:
		{
			printf("LXI\tH, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x22:
		{
			printf("SHLD\t0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x23:
		{
			printf("INX\tH");
		} break;
		case 0x24:
		{
			printf("INR\tH");
		} break;
		case 0x25:
		{
			printf("DCR\tH");
		} break;
		case 0x26:
		{
			printf("MVI\tH, #$0x%02x", opcode[1]);
			opbytes = 2;
		} break;
		case 0x27:
		{
			printf("DAA");
		} break;
		case 0x29:
		{
			printf("DAD\tH");
		} break;
		case 0x2a:
		{
			printf("LHLD\t 0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x2b:
		{
			printf("DCX\tH");
		} break;
		case 0x2c:
		{
			printf("INR\tL");
		} break;
		case 0x2d:
		{
			printf("DCR\tL");
		} break;
		case 0x2e:
		{
			printf("MVI\tL, #$0x%02x", opcode[1]);
			opbytes = 2;
		} break;
		case 0x2f:
		{
			printf("CMA");
		} break;
		case 0x30:
		{
			printf("SIM");
		} break;
		case 0x31:
		{
			printf("LXI\tSP, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x32:
		{
			printf("STA\t0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x33:
		{
			printf("INX\tSP"); 
		} break;
		case 0x34:
		{
			printf("INR\tM");
		} break;
		case 0x35:
		{
			printf("DCR\tM");
		} break;
		case 0x36:
		{
			printf("MVI\tM, #$0x%02x", opcode[1]);
			opbytes = 2;
		} break;
		case 0x37:
		{
			printf("STC");
		} break;
		case 0x39:
		{
			printf("DAD\tSP");
		} break;
		case 0x3a:
		{
			printf("LDA\t0x%02x%02X", opcode[2], opcode[1]);
			opbytes = 3;
		} break;
		case 0x3b:
		{
			printf("DCX\tSP"); 
		} break;
		case 0x3c:
		{
			printf("INR\tA");
		} break;
		case 0x3d:
		{
			printf("DCR\tA");
		} break;
		case 0x3e:
		{
			printf("MVI\tA, #$0x%02x", opcode[1]);
			opbytes = 2;
		} break;
		case 0x3f:
		{
			printf("CMC");
		} break;
		case 0x40:
		{
			printf("MOV\tB, B");
		} break;
		case 0x41:
		{
			printf("MOV\tB, C");
		} break;
		case 0x42:
		{
			printf("MOV\tB, D");
		} break;
		case 0x43:
		{
			printf("MOV\tB, E");
		} break;
		case 0x44:
		{
			printf("MOV\tB, H");
		} break;
		case 0x45:
		{
			printf("MOV\tB, L");
		} break;
		case 0x46:
		{
			printf("MOV\tB, M");
		} break;
		case 0x47:
		{
			printf("MOV\tB, A");
		} break;
		default:
		{
			printf("Unknown instruction", *opcode);
		} break;
	}
	printf("\n");
	return opbytes;
}

const uint8_t* read_file(const char* path, uint32_t* buffer_size)
{
	FILE* file = fopen(path, "rb");
	if (file == NULL)
	{
		printf("Couldn't able to load the file: %s\n", path);
		return NULL;
	}
	fseek(file, 0L, SEEK_END);
	uint64_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	const uint8_t* buffer = malloc(size);
	fread(buffer, size, 1, file);
	fclose(file);
	*buffer_size = size;
	return buffer;
}

void write_to_file(const char* path)
{
	int instructions = 6;
	uint32_t size = sizeof(uint8_t) * instructions;
	uint8_t* data = malloc(size);
	*(data + 0) = 0x00;
	*(data + 1) = 0x47;
	*(data + 2) = 0x46;
	*(data + 3) = 0x03;
	*(data + 4) = 0x04;
	*(data + 5) = 0x47;
	FILE* file = fopen(path, "wb");
	if (file == NULL)
	{
		printf("Couldn't able to load the file: %s\n", path);
		return NULL;
	}
	fwrite(data, sizeof(uint8_t), instructions, file);
	fclose(file);
}

int main()
{
	cpu8080 cpu;
	cpu8080_init(&cpu);	
	uint32_t size = 0;
	write_to_file("main.obj");
	const uint8_t* buffer = read_file("main.obj", &size);
	while (cpu.pc < size)
	{
		cpu.pc += disassembly(buffer, cpu.pc);
	}
	getchar();
	return 0;
}