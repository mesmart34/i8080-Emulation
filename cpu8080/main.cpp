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

void run_test(CPU8080& cpu, Compiler& compiler)
{
	size_t size = 0;
	/*compiler.Compile("fib.asm", "fib.bin");
	auto data = (uint8_t*)read_file("fib.bin", size);*/
	load_to_memory(cpu, "fib.bin", 0);
	//compiler.Disassembly(data, size);
	cpu.run(0, false);
}

void run_diag(CPU8080& cpu, Compiler& compiler)
{
	size_t size = 0;
	auto data = (uint8_t*)read_file("diag.bin", size);
	load_to_memory(cpu, "diag.bin", 0);
	//compiler.Disassembly(data, size);
	cpu.run(0, true);
}

void load_space_inv(CPU8080& cpu, Compiler& compiler)
{
	load_to_memory(cpu, "space invaders/invaders.h", 0);
	compiler.DisassemblyFromFile("space invaders/invaders.h");
	load_to_memory(cpu, "space invaders/invaders.g", 0x800);
	compiler.DisassemblyFromFile("space invaders/invaders.g");
	load_to_memory(cpu, "space invaders/invaders.f", 0x1000);
	compiler.DisassemblyFromFile("space invaders/invaders.f");
	load_to_memory(cpu, "space invaders/invaders.e", 0x1800);
	compiler.DisassemblyFromFile("space invaders/invaders.e");
	//cpu.run(0, true);
}

int main(int argc, char** argv)
{
	CPU8080 cpu = CPU8080();
	Compiler cmp = Compiler();
	//run_diag(cpu, cmp);
	run_test(cpu, cmp);
	//load_space_inv(cpu, cmp);
	getchar();
	return 0;
}