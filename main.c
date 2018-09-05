#include <stdio.h>
#include <stdlib.h>

struct caracter{
    char* first;
    struct caracter* next;
    struct caracter* prev;
}

typedef struct caracter WORD;

void previous(WORD** cursor){
    if((*cursor)->prev!=NULL)
        *cursor=(*cursor)->prev;
}

void next(WORD** cursor){
    if((*cursor)->next!=NULL)
        *cursor=(*cursor)->next;
}

void begin(WORD** cursor){
    while((*cursor)->prev!=NULL)
        *cursor=(*cursor)->prev;
}

void end(WORD** cursor){
    while((*cursor)->next!=NULL)
        *cursor=(*cursor)->next;
}

void move(WORD** cursor){
    int 
}

void menu(){
    WORD* cursor;
    char mode;

    mode=getchar();
    getchar();

    switch(mode){
        case 'b':
            begin(&cursor);
            break;
        
        case 'e':
            end(&cursor);
            break;

        case 'p':
            previous(&cursor);
            break;

        case 'n':
            next(&cursor);
            break;

        case 'g':

            break;
    }
}

int main(int argc, char const *argv[]){
    
    return 0;
}
