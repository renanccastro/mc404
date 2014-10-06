#include "readASM.h"

Node* read_ASM_file(char* file_name){
    Node* root = NULL, *word_list, *aux = NULL;
    size_t nbytes_preffered = BUFFER_SIZE, bytes_read;//numero de bytes esperados/lidos
    char * file_line, *strtok_aux;
    FILE *file;
    
    file = fopen(file_name , "r");
    
    if(file == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(EXIT_FAILURE);
    }

    // Lê uma linha (inclusive com o '\n')
    while ((bytes_read = getline (&file_line, &nbytes_preffered, file)) != -1){
        //tira o \n da string
        chomp_string(file_line);
        //se é uma linha vazia, não devemos adicionar a lista
        if (!strcmp(file_line,"")) {
            continue;
        }
        
        insert_at_end(&root, kListOfLists, NULL); //data é null pq setamos mais pra frente
        aux = aux != NULL ? aux->next : root; //avança o ponteiro da lista

        word_list = NULL;
        //quebra a linha do arquivo e salva em uma lista todas as palavras
        strtok_aux = strtok(file_line, " \t"); //considera espaços e tabs
        while (strtok_aux != NULL)
        {
            convertToUpperCase(strtok_aux);
            insert_at_end(&word_list, kListOfData, strtok_aux);
            strtok_aux = strtok(NULL, " \t");
        }
        
        //agora seta a lista do nó adicionado anteriormente
        aux->data.list = word_list;
        printf("First element data: %s\n",root->data.list->data.word);
    }
    //fecha o arquivo
    fclose(file);
    return root;
}

//tira o \n no final da string
void chomp_string(char* string){
    //tira o \n do final
    size_t string_length = strlen(string);
    if (string[string_length-1] == '\n') {
        string[string_length-1] = '\0';
    }
}
void convertToUpperCase(char *sPtr)
{
    while(*sPtr != '\0')
        *sPtr = toupper((unsigned char)*sPtr);
}

//Retorna 0/1 se a string passada é um rótulo
int is_valid_label(char* string, size_t length){
    char* substring;
    if (length > 0) {
        substring = strstr(string, ":");
        //se contiver ':'
        if (substring == NULL) {
            return 0;
        }else{
            //e se ele estiver no final
            if (strlen(substring) == 1) {
                //se o primeiro caracter for alfabetico
                return isalpha(string[0]);
            }else{
                return 0;
            }
        }
    }else{
        return 0;
    }
}
int is_valid_instruction(char* string){
    //se for uma dos mnemonicos do ias
    return(!strcmp(string, "LD")       || !strcmp(string, "LD_N")    ||
           !strcmp(string, "LD_ABS")   || !strcmp(string, "LD_MQ")   ||
           !strcmp(string, "LD_MQMX")  || !strcmp(string, "ST")      ||
           !strcmp(string, "JMP")      || !strcmp(string, "JGE")     ||
           !strcmp(string, "ADD")      || !strcmp(string, "ADD_ABS") ||
           !strcmp(string, "SUB")      || !strcmp(string, "SUB_ABS") ||
           !strcmp(string, "MUL")      || !strcmp(string, "DIV")     ||
           !strcmp(string, "LSH")      || !strcmp(string, "RSH")     ||
           !strcmp(string, "ST_ADDR") );
}

int instruction_value(char* string, size_t length){
    //TODO:
    return 0;
}

STR2INT_ERROR str2int (long *i, char const *s)
{
    char *end;
    int base = 10;
    long  l;
    //primeiro, descobrimos a base
    size_t strlength = strlen(s);
    if (strlength > 2 && s[0] == '0') {
        switch (s[1]) {
            case 'x':
                base = 16;
                break;
            case 'b':
                base = 2;
                break;
            case 'o':
                base = 8;
                break;
            default:
                base = 10;
                break;
        }
        s = s+2;
    }
    
    
    errno = 0;
    //depois convertemos com a base obtida!
    l = strtol(s, &end, base);
    if (errno == ERANGE) {
        return RANGE_ERROR;
    }
    if (*s == '\0' || *end != '\0') {
        return INCONVERTIBLE;
    }
    *i = l;
    return SUCCESS;
}

