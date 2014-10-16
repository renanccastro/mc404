#include "assembler.h"

//valida uma diretiva, e retorna o número de argumentos(incluindo a diretiva) esperada
int validateDirective(Dictionary* sets_dictionary, Node* word, int* current_ias_word, InstructionPosition* current_position, int line){
    char* string = word->data.word;
    //pega os argumentos da possível diretiva
    char* first_argument = word->next ? word->next->data.word : NULL;
    char* second_argument = first_argument ? (word->next->next ? word->next->next->data.word : NULL) : NULL;
    
    long value, value2;
    int base = 10;
    //pode ser uma diretiva, e está no lugar correto.
    if (is_valid_directive(string)) {
        //todas possuem pelo menos um argumento
        if (first_argument) {
            if (str2int(&value, first_argument,&base) == SUCCESS) {
                if (!strcmp(string, ".ALIGN") ) {
                    if (base == 10 && value<=1023 && value>=0) {
                        //pega o proximo numero multiplo de 'value'
                        *current_ias_word = ((*current_ias_word / (int)value) + 1) * (int)value;
                        *current_position = kLeft;
                        show_build_warning("Encontrei um .Align!");
                        return 2;
                    }else{
                        show_build_error("Parâmetro inválido para diretiva .ALIGN", line);
                    }
                }else if (!strcmp(string, ".ORG")){
                    //se for decimal ou outra base
                    if ((base == 10 && value>=0 && value <= 1023) || base != 10){
                        *current_ias_word = (int)value;
                        *current_position = kLeft;
                        show_build_warning("Encontrei um .org!");
                        return 2;
                    }else{
                        show_build_error("Parâmetro inválido para diretiva .ORG", line);
                    }
                }else if(!strcmp(string, ".WORD")){
                    if (*current_position == kLeft) {
                        (*current_ias_word)++;
                        show_build_warning("Encontrei um .word!");
                    }else{
                        show_build_error("Não é possível colocar um valor desalinhado na memória!", line);
                    }
                    return 2;
                }else if (!strcmp(string, ".WFILL")){
                    if (second_argument && (str2int(&value2, second_argument,&base) == SUCCESS || is_valid_sym(second_argument, strlen(second_argument)))) {
                        //se é possível colocar uma palavra inteira aqui
                        if (*current_position == kLeft) {
                            (*current_ias_word) += value;
                            show_build_warning("Encontrei um .wfill!");
                        }else{
                            show_build_error("Não é possível colocar um vetor desalinhado na memória!", line);
                        }
                        return 3;
                    }
                }
                //se não for um número o primeiro parâmetro
            }else {
                if(!strcmp(string, ".WORD") && is_valid_sym(first_argument, strlen(first_argument))){
                    (*current_ias_word)++;
                    show_build_warning("Encontrei um .word");
                    return 2;
                }else if(!strcmp(string, ".SET") && is_valid_sym(first_argument, strlen(first_argument)) && second_argument && (str2int(&value2, second_argument,&base) == SUCCESS)){
                    dictionary_put(sets_dictionary, first_argument, value2, kSetDirective);
                    show_build_warning("Encontrei um .set!");
                    return 3;
                }
            }
        }else{
            show_build_error("Diretiva sem parâmetros!", line);
        }
        
    }else{
        show_build_error("Diretiva inválida", line);
    }
    return -1;
}



//Primeira etapa da montagem: Monta a tabela de rótulos!
Dictionary* labels_dictionary(Node* file, Dictionary** set_constants){
    Dictionary* dictionary = malloc(sizeof(Dictionary));
    *set_constants = malloc(sizeof(Dictionary));
    int current_ias_word = 0, i, line_contains_label = 0, number_of_expected_words = 0, line_number = 1;
    char* first_argument = NULL, *second_argument = NULL;
    InstructionPosition current_position = kLeft;
    size_t string_size;
    char * string;
    for (Node* line = file; line != NULL; line = line->next){
        
        if (current_ias_word > IAS_MAX_MEM_WORDS){
            show_build_error("O arquivo ultrapassou o limite de memória do IAS",-1);
        }
        //contador de palavra
        i = 0;
        line_contains_label = 0;
        number_of_expected_words = 0;
        //agora vemos cada palavra da linha
        for (Node* word = line->data.list; word != NULL; word = word->next) {
            //pega o primeiro e o segundo argumento de uma possível instrução atual.
            first_argument = word->next ? word->next->data.word : NULL;
            second_argument = first_argument ? (word->next->next ? word->next->next->data.word : NULL) : NULL;
            
            string = word->data.word;
            string_size = strlen(string);
            
            //se for um rotulo, e estiver em primeiro na linha
            if(is_valid_label(string, string_size)){
                if (i == 0) {
                    //TODO: Retirar depois
                    printf("Label '%s' encontrado, %d\n", string, i);
                    line_contains_label = 1;
                    dictionary_put(dictionary, string, current_ias_word, current_position);
                    number_of_expected_words += 1;
                }else{
                    show_build_error("O rótulo deve estar no começo da linha.\n",line_number);
                }
            }
            
            //se pode ser uma diretiva
            if (*string == '.') {
                if (i <= 1) {
                    number_of_expected_words += validateDirective(*set_constants, word, &current_ias_word, &current_position, line_number);
                }
            }
            if(is_valid_instruction(string)){
                //incrementa os contadores do arquivo!
                current_ias_word += ((current_position + 1) / 2);
                current_position =  ((current_position + 1) % 2);
                if (!strcmp(string,"RSH") || !strcmp(string,"LSH")) {
                    number_of_expected_words += 1;
                }else{
                    number_of_expected_words += 2;
                }
            }
            i++;
        }
        
        //Verifica se na linha só tem coisas válidas.
        if (i > number_of_expected_words) {
            show_build_error("Caracteres sobrando", line_number);
        }
        line_number++;
    }
    
    return dictionary;
}


void assemble_file(Node* file, Dictionary* label_dictionary, Dictionary* set_constants, char* output_filename){
    
}


