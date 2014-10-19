#ifndef ASSEMBLER_147775
#define ASSEMBLER_147775

#include <stdio.h>
#include <stdlib.h>
#include "readASM.h"
#include <unistd.h>
#include "utils.h"
#import <math.h>
#include "dictionary_structure.h"
#include "writeASM.h"


Dictionary* labels_dictionary(Node* file, Dictionary** set_constants);
void assemble_file(Node* file, Dictionary* label_dictionary, Dictionary* set_constants, char* output_filename);

#endif