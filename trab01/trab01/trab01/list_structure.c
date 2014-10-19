#include "list_structure.h"
#include <string.h>

Node * create_list(){     // Função do tipo apontador para lista, 
    Node * new_list;
    new_list = (Node *) malloc( sizeof( Node ));   /*Aloca memória do tamanho de uma célula*/
    if(new_list == NULL) exit(0);    /* Se não alocar memória significa que não há memoria disponível, logo deve sair*/
    new_list->next = NULL;         /*Como esta deve ser a primeira função a ser executada, esta célula vazia aponta para NULL*/
    return new_list;
}

void insert_at_end(Node **root, ListType type, void * data, int line_number){
    Node *novo;
    char* string_buffer;
    
    novo = (Node *) malloc(sizeof(Node));
    if(novo == NULL) exit(0);
	
	if(type == kListOfLists){
		novo->data.list = data;
        novo->line_number = line_number;
	}else{
        string_buffer = malloc(sizeof(char)*strlen((char*)data));
        strcpy(string_buffer, data);
		novo->data.word = string_buffer;
	}
    novo->next = NULL;
 
    if((*root) == NULL){
        *root = novo;
    }else{
        Node *aux;
        aux = *root;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = novo;
    }
}
Node* last_element(Node* root){ 
    Node *aux;
    aux = root;	
    while(aux->next != NULL){
        aux = aux->next;
    }
	return aux;
}
