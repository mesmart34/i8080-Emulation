#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cpu8080.h"
#include "compiler.h"

static const uint8_t* read_file(const char* path, size_t& buffer_size)
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
	uint8_t* buffer = (uint8_t*)malloc(size);
	fread(buffer, size, 1, file);
	buffer[size] = '\0';
	fclose(file);
	buffer_size = size;
	return buffer;
}

void load_to_memory(CPU8080& cpu, const char* path, uint32_t offset)
{
	size_t size = 0;
	const uint8_t* buffer = read_file(path, size);
	cpu.load_program(buffer, size, offset);
}

int main(int argc, char** argv)
{
	/*cpu8080 cpu;
	cpu8080_init(&cpu);
	write_program_to_file("main.asm");
	load_to_memory(&cpu, "main.asm", 0);
	uint32_t buf_size;
	uint8_t* buffer = read_file("main.asm", &buf_size);
	cpu8080_disassembly(&cpu, buffer, buf_size);
	cpu8080_run(&cpu);*/
	/*load_to_memory(&cpu, "space invaders/invaders.h", 0);
	load_to_memory(&cpu, "space invaders/invaders.g", 0x800);
	load_to_memory(&cpu, "space invaders/invaders.f", 0x1000);
	load_to_memory(&cpu, "space invaders/invaders.e", 0x1800);8?
	uint32_t buf_size = 0;
	uint8_t* compiled_code = compile("test.asm", &buf_size);
	write_program_to_file("test.bin", compiled_code, buf_size);
	load_to_memory(&cpu, "test.bin", 0);
	cpu8080_disassembly(&cpu, compiled_code, buf_size);
	
	cpu8080_run(&cpu);*/
	CPU8080 cpu = CPU8080();
	Compiler cmp = Compiler();
	cmp.Compile("test.asm", "test2.bin");
	size_t size = 0;
	auto data = (uint8_t*)read_file("test2.bin", size);
	load_to_memory(cpu, "test2.bin", 0);
	cpu.disassembly(data, size);
	cpu.run(0);
	getchar();
	return 0;
}