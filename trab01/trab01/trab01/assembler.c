#include "assembler.h"

//valida uma diretiva, e retorna o número de argumentos(incluindo a diretiva) esperada
int validateDirective(Dictionary* sets_dictionary, Node* word, int* current_ias_word, InstructionPosition* current_position, int line){
    char* string = word->data.word;
    //pega os argumentos da possível diretiva
    char* first_argument = word->next ? word->next->data.word : NULL;
    char* second_argument = first_argument ? (word->next->next ? word->next->next->data.word : NULL) : NULL;
    DictionaryNode* tmp;
    long value, value2;
    int base = 10;
    int flag_first_argument_valid = 0;
    //pode ser uma diretiva, e está no lugar correto.
    if (is_valid_directive(string)) {
        //Primeiro verificamos se é um set.
        if(!strcmp(string, ".SET")){
            if (is_valid_sym(first_argument, strlen(first_argument))){
                if(dictionary_get(sets_dictionary, first_argument)){
                    show_build_error("Múltiplas definições para uma constante", line);
                }
                if (second_argument && (str2int(&value2, second_argument,&base) == SUCCESS)) {
                    if (sets_dictionary) {
                        if (!dictionary_get(sets_dictionary, first_argument)) {
                            tmp = dictionary_put(sets_dictionary, first_argument, value2, kSetDirective);
                            tmp->base = base;
                        }else{
                            show_build_error("Múltiplas definições com .set", line);
                        }
                        
                    }
                    return 3;
                }
            }else{
                show_build_error("Caracteres inválidos no nome da constante", line);
            }
        }
        
        //todas possuem pelo menos um argumento
        tmp = dictionary_get(sets_dictionary, first_argument);
        if (first_argument) {
            //pegamos o primeiro argumento ou parseando um int, ou pegando do banco de sets
            if (str2int(&value, first_argument,&base) == SUCCESS) {
                flag_first_argument_valid = SUCCESS;
            }else if(tmp != NULL){
                flag_first_argument_valid = SUCCESS;
                value = tmp->location;
                base = tmp->base;
            }

            if (flag_first_argument_valid == SUCCESS) {
                
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
                }else if(!strcmp(string, ".WORD")){
                    if (*current_position == kLeft) {
                        //HEX | DEC(0:2^23-1) | LAB
                        if ((base == 10 && value <= (pow(2,32)-1) && value >= 0) || base != 10 || is_valid_sym(first_argument, strlen(first_argument))) {
                            (*current_ias_word)++;
                            return 2;
                        }else{
                            show_build_error("Número extrapola o valor máximo para .word!", line);
                        }
                    }else{
                        show_build_error("Não é possível colocar um valor desalinhado na memória!", line);
                    }
                }else if (!strcmp(string, ".WFILL")){
                    if (base == 10 && value <= 1023 && value >= 1) {
                        if (second_argument) {
                            //pega do banco de sets ou parseando o número fornecido
                            tmp = dictionary_get(sets_dictionary, second_argument);
                            if (str2int(&value2, second_argument,&base) == SUCCESS) {
                                flag_first_argument_valid = SUCCESS;
                            }else if(tmp != NULL){
                                flag_first_argument_valid = SUCCESS;
                                value2 = tmp->location;
                                base = tmp->base;
                            }
                            //se o segundo parametro é um número na base 10, confere o intervalo
                            if (flag_first_argument_valid == SUCCESS && base == 10 && (value2 > (pow(2,31)-1) ||value2 < -1*(pow(2,31)))){
                                show_build_error("Parâmetro fora dos limites", line);
                            }
                            //se conseguiu parsear, ou é um possível label
                            if (flag_first_argument_valid == SUCCESS || is_valid_sym(second_argument, strlen(second_argument))) {
                                //se é possível colocar uma palavra inteira aqui
                                if (*current_position == kLeft) {
                                    (*current_ias_word) += value;
                                }else{
                                    show_build_error("Não é possível colocar um vetor desalinhado na memória!", line);
                                }
                                return 3;
                            }

                        }else{
                            show_build_error("Especifique o valor para preencher o vetor!", line);
                        }
                        
                    }else{
                        show_build_error("Parâmetro inválido para diretiva .wfill!", line);
                    }
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



//Primeira etapa da montagem: Monta a tabela de rótulos e sets!
Dictionary* labels_dictionary(Node* file, Dictionary** set_constants){
    Dictionary* dictionary = malloc(sizeof(Dictionary));
    Node* line, * word;
    *set_constants = malloc(sizeof(Dictionary));
    int current_ias_word = 0, i, line_contains_label = 0, number_of_expected_words = 0;
    char* first_argument = NULL, *second_argument = NULL;
    InstructionPosition current_position = kLeft;
    size_t string_size;
    char * string;
    for (line = file; line != NULL; line = line->next){
        
        if (current_ias_word > IAS_MAX_MEM_WORDS){
            show_build_error("O arquivo ultrapassou o limite de memória do IAS",-1);
        }
        //contador de palavra
        i = 0;
        line_contains_label = 0;
        number_of_expected_words = 0;
        //agora vemos cada palavra da linha
        for (word = line->data.list; word != NULL; word = word->next) {
            //pega o primeiro e o segundo argumento de uma possível instrução atual.
            first_argument = word->next ? word->next->data.word : NULL;
            second_argument = first_argument ? (word->next->next ? word->next->next->data.word : NULL) : NULL;
            
            string = word->data.word;
            string_size = strlen(string);
            
            //se for um rotulo, e estiver em primeiro na linha
            if(is_valid_label(string, string_size)){
                if (i == 0) {
                    string[string_size-1] = '\0';
                    line_contains_label = 1;
                    dictionary_put(dictionary, string, current_ias_word, current_position);
                    number_of_expected_words++;
                }else{
                    show_build_error("O rótulo deve estar no começo da linha.\n",line->line_number);
                }
            }
            
            //se pode ser uma diretiva
            if (*string == '.') {
                if (i <= 1) {
                    number_of_expected_words += validateDirective(*set_constants, word, &current_ias_word, &current_position, line->line_number);
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
            show_build_error("Caracteres sobrando", line->line_number);
        }
    }
    
    return dictionary;
}


void assemble_file(Node* file, Dictionary* label_dictionary, Dictionary* set_constants, char* output_filename){
    FILE *output = fopen(output_filename, "w");
    Node* line, *word;
    if (output == NULL)
    {
        show_build_error("Não foi possível abrir o arquivo de saída", -1);
    }
    int current_ias_word = 0, base = 10;
    InstructionPosition current_position = kLeft;
    size_t string_size;
    char* string, *second_parameter_string;
    DictionaryNode* tmp;
    long int first_parameter, second_parameter;
    for (line = file; line != NULL; line = line->next){
        
        for (word = line->data.list; word != NULL; word = word->next) {
            
            string = word->data.word;
            string_size = strlen(string);
            if (dictionary_get(label_dictionary, string)) {
                continue;
            }
            
            /**
             *  Diretivas
             */
            if (!strcmp(string, ".WORD")) {
                if (current_position != kLeft) {
                    show_build_error("Não é possível colocar um valor desalinhado na memória!", line->line_number);
                }
                if(str2int(&first_parameter, word->next->data.word, &base) == SUCCESS){
                    print_word(output, current_ias_word, first_parameter);
                }else if((tmp = dictionary_get(set_constants, word->next->data.word)) != NULL){
                    //verifica as constantes
                    print_word(output, current_ias_word, tmp->location);
                }else if((tmp = dictionary_get(label_dictionary, word->next->data.word)) != NULL){
                    //verifica os labels
                    print_word(output, current_ias_word, tmp->location);
                }else{
                    show_build_error("Símbolo não definido", line->line_number);
                }
                current_ias_word++;
                break;
            }else if(!strcmp(string, ".WFILL")){
                //pega o primeiro parâmetro do número ou do set
                if(str2int(&first_parameter, word->next->data.word, &base) == SUCCESS){
                }else if((tmp = dictionary_get(set_constants, word->next->data.word)) != NULL){
                    first_parameter = tmp->location;
                }else{
                    show_build_error("Símbolo não definido", line->line_number);
                }


                second_parameter_string = word->next->next->data.word;

                //pega o segundo parâmetro do número ou do set ou de um label
                if(str2int(&second_parameter, second_parameter_string, &base) == SUCCESS){
                    print_vector(output, &current_ias_word, first_parameter, second_parameter);
                }else if((tmp = dictionary_get(set_constants, second_parameter_string)) != NULL){
                    //verifica as constantes
                    print_vector(output, &current_ias_word, first_parameter, tmp->location);
                }else if((tmp = dictionary_get(label_dictionary, second_parameter_string)) != NULL){
                    //verifica os labels
                    print_vector(output, &current_ias_word, first_parameter, tmp->location);
                }else{
                    show_build_error("Símbolo não definido", line->line_number);
                }
                break;
            }else if(!strcmp(string, ".SET")){
                //não faz nada, pois já foi setado as constantes
                break;
            }else if (string[0] == '.'){
                //preenche com zeros
                if ((!strcmp(string, ".ALIGN") || !strcmp(string, ".ORG"))&& current_position == kRight) {
                    fprintf(output, "00 000\n");
                }
                //valida a diretiva
                validateDirective(set_constants, word, &current_ias_word, &current_position, line->line_number);
            }
            
            /**
             *  Mnemônicos
             */
            if (is_valid_instruction(string)) {
                print_instruction(output, word, label_dictionary, set_constants, current_ias_word, current_position,line->line_number);
                current_ias_word += ((current_position + 1) / 2);
                current_position =  ((current_position + 1) % 2);
            }
            
        }
    }
    if (current_position == kRight) {
        fprintf(output, "00 000\n");
    }
    
    
}


