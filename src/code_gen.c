#include "code_gen.h"
#include <stdio.h>

void create_function(){

}

void generate_header(){
    printf(".IFcode22");
    printf("JUMP $$main");
    printf("LABEL $$main");
}