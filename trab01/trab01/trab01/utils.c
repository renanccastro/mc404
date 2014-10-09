#include "utils.h"

void show_build_error(char* error, int line){
    if (line < 0) {
        printf("ERRO: %s\n",error);
    }else{
        printf("ERRO: %s, na linha %d\n",error,line);
    }
    exit(EXIT_FAILURE);

}
void show_build_warning(char* warning){
    printf("WARNING: %s\n",warning);
}