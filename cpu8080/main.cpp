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

void run_diag(CPU8080& cpu, Compiler& compiler)
{
	size_t size = 0;
	auto data = (uint8_t*)read_file("cpudiag.bin", size);
	load_to_memory(cpu, "cpudiag.bin", 0x100);
	cpu.memory[0] = 0xc3;
	cpu.memory[1] = 0;
	cpu.memory[2] = 0x01; 
	cpu.memory[368] = 0x7;
	cpu.memory[0x59c] = 0xc3;  
	cpu.memory[0x59d] = 0xc2;
	cpu.memory[0x59e] = 0x05;
	compiler.Disassembly(data, size);
	cpu.run(0, false);
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
	run_diag(cpu, cmp);
	//load_space_inv(cpu, cmp);
	getchar();
	return 0;
}