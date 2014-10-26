#include "readASM.h"

Node* read_ASM_file(char* file_name){
    Node* root = NULL, *word_list, *aux = NULL;
    size_t nbytes_preffered = 0, bytes_read;//numero de bytes esperados/lidos
    int line_number = 1;
    char * file_line = NULL, *strtok_aux;
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
            line_number++;
            continue;
        }
        
        insert_at_end(&root, kListOfLists, NULL, line_number); //data é null pq setamos mais pra frente
        aux = aux != NULL ? aux->next : root; //avança o ponteiro da lista

        word_list = NULL;
        //quebra a linha do arquivo e salva em uma lista todas as palavras
        strtok_aux = strtok(file_line, " \t"); //considera espaços e tabs
        while (strtok_aux != NULL)
        {
            //se for um comentário, para de ler!
            if (strtok_aux[0] == '#') {
                break;
            }
            convertToUpperCase(strtok_aux);
            insert_at_end(&word_list, kListOfData, strtok_aux, -1);
            strtok_aux = strtok(NULL, " \t");
        }
        
        //agora seta a lista do nó adicionado anteriormente
        aux->data.list = word_list;
        line_number++;
        nbytes_preffered = 0;
        file_line = NULL;
    }
    //fecha o arquivo
    fclose(file);
    return root;
}