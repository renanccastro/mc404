#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "list_structure.h"

#define BUFFER_SIZE 255

typedef enum STR2INT_ERROR { SUCCESS, RANGE_ERROR, INCONVERTIBLE } STR2INT_ERROR;


Node* read_ASM_file(char* file_name);
void chomp_string(char* string);
int is_valid_label(char* string, size_t length);
int is_valid_instruction(char* string);
void convertToLowerCase(char *sPtr);