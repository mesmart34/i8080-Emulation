#include "compiler.h"

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
	uint8_t* code = new uint8_t(256);
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
			auto label = std::string(splited[0].substr(0, splited[0].size() - 1));
			labels[label] = pc - 1;
			splited = std::vector<std::string>(splited.begin() + 1, splited.end());
		}
		ProcessInstruction(code, splited, labels, pc);
	}
	input.close();
	WriteToFile(code, name, pc);
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
	//auto val = (uint16_t)strtol(opcodes[1].c_str(), NULL, 16);
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
