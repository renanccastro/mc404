//
//  writeASM.c
//  trab01
//
//  Created by Renan Camargo de Castro on 18/10/14.
//  Copyright (c) 2014 TecSinapse. All rights reserved.
//

#include "writeASM.h"

void print_word(FILE* output, int current_ias_word, long int first_parameter){
    fprintf(output, "%03X %02lX %03lX %02lX %03lX\n", current_ias_word, (first_parameter & 0xff00000000)/0x0100000000, (first_parameter & 0x00fff00000)/0x0000100000, (first_parameter & 0x00000ff000)/0x0000001000, (first_parameter & 0xfff)/0x001);
}
void print_vector(FILE* output, int *current_ias_word, long int first_parameter, long int second_parameter){
    int i;
    for (i = 0; i < first_parameter; i++) {
        print_word(output, *current_ias_word, second_parameter);
        (*current_ias_word)++;
    }
}

void print_instruction(FILE* output, Node* line, Dictionary* labels, Dictionary* sets,int current_ias_word, InstructionPosition position, int line_number){
    char* string = line->data.word, *first_argument_string = line->next? line->next->data.word : NULL;
    size_t first_argument_string_length;
    long int first_argument;
    DictionaryNode* first = dictionary_get(labels, first_argument_string);
    int instruction_code = 0x01, base;
    
    if (position == kLeft) {
        fprintf(output, "%03X ", current_ias_word);
    }
    if (!strcmp(string, "LD")) {
        instruction_code = 0x01;
    }else if (!strcmp(string, "LD_N")){
        instruction_code = 0x02;
    }else if (!strcmp(string, "LD_ABS")){
        instruction_code = 0x03;
    }else if (!strcmp(string, "LD_MQ")){
        fprintf(output, "0A 000%c", position == kRight ? '\n' : ' ');
        return;
    }else if (!strcmp(string, "LD_MQMX")){
        instruction_code = 0x09;
    }else if (!strcmp(string, "ST")){
        instruction_code = 0x21;
    }else if (!strcmp(string, "JMP")){
        if (first) {
            if (first->position == kLeft) {
                instruction_code = 0x0D;
            }else{
                instruction_code = 0x0E;
            }
        }else{
            show_build_warning("JMP não especificou a posição! Use um label!", line_number);
            instruction_code = 0x0D;
        }
        
    }else if (!strcmp(string, "JGE")){
        if (first) {
            if (first->position == kLeft) {
                instruction_code = 0x0F;
            }else{
                instruction_code = 0x10;
            }
        }else{
            show_build_warning("JGE não especificou a posição! Use um label!", line_number);
            instruction_code = 0x0F;
        }

    }else if (!strcmp(string, "ADD")){
        instruction_code = 0x05;
    }else if (!strcmp(string, "ADD_ABS")){
        instruction_code = 0x07;
    }else if (!strcmp(string, "SUB")){
        instruction_code = 0x06;
    }else if (!strcmp(string, "SUB_ABS")){
        instruction_code = 0x08;
    }else if (!strcmp(string, "MUL")){
        instruction_code = 0x0B;
    }else if (!strcmp(string, "DIV")){
        instruction_code = 0x0C;
    }else if (!strcmp(string, "LSH")){
        fprintf(output, "14 000%c", position == kRight ? '\n' : ' ');
                return;
    }else if (!strcmp(string, "RSH")){
        fprintf(output, "15 000%c", position == kRight ? '\n' : ' ');
                return;
    }else if (!strcmp(string, "ST_ADDR")){
        if (first) {
            if (first->position == kLeft) {
                instruction_code = 0x12;
            }else{
                instruction_code = 0x13;
            }
        }else{
            show_build_warning("ST_ADDR não especificou a posição! Use um label!", line_number);
            instruction_code = 0x12;
        }
    }
    //aqui temos o código da instrução em instruction_code
    //verificamos se é um label, ou caso contrário se é um endereço de memória
    if (first) {
        fprintf(output, "%02X %03lX%c", instruction_code, first->location, position == kRight ? '\n' : ' ');
    }else{
        if (first_argument_string && (first_argument_string_length = strlen(first_argument_string)) > 3 && first_argument_string[0] == 'M') {
            //tira os parenteses e o M
            first_argument_string[first_argument_string_length-1] = '\0';
            first_argument_string += 2;
            
            //se for um número válido, printa!
            if(str2int(&first_argument, first_argument_string, &base) == SUCCESS){
                if (first_argument <= IAS_MAX_MEM_WORDS) {
                    fprintf(output, "%02X %03lX%c", instruction_code, first_argument & 0xFFF, position == kRight ? '\n' : ' ');
                }else{
                    show_build_error("Endereço de instrução inconsistente", line_number);
                }
            //Se não, procura no dicionário de constantes!
            }else if((first = dictionary_get(sets, first_argument_string)) != NULL){
                if (first->location > IAS_MAX_MEM_WORDS) {
                    show_build_warning("Tentativa de passar um valor maior que o número de palavras no IAS para uma instrução. Possível erro",line_number);
                }
                fprintf(output, "%02X %03lX%c", instruction_code, first->location & 0xFFF, position == kRight ? '\n' : ' ');
            }else{
                show_build_error("Endereço de instrução inconsistente", line_number);
            }
        }else{
            show_build_error("Label não encontrado", line_number);
        }
    }
}