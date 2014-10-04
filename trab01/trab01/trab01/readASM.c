#include "readASM.h"

Node* read_ASM_file(char* file_name){
    Node* list_of_lists = NULL, *word_list, *aux = NULL;
    size_t nbytes = BUFFER_SIZE, bytes_read, string_length;//numero de bytes esperados/lidos
    char * file_line, *strtok_aux;
    FILE *file;
    
    file = fopen(file_name , "r");
    
    if(file == NULL) {
        printf("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    // Lê uma linha (inclusive com o '\n')
    while ((bytes_read = getline (&file_line, &nbytes, file)) != -1){
        //tira o \n da string
        chomp_string(file_line);
        if (!strcmp(file_line,"")) {
            continue;
        }
        
        aux = aux ? aux->next : NULL; //avança o ponteiro da lista
        insert_at_end(&list_of_lists, kListOfLists, NULL);
        if(aux == NULL){
            aux = list_of_lists;
        }
        word_list = NULL;
        
        //quebra a linha do arquivo e salva em uma lista todas as palavras
        strtok_aux = strtok(file_line, " \t"); //considera espaços e tabs
        while (strtok_aux != NULL)
        {
            insert_at_end(&word_list, kListOfData, strtok_aux);
            strtok_aux = strtok(NULL, " \t");
        }
        aux->data.list = word_list;
        printf("First element data: %s",aux->data.list->data.word);
    }
    //fecha o arquivo
    fclose(file);
    return list_of_lists;
}

void chomp_string(char* string){
    //tira o \n do final
    size_t string_length = strlen(string);
    if (string[string_length-1] == '\n') {
        string[string_length-1] = '\0';
    }
}

