#include "assembler.h"


//Primeira etapa da montagem: Monta a tabela de rótulos!
Dictionary* labels_dictionary(Node* file){
    Dictionary* dictionary = malloc(sizeof(Dictionary));
    int current_ias_word = 0x000, i, line_contains_label = 0, value;
    InstructionPosition current_position = kLeft;
    size_t string_size;
    char * string;
    for (Node* line = file; line != NULL; line = line->next){
        
        if (current_ias_word > IAS_MAX_MEM_WORDS){
            printf("ERRO: O arquivo ultrapassou o limite de memória do IAS\n");
            exit(EXIT_FAILURE);
        }
        i = 0;
        line_contains_label = 0;
        //agora vemos cada palavra da linha
        for (Node* word = line->data.list; word != NULL; word = word->next, i++) {
        
            string = word->data.word;
            string_size = strlen(string);

            //se começar um comentário, passa pra outra linha
            if (!strcmp(string,"#")) {
                break;
            }
            //se for um rotulo, e estiver em primeiro na linha
            if(is_valid_label(string, string_size)){
                if (i == 0) {
                    printf("Label '%s' encontrado, %d\n", string, i);
                    line_contains_label = 1;
                    dictionary_put(dictionary, string, current_ias_word, current_position);
                }else{
                    printf("ERROR: O rótulo deve estar no começo da linha\n");
                    exit(EXIT_FAILURE);
                }
            }
            
            //verificamos as diretrizes
            if (!strcmp(string, ".ORG")) {
                if (word->next != NULL) {
                    if (str2int(&value, word->next->data.word) == SUCCESS) {
                        
                    }
                    atoi(word->next->data.word);
                }
            }
            if(is_valid_instruction(string)){
                //incrementa os contadores do arquivo!
                current_ias_word += ((current_position + 1) / 2);
                current_position =  ((current_position + 1) % 2);
            }
        }
    }

    return dictionary;
}
