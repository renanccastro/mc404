typedef struct Node_st{
	union{
		struct Node_st* list;
		char * word;
	} data;
    struct Node_st *next;
}Node;

typedef enum ListType{
	kListOfLists,
	kListOfData
 } ListType;


 Node * create_list();
 void insert_at_end(Node **root, ListType type, void * data);
 Node* last_element(Node* root);