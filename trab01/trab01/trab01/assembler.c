#include "assembler.h"

//valida uma diretiva, e retorna o número de argumentos(incluindo a diretiva) esperada
int validateDirective(char* string,char* first_argument, char* second_argument, int* current_ias_word, InstructionPosition* current_position, int line){
    long value;
    int base = 10;
    //se for um número.
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
                        return 2;
                    }else{
                        show_build_error("Parâmetro inválido para diretiva .ALIGN", line);
                    }
                }else if (!strcmp(string, ".ORG")){
                    //se for decimal ou outra base
                    if ((base == 10 && value>=0 && value <= 1023) || base != 10){
                        *current_ias_word = (int)value;
                        *current_position = kLeft;
                        return 2;
                    }else{
                        show_build_error("Parâmetro inválido para diretiva .ORG", line);
                    }
                }
                //se não for um número o primeiro parâmetro
            }else {
                
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
                    number_of_expected_words += validateDirective(string, first_argument, second_argument, &current_ias_word, &current_position, line_number);
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
#warning colocar linha do erro!
            show_build_error("Caracteres sobrando", line_number);
        }
        line_number++;
    }

    return dictionary;
}
