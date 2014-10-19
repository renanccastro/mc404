#ifndef UTILS_147775
#define UTILS_147775

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define IAS_MAX_MEM_WORDS 0x400


void show_build_error(char* error, int line);
void show_build_warning(char* warning, int line);
#endif