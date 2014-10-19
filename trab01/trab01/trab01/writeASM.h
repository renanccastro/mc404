#ifndef WRITEASM_147775
#define WRITEASM_147775

#include <stdio.h>
#include "readASM.h"

void print_word(FILE* output, int current_ias_word, long int first_parameter);
void print_vector(FILE* output, int *current_ias_word, long int first_parameter, long int second_parameter);
void print_instruction(FILE* output, Node* line, Dictionary* labels, Dictionary* sets,int current_ias_word, InstructionPosition position, int line_number);

#endif