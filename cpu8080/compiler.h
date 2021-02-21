#ifndef COMPILER
#define COMPILER_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu8080.h"
#include <ctype.h>

const char* compile(const char* filename, uint32_t* buf_size);

#endif