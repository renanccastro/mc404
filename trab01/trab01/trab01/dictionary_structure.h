//Estrutura baseada no exemplo do livro : The C Programming Language
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HASHSIZE 101

typedef enum InstructionPosition{
    kLeft,
    kRight
} InstructionPosition;

typedef struct nlist { /* elemento da tabela */
    struct nlist *next; /* próximo elemento */
    char *name; /* nome da entrada(rótulo) */
    int location;
    InstructionPosition position;
} DictionaryNode;

typedef struct dictionary { /* elemento da tabela */
    DictionaryNode * hashtable[HASHSIZE];
} Dictionary;

DictionaryNode *dictionary_put(Dictionary* dictionary, char *name, int location, InstructionPosition position);
DictionaryNode *dictionary_get(Dictionary* dictionary, char *s);