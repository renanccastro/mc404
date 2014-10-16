#include <stdio.h>
#include <stdlib.h>
#include "readASM.h"
#include "utils.h"
#import <math.h>
#include "dictionary_structure.h"
#define IAS_MAX_MEM_WORDS 0x400


Dictionary* labels_dictionary(Node* file, Dictionary** set_constants);
void assemble_file(Node* file, Dictionary* label_dictionary, Dictionary* set_constants, char* output_filename);