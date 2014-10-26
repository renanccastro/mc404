#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readASM.h"
#include "assembler.h"
#include "dictionary_structure.h"

int main (int argc, char *argv[])
{
    Dictionary* set_constants;
    if (argc < 2) {
        printf("Forma de uso: %s source.in output.out\n", argv[0]);
        return (1);
    }

    char* output_file_name;
    char* file_name = argv[1];
    
    //se o usuário forneceu um segundo parâmetro usa
    //caso contrário adiciona .hex no final do primeiro
    if (argc > 2) {
        output_file_name = strdup(argv[2]);
    }else{
        output_file_name = malloc(sizeof(char) * (strlen(file_name)  + 4));
        sprintf(output_file_name, "%s.hex", file_name);
    }
    
    Node* main_list = read_ASM_file(file_name);
    Dictionary* labels_dic = labels_dictionary(main_list, &set_constants);
    assemble_file(main_list, labels_dic, set_constants, output_file_name);
    //libera a memória
    free_dictionary(labels_dic);
    free_dictionary(set_constants);
    free_list(main_list);
    
    return 0;
}
