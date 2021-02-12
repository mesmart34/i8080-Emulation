#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cpu8080.h"
#include "compiler.h"

static const uint8_t* read_file(const char* path, uint32_t* buffer_size)
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
	uint8_t* buffer = malloc(size);
	fread(buffer, size, 1, file);
	buffer[size] = '\0';
	fclose(file);
	*buffer_size = size;
	return buffer;
}

static void write_program_to_file(const char* path)
{
	int instructions = 9;
	uint32_t size = sizeof(uint8_t) * instructions;
	uint8_t* data = malloc(size);
	*(data + 0) = MVI_B_D8;
	*(data + 1) = 0x10;
	*(data + 2) = PRINT_B;
	*(data + 3) = DCR_B;
	*(data + 4) = PRINT_B;
	*(data + 5) = JNZ_addr;
	*(data + 6) = 0x02;
	*(data + 7) = 0x00;
	*(data + 8) = HLT;
	FILE* file = fopen(path, "wb");
	if (file == NULL)
	{
		printf("Couldn't able to load the file: %s\n", path);
		return NULL;
	}
	fwrite(data, sizeof(uint8_t), instructions, file);
	fclose(file);
}

void load_to_memory(cpu8080* cpu, const char* path, uint32_t offset)
{
	uint32_t size = 0;
	const uint8_t* buffer = read_file(path, &size);
	cpu8080_load_program(cpu, buffer, size, offset);
}

int main(int argc, char** argv)
{
	cpu8080 cpu;
	cpu8080_init(&cpu);
	write_program_to_file("main.asm");
	load_to_memory(&cpu, "main.asm", 0);
	uint32_t buf_size;
	uint8_t* buffer = read_file("main.asm", &buf_size);
	//cpu8080_disassembly(&cpu, buffer, buf_size);
	cpu8080_run(&cpu);
	/*load_to_memory(&cpu, "space invaders/invaders.h", 0);
	load_to_memory(&cpu, "space invaders/invaders.g", 0x800);
	load_to_memory(&cpu, "space invaders/invaders.f", 0x1000);
	load_to_memory(&cpu, "space invaders/invaders.e", 0x1800);*/
	//cpu8080_run(&cpu);
	getchar();
	return 0;
}