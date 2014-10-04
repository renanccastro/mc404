#include "assembler.h"


Dictionary* labels_dictionary(Node* file){
    Dictionary* dictionary = malloc(sizeof(Dictionary));
    for (Node* line = file; line != NULL; line = line->next){
        int i = 0;
        for (Node* word = file->data.list; word != NULL; word = word->next, i++) {
            //agora vemos cada palavra da linha
            
        }
    }

    return dictionary;
}
