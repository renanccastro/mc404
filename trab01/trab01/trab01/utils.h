#ifndef UTILS_147775
#define UTILS_147775

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "list_structure.h"
#include "dictionary_structure.h"

#define IAS_MAX_MEM_WORDS 0x400
typedef enum STR2INT_ERROR { SUCCESS, RANGE_ERROR, INCONVERTIBLE } STR2INT_ERROR;


void show_build_error(char* error, int line);
void show_build_warning(char* warning, int line);

void chomp_string(char* string);
int is_valid_label(char* string, size_t length);
int is_valid_instruction(char* string);
int is_valid_directive(char* string);
int is_valid_sym(char* string, size_t length);
void convertToUpperCase(char *sPtr);
STR2INT_ERROR str2int (long *i, char const *s, int *base);
int match_regexp(char* string, char* regexp);

#endif