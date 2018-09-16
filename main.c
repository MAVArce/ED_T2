#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMON 1
#define SPECIAL 0
#define ENTER 2

struct character{
    char letter;
    struct character* next;
    struct character* prev;
};

typedef struct character CHARACTER;

struct node{
    CHARACTER* first;
    struct node* next;
    struct node* prev;
};

typedef struct node WORD;

CHARACTER* insert_character(char a){
    CHARACTER* new;
    
    new=(CHARACTER*) malloc(sizeof(CHARACTER));
    new->letter=a;
    new->next=NULL;
    new->prev=NULL;

    return new;
}

WORD* insert_word(){
    WORD* new;

    new = (WORD*) malloc(sizeof(WORD));
    new->first=NULL;
    new->prev=NULL;
    new->next=NULL;

    return new;
}

int check(char digit){
    if((digit>='A' && digit <='Z') || (digit>='a' && digit<='z') || (digit>='0' && digit <='9')){
        return COMMON;
    }else if(digit=='\n'){
        return ENTER;
    }else{
        return SPECIAL;
    }
}

void end_str(char a[]){
    int aux;

    for(aux=0; a[aux]!=' ' && a[aux]!='\n'; aux++);
    a[aux]='\0';
}

void previous(WORD** cursor){
    if((*cursor)->prev!=NULL)
        *cursor=(*cursor)->prev;
}

void next(WORD** cursor){
    if((*cursor)->next!=NULL)
        *cursor=(*cursor)->next;
}

void end(WORD** cursor){
    while((*cursor)->next!=NULL)
        next(cursor);
}

void begin(WORD** cursor){
    while((*cursor)->prev!=NULL)
        previous(cursor);
}

void check_char(WORD** cursor, char digit){
    WORD* pntr_w;
    CHARACTER* new_c = NULL;
    CHARACTER* pntr_c = NULL;

    if(digit!=' ')
        new_c=insert_character(digit);

    if((*cursor)->first!=NULL){
        for(pntr_c=(*cursor)->first; pntr_c->next!=NULL; pntr_c=pntr_c->next);
    }else{
        (*cursor)->first=new_c;
        return;
    }
    
    if(check(digit)==COMMON && check(pntr_c->letter)==COMMON){
        pntr_c->next=new_c;
        return;
    }

    (*cursor)->next=insert_word();
    pntr_w=*cursor;
    *cursor=(*cursor)->next;
    (*cursor)->prev=pntr_w;

    if(digit!=' '){
        (*cursor)->first=new_c;
    }
}

void read_file(WORD** cursor, char archive[]){
    FILE* fd;
    char letter;

    fd = fopen(archive, "rt");

    while(!feof(fd)){
        letter=getc(fd);
        check_char(cursor, letter);
    }
    
    *cursor=(*cursor)->prev;
    free((*cursor)->next);
    (*cursor)->next=NULL;

    fclose(fd);
}

void move(WORD** cursor){
    int number;
    scanf("%d", &number);
    getchar();

    if(number<0){
        while(number!=0){
            previous(cursor);
            number++;
        }
    }else if(number>0){
        while(number!=0){
            next(cursor);
            number--;
        }
    }
}

void find_word(WORD* cursor){
    CHARACTER* pntr_c;
    WORD* pntr_w;
    char text[30];
    int cont = 0;
    int check = 0;
    int aux;
    
    fgets(text, 30, stdin);
    end_str(text);
    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");

    for(pntr_w=cursor; pntr_w->prev!=NULL; pntr_w=pntr_w->prev)
        cont++;
    
    for(pntr_w=cursor; pntr_w!=NULL; pntr_w=pntr_w->next){
        for(aux=0, pntr_c=pntr_w->first; text[aux]!='\0' && pntr_c!=NULL; aux++, pntr_c=pntr_c->next){
            if(text[aux]!=pntr_c->letter)
                break;
        }
        if(text[aux]=='\0' && pntr_c==NULL){
            check = 1;
            break;
        }
        cont++;
    }

    if(check==1)
        printf("%d\n", cont);
}

void print(WORD* cursor){
    CHARACTER* aux_c;

    aux_c=cursor->first;

    while(cursor!=NULL){
        if(aux_c!=NULL){
            printf("%c", aux_c->letter);
            aux_c=aux_c->next;
        }else{
            if((cursor->next)!=NULL && cursor->first->letter!='\n' && check((cursor->next)->first->letter)==COMMON){
                printf(" ");
            }
            cursor=cursor->next;
            if(cursor!=NULL)
                aux_c=cursor->first;
        }
    }
    printf("\n"); //
}

void write_word(WORD** pntr_w, char text[]){
    CHARACTER* pntr_c;
    int aux;

    (*pntr_w)->first=insert_character(text[0]);
    pntr_c=(*pntr_w)->first;

    for(aux=1; text[aux]!='\0'; aux++){
        pntr_c->next=insert_character(text[aux]);
        pntr_c->next->prev=pntr_c;
        pntr_c=pntr_c->next;
    }
}

void free_word(WORD** cursor){
    CHARACTER* pntr_c;
    CHARACTER* aux;

    pntr_c=(*cursor)->first;

    while(pntr_c->next!=NULL){
        aux=pntr_c;
        pntr_c=pntr_c->next;
        free(aux);
    }
    free(pntr_c);

    (*cursor)->first=NULL;
}

void remove_node(WORD** cursor){
    WORD* pntr;

    free_word(cursor);
    
    if((*cursor)->next==NULL){
        *cursor=(*cursor)->prev;
        free((*cursor)->next);
        (*cursor)->next=NULL;
    }else{
        pntr=*cursor;
        *cursor=(*cursor)->next;
        (*cursor)->prev=pntr->prev;
        pntr->prev->next=*cursor;
        free(pntr);
    }
}

void replace(WORD** cursor){
    char text[30];
    
    fgets(text, 30, stdin);
    end_str(text);
    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");

    free_word(cursor);
    write_word(cursor, text);    
}

void insert_before(WORD** cursor){
    WORD* pntr_w;
    char text[30];

    fgets(text, 30, stdin);
    end_str(text);

    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");
    
    pntr_w=insert_word();
    write_word(&pntr_w, text);

    if((*cursor)->prev==NULL){
        (*cursor)->prev=pntr_w;
        pntr_w->next=*cursor;
        pntr_w->prev=NULL;    
    }else{
        pntr_w->prev=(*cursor)->prev;
        pntr_w->next=*cursor;
        ((*cursor)->prev)->next=pntr_w;
        (*cursor)->prev=pntr_w;
    }
}

void insert_after(WORD** cursor){
    WORD* pntr_w;
    char text[30];

    fgets(text, 30, stdin);
    end_str(text);    

    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");
    
    pntr_w=insert_word();
    write_word(&pntr_w, text);

    if((*cursor)->next==NULL){
        (*cursor)->next=pntr_w;
        pntr_w->prev=*cursor;
        pntr_w->next=NULL;
    }else{
        pntr_w->prev=*cursor;
        pntr_w->next=(*cursor)->next;
        (*cursor)->next->prev=pntr_w;
        (*cursor)->next=pntr_w;
    }
}

void free_all(WORD** cursor){
    WORD* aux;

    while(*cursor!=NULL){
        aux=*cursor;
        *cursor=(*cursor)->next;
        free_word(&aux);
        free(aux);
    }
}

void menu(WORD** cursor){
    char mode;
    int t = 1;

    begin(cursor);
    while(t){
        //print(*cursor);
        mode=getchar();
        getchar();
        switch(mode){
            case 'i':
                insert_before(cursor);
                break;

            case 'a':
                insert_after(cursor);
                break;

            case 'r':
                replace(cursor);
                break;

            case 'f':
                find_word(*cursor);
                break;

            case 'd':
                remove_node(cursor);
                break;

            case 'b':
                begin(cursor);
                break;
        
            case 'e':
                end(cursor);
                break;

            case 'p':
                previous(cursor);
                break;

            case 'n':
                next(cursor);
                break;

            case 'g':
                move(cursor);
                break;
            
            case 's':
                begin(cursor);
                print(*cursor);
                t = 0;
                break;
        }   
    }
}

int main(int argc, char const *argv[]){
    WORD* cursor;
    char archive[6];

    cursor=insert_word();

    archive[0]=getchar();
    while(getchar()!='\n');
    archive[1]='\0';
    strcat(archive, ".ext\0");
    
    read_file(&cursor, archive);
    menu(&cursor);
    begin(&cursor);
    free_all(&cursor);

    return 0;
}