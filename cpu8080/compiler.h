#pragma once

#include <stdint.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "CPU8080.h"


class Compiler
{
public:
	Compiler() = default;

	void Compile(const char* filename, const char* name);
	void Disassembly(uint8_t* buffer, const size_t size);
	void DisassemblyFromFile(const char* filename);
private:
	void FindLabeles(std::ifstream&  input, std::map<std::string, uint32_t>& labels);
	void WriteToFile(const uint8_t* code, const char* name, const size_t size);
	void ProcessInstruction(uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc);
	void MVI(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void MOV(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void JMP(uint32_t type, uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc);
	void HALT(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void DCR(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void PRINT(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void CALL(uint8_t* code, const std::vector<std::string>& opcodes, std::map<std::string, uint32_t>& labels, uint32_t& pc);
	void RETURN(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void LowerCase(char* p);
	void STA(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void LXI(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void XRA(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void LHLD(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void ADD(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
	void CMP(uint8_t* code, const std::vector<std::string>& opcodes, uint32_t& pc);
};