#include "compiler.h"

static void process_instruction(FILE* file, char * token)
{
	if (strcmp(token, "MVI") == 0)
	{
		token = strtok(NULL, ", ");
		if (strcmp(token, "A") == 0)
		{
			token = strtok(NULL, ", ");
			fwrite(MVI_A_D8, sizeof(uint8_t), 1, file);
		}
		if (strcmp(token, "B") == 0)
		{
			token = strtok(NULL, ", ");
			fwrite(MVI_B_D8, sizeof(uint8_t), 1, file);
		}
	}
}

void compile(const char* buffer, uint32_t size, const char* output)
{
	FILE* file = fopen(output, "wb");
	char* token = strtok(buffer, ", ");
	while (token != NULL)
	{
		process_instruction(file, token);
	}
	fclose(file);
}
