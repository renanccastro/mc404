#include <stdio.h>
#include <stdlib.h>
#include "readASM.h"
#include "dictionary_structure.h"
#define IAS_MAX_MEM_WORDS 0x400


Dictionary* labels_dictionary(Node* file);