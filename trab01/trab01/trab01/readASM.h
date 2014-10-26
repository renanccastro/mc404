#ifndef READASM_147775
#define READASM_147775

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include <errno.h>
#include "list_structure.h"
#include "dictionary_structure.h"

#define BUFFER_SIZE 255



Node* read_ASM_file(char* file_name);
#endif