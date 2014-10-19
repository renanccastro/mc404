#ifndef READASM_147775
#define READASM_147775

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "utils.h"
#include <errno.h>
#include "list_structure.h"
#include "dictionary_structure.h"

#define BUFFER_SIZE 255

typedef enum STR2INT_ERROR { SUCCESS, RANGE_ERROR, INCONVERTIBLE } STR2INT_ERROR;


Node* read_ASM_file(char* file_name);
void chomp_string(char* string);
int is_valid_label(char* string, size_t length);
int is_valid_instruction(char* string);
int is_valid_directive(char* string);
int is_valid_sym(char* string, size_t length);
int instruction_code(char* string, size_t length);
void convertToUpperCase(char *sPtr);
STR2INT_ERROR str2int (long *i, char const *s, int *base);
#endif