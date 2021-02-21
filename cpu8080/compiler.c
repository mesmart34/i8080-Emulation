#include "compiler.h"

static bool found_semicolon(const char* line, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		if (line[i] == ';')
		{
			return true;
		}
	}
}

static void to_lower_case(char* p)
{
	for (; *p; ++p) *p = tolower(*p);
}

static void mvi(char* code, char* opcodes[3], size_t size, int* pc)
{
	if (strcmp(opcodes[1], "a") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_A_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
	if (strcmp(opcodes[1], "b") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_B_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
	if (strcmp(opcodes[1], "c") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_C_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
	if (strcmp(opcodes[1], "h") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_H_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
	if (strcmp(opcodes[1], "m") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_M_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
	if (strcmp(opcodes[1], "m") == 0)
	{
		uint8_t val = (int)strtol(opcodes[2], NULL, 16);
		code[*pc + 0] = MVI_M_D8;
		code[*pc + 1] = val;
		*pc += 2;
	}
}

static void mov(char* code, char* opcodes[3], size_t size, int* pc)
{
	if (strcmp(opcodes[1], "a") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_A_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_A_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_A_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_A_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_A_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_A_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_A_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_A_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "b") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_B_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_B_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_B_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_B_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_B_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_B_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_B_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_B_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "c") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_C_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_C_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_C_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_C_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_C_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_C_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_C_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_C_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "d") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_D_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_D_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_D_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_D_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_D_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_D_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_D_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_D_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "e") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_E_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_E_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_E_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_E_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_E_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_E_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_E_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_E_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "h") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_H_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_H_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_H_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_H_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_H_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_H_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_H_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_H_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "l") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_L_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_L_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_L_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_L_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_L_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_L_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_L_L;
		else if (strcmp(opcodes[2], "m") == 0)
			code[*pc + 0] = MOV_L_M;
		*pc += 1;
	}
	else if (strcmp(opcodes[1], "m") == 0)
	{
		if (strcmp(opcodes[2], "a") == 0)
			code[*pc + 0] = MOV_M_A;
		else if (strcmp(opcodes[2], "b") == 0)
			code[*pc + 0] = MOV_M_B;
		else if (strcmp(opcodes[2], "d") == 0)
			code[*pc + 0] = MOV_M_D;
		else if (strcmp(opcodes[2], "c") == 0)
			code[*pc + 0] = MOV_M_C;
		else if (strcmp(opcodes[2], "e") == 0)
			code[*pc + 0] = MOV_M_E;
		else if (strcmp(opcodes[2], "h") == 0)
			code[*pc + 0] = MOV_M_H;
		else if (strcmp(opcodes[2], "l") == 0)
			code[*pc + 0] = MOV_M_L;
		*pc += 1;
	}
}

static void jnz(char* code, char* opcodes[3], size_t size, int* pc)
{
	code[*pc + 0] = JNZ_addr;
	uint16_t val = (int)strtol(opcodes[1], NULL, 16);
	uint8_t a = val << 8;
	uint8_t b = val;
	code[*pc + 1] = b;
	code[*pc + 2] = a;
	*pc += 3;
}

static void hlt(char* code, char* opcodes[3], size_t size, int* pc)
{
	code[*pc + 0] = HLT;
	*pc += 1;
}

static void dcr(char* code, char* opcodes[3], size_t size, int* pc)
{
	if (strcmp(opcodes[1], "a") == 0)
		code[*pc + 0] = DCR_A;
	if (strcmp(opcodes[1], "b") == 0)
		code[*pc + 0] = DCR_B;
	if (strcmp(opcodes[1], "c") == 0)
		code[*pc + 0] = DCR_C; 
	*pc += 1;
	
}

static void print(char* code, char* opcodes[3], size_t size, int* pc)
{
	if (strcmp(opcodes[1], "a") == 0)
	{
		code[*pc + 0] = PRINT_A;
		*pc += 1;
	}
	if (strcmp(opcodes[1], "b") == 0)
	{
		code[*pc + 0] = PRINT_B;
		*pc += 1;
	}
	if (strcmp(opcodes[1], "c") == 0)
	{
		code[*pc + 0] = PRINT_C;
		*pc += 1;
	}
}

static void process_instruction(char *code, char* opcodes[3], size_t size, int *pc)
{
	opcodes[size - 1][strcspn(opcodes[size - 1], "\n")] = 0;
	if (strcmp(opcodes[0], "mvi") == 0)
	{
		mvi(code, opcodes, size, pc);
	}
	if (strcmp(opcodes[0], "mov") == 0)
	{
		mov(code, opcodes, size, pc);
	}
	if (strcmp(opcodes[0], "print") == 0)
	{
		print(code, opcodes, size, pc);
	}
	if (strcmp(opcodes[0], "dcr") == 0)
	{
		dcr(code, opcodes, size, pc);
	}
	if (strcmp(opcodes[0], "hlt") == 0)
	{
		hlt(code, opcodes, size, pc);
	}
	if (strcmp(opcodes[0], "jnz") == 0)
	{
		jnz(code, opcodes, size, pc);
	}
}

const char* compile(const char* filename, uint32_t* buf_size)
{
	char* code = malloc(sizeof(char) * 256);
	int pc = 0;
	FILE* file = fopen(filename, "r");
	size_t read = 0;
	size_t len = 255;
	char line[255];
	while (fgets(line, len, file)) {
		
		if(line[0] == ';')
			continue;

		char* pch;
		size_t count = 0;
		pch = strtok(line, " ,");
		char* opcodes[3];
		while (pch != NULL)
		{
			to_lower_case(pch);
			opcodes[count] = pch;
			count++;
			pch = strtok(NULL, " ,");
		}
		process_instruction(&code[0], opcodes, count, &pc);
	}
	fclose(file);
	*buf_size = pc;
	return code;
}
