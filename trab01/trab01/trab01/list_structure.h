#include <stdlib.h>
#ifndef LIST_STRUCTURE_147775
#define LIST_STRUCTURE_147775

//Tipo da lista
typedef enum ListType{
    kListOfLists,
    kListOfData
} ListType;


//Um nó da lista
typedef struct Node_st{
	union{
		struct Node_st* list;
		char * word;
	} data;
    struct Node_st *next;
    int line_number;
    ListType type;
}Node;



 Node * create_list();
 void insert_at_end(Node **root, ListType type, void * data, int line_number);
 Node* last_element(Node* root);

#endif