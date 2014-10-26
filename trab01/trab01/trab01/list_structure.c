#include "list_structure.h"
#include <string.h>

//cria uma lista
Node * create_list(){     // Função do tipo apontador para lista, 
    Node * new_list;
    new_list = (Node *) malloc( sizeof( Node ));   /*Aloca memória do tamanho de uma célula*/
    if(new_list == NULL) exit(0);    /* Se não alocar memória significa que não há memoria disponível, logo deve sair*/
    new_list->next = NULL;         /*Como esta deve ser a primeira função a ser executada, esta célula vazia aponta para NULL*/
    return new_list;
}

//insere um nó no fim da lista
void insert_at_end(Node **root, ListType type, void * data, int line_number){
    Node *novo;
    
    novo = (Node *) malloc(sizeof(Node));
    if(novo == NULL) exit(0);
	
	if(type == kListOfLists){
		novo->data.list = data;
        novo->line_number = line_number;
        novo->type = type;
	}else{
		novo->data.word = strdup((char*)data);
        novo->type = type;
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
//pega o último elemento da lista
Node* last_element(Node* root){ 
    Node *aux;
    aux = root;	
    while(aux->next != NULL){
        aux = aux->next;
    }
	return aux;
}
void free_list(Node* root){
    if (root == NULL) {
        return;
    }
    Node *aux1,*tmp1;
    aux1 = root;
    while(aux1->next != NULL){
        tmp1 = aux1->next;
        if (aux1->type == kListOfLists) {
            free_list(aux1->data.list);
        }else{
            free(aux1->data.word);
        }
        free(aux1);
        aux1 = tmp1;
    }
    free(aux1);
}
