#include "dictionary_structure.h"


/* hash: form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

DictionaryNode *dictionary_get(Dictionary* dictionary, char *s)
{
    if (!s) {
        return NULL;
    }
    struct nlist *np;
    for (np = dictionary->hashtable[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

DictionaryNode *dictionary_put(Dictionary* dictionary, char *name, long location, InstructionPosition position)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = dictionary_get(dictionary, name)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = dictionary->hashtable[hashval];
        dictionary->hashtable[hashval] = np;
    }
    np->location = location;
    np->position = position;
    return np;
}
void free_dictionary(Dictionary* dictionary){
    int i;
    struct nlist* temp, *aux;
    for (i = 0; i < HASHSIZE; i++) {
        if (dictionary->hashtable[i] != NULL) {
            temp = dictionary->hashtable[i];
            while(temp->next != NULL){
                aux = temp->next;
                free(temp->name);
                free(temp);
                temp = aux;
            }
            free(temp);
        }
    }
    free(dictionary);
}
