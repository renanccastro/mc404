#include "utils.h"

void show_build_error(char* error, int line){
    if (line < 0) {
        printf("ERRO: %s\n",error);
    }else{
        printf("ERRO: %s, na linha %d\n",error,line);
    }
    exit(EXIT_FAILURE);

}
void show_build_warning(char* warning, int line){
    if (line < 0) {
        printf("WARNING: %s\n",warning);
    }else{
        printf("WARNING: %s, na linha %d\n",warning,line);
    }
}

//tira o \n no final da string
void chomp_string(char* string){
    //tira o \n do final
    size_t string_length = strlen(string);
    if (string[string_length-1] == '\n') {
        string[string_length-1] = '\0';
    }
}
void convertToUpperCase(char *sPtr)
{
    while(*sPtr != '\0'){
        *sPtr = toupper((unsigned char)*sPtr);
        sPtr++;
    }
}

int match_regexp(char* string, char* regexp){
    regex_t regex;
    int reti;
    reti = regcomp(&regex, regexp, REG_EXTENDED);
    reti = regexec(&regex, string, 0, NULL, 0);
    
    return !reti;
}
//Retorna 0/1 se a string passada é um rótulo
int is_valid_label(char* string, size_t length){
    return match_regexp(string, "^[A-Z_][A-Z0-9_]*:$");
}
int is_valid_sym(char* string, size_t length){
    return match_regexp(string, "^[A-Z_]([A-Z_0-9])*$");
}

int is_valid_instruction(char* string){
    //se for uma dos mnemonicos do ias
    return(!strcmp(string, "LD")       || !strcmp(string, "LD_N")    ||
           !strcmp(string, "LD_ABS")   || !strcmp(string, "LD_MQ")   ||
           !strcmp(string, "LD_MQMX")  || !strcmp(string, "ST")      ||
           !strcmp(string, "JMP")      || !strcmp(string, "JGE")     ||
           !strcmp(string, "ADD")      || !strcmp(string, "ADD_ABS") ||
           !strcmp(string, "SUB")      || !strcmp(string, "SUB_ABS") ||
           !strcmp(string, "MUL")      || !strcmp(string, "DIV")     ||
           !strcmp(string, "LSH")      || !strcmp(string, "RSH")     ||
           !strcmp(string, "ST_ADDR") );
}

STR2INT_ERROR str2int (long *i, char const *s, int *base)
{
    char *end;
    *base = 10;
    long  l;
    //primeiro, descobrimos a base
    size_t strlength = strlen(s);
    if (strlength > 2 && s[0] == '0') {
        switch (s[1]) {
            case 'X':
                *base = 16;
                break;
            case 'B':
                *base = 2;
                break;
            case 'O':
                *base = 8;
                break;
        }
        s = s+2;
    }
    
    
    errno = 0;
    //depois convertemos com a base obtida!
    l = strtol(s, &end, *base);
    if (errno == ERANGE) {
        return RANGE_ERROR;
    }
    if (*s == '\0' || *end != '\0') {
        return INCONVERTIBLE;
    }
    *i = l;
    return SUCCESS;
}
int is_valid_directive(char* string){
    return(!strcmp(string, ".ORG")       || !strcmp(string, ".ALIGN")    ||
           !strcmp(string, ".WFILL")   || !strcmp(string, ".WORD")   ||
           !strcmp(string, ".SET"));
    
}
