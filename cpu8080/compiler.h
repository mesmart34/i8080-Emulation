#ifndef COMPILER
#define COMPILER_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "cpu8080.h"

void compile(const char* buffer, uint32_t size, const char* output);

#endif