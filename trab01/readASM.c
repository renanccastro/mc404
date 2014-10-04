#include <stdio.h>
#include <string.h>
#include "list_structure.h"
#define BUFFER_SIZE 255

// apenas para testes!
int main (int argc, char const *argv[])
{
	Node* list_of_lists, *word_list, *aux;
    char buffer[BUFFER_SIZE];
	char * file_line, *strtok_aux;
    FILE *file;
	
    file = fopen("file.txt" , "r");

    if(file == NULL) {
		printf("Erro ao abrir arquivo");
       return(-1);
    }
	//enquanto o arquivo não terminou
	while (!feof(file)){
		aux = aux ? aux->next : NULL; //avança o ponteiro da lista
  		// Lê uma linha (inclusive com o '\n')
        file_line = fgets(buffer, BUFFER_SIZE, file);  // o 'fgets' lê até BUFFER_SIZE caracteres ou até o '\n'
        if (file_line){  // Se foi possível ler
			insert_at_end(&list_of_lists, kListOfLists, NULL);
			if(aux == NULL){
				aux = list_of_lists;
			}
			word_list = NULL;
			//quebra a linha do fileuivo e salva em uma lista todas as palavras
			strtok_aux = strtok(file_line, " \t"); //considera espaços e tabs
		    while (strtok_aux != NULL)
		    {
				insert_at_end(&word_list, kListOfData, strtok_aux);
				strtok_aux = strtok(NULL, " \t");
		    }
			aux->data.list = word_list;
			printf("First element data: %s",aux->data.list->data.word);
  		}
    }
	//fecha o arquivo
    fclose(file);
}

