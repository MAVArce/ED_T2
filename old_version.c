#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
    char word[30];
    struct node* next;
    struct node* prev;
};

typedef struct node WORD;

WORD* insert(){
    WORD* new;
    new = (WORD*) malloc(sizeof(WORD));
    new->prev=NULL;
    new->next=NULL;

    return new;
}

int check(char digit){
    if((digit>='A' && digit <='Z') || (digit>='a' && digit<='z') || (digit>='0' && digit <='9')){
        return 1;
    }else if(digit=='\n'){
        return 2;
    }else{
        return 0;
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

void check_char(WORD** cursor, char digit, int* cont){
    WORD* aux;

    if(check(digit)==1 && (*cont==0 || (*cont!=0 && check((*cursor)->word[(*cont)-1])==1))){
        (*cursor)->word[*cont]=digit;
        (*cont)++;
        return;
    }

    (*cursor)->word[*cont]='\0';
    (*cursor)->next=insert();
    aux=*cursor;
    *cursor=(*cursor)->next;
    (*cursor)->prev=aux;
    *cont=0;

    if(digit!=' '){
        (*cursor)->word[*cont]=digit;
        (*cont)++; 
    }
}

void read_file(WORD** cursor, char archive[]){
    FILE* fd;
    char letter;
    int cont = 0;

    fd = fopen(archive, "rt");

    while(!feof(fd)){
        letter=getc(fd);
        check_char(cursor, letter, &cont);
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
    WORD* aux;
    int cont = 0;
    int check = 0;
    char text[30];
    
    fgets(text, 30, stdin);
    end_str(text);

    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");

    for(aux=cursor; aux->prev!=NULL; aux=aux->prev)
        cont++;
    
    aux=cursor;
    
    while(strcmp(aux->word, text)!=0){
        aux=aux->next;
        cont++;
        if(aux==NULL){
            check=1;
            break;
        }
    }

    if(check==0)
        printf("%d\n", cont);
}

void print(WORD* cursor){
    int cont = 0;
    WORD* atual = cursor;


    begin(&cursor);

    while(cursor!=NULL){
        if(cursor->word[cont]!='\0'){
            if(cursor == atual)
                printf("|");
            printf("%c", cursor->word[cont]);
            if(cursor == atual)
                printf("|");
            cont++;
        }else{
            if((cursor->next)!=NULL && cursor->word[0]!='\n' && check((cursor->next)->word[0])==1){
                printf(" ");
            }
            cont = 0;
            cursor=cursor->next;
        }
    }
    printf("\n"); //
}

void remove_node(WORD** cursor){
    WORD* aux;
    
    if((*cursor)->next==NULL){
        *cursor=(*cursor)->prev;
        free((*cursor)->next);
        (*cursor)->next=NULL;
    }else{
        aux=*cursor;
        *cursor=(*cursor)->next;
        (*cursor)->prev=aux->prev;
        aux->prev->next=*cursor;
        free(aux);
    }
}

void replace(WORD** cursor){
    char text[30];
    
    fgets(text, 30, stdin);
    end_str(text);
    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");
    
    strcpy((*cursor)->word, text);
}

void insert_before(WORD** cursor){
    WORD* aux;
    char text[30];

    fgets(text, 30, stdin);
    end_str(text);

    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");
    
    aux=insert();
    strcpy(aux->word, text);

    if((*cursor)->prev==NULL){
        (*cursor)->prev=aux;
        aux->next=*cursor;
        aux->prev=NULL;    
    }else{
        aux->prev=(*cursor)->prev;
        aux->next=*cursor;
        ((*cursor)->prev)->next=aux;
        (*cursor)->prev=aux;
    }
}

void insert_after(WORD** cursor){
    WORD* aux;
    char text[30];

    fgets(text, 30, stdin);
    end_str(text);    

    if(strcmp(text, "<ENTER>")==0)
        strcpy(text, "\n\0");
    
    aux=insert();
    strcpy(aux->word, text);

    if((*cursor)->next==NULL){
        (*cursor)->next=aux;
        aux->prev=*cursor;
        aux->next=NULL;
    }else{
        aux->prev=*cursor;
        aux->next=(*cursor)->next;
        (*cursor)->next->prev=aux;
        (*cursor)->next=aux;
    }
}

void write_file(WORD* cursor, char archive[]){
    FILE* fd;
    fd = fopen(archive, "wt");
    int cont = 0;

    while(cursor!=NULL){
        if(cursor->word[cont]!='\0'){
            fprintf(fd, "%c", cursor->word[cont]);
            cont++;
        }else{
            if((cursor->next)!=NULL && cursor->word[0]!='\n' && check((cursor->next)->word[0])==1){
                fprintf(fd, " ");
            }
            cont = 0;
            cursor=cursor->next;
        }
    }
    fclose(fd);
}

void free_all(WORD** cursor){
    WORD* aux;

    while(*cursor!=NULL){
        aux=*cursor;
        *cursor=(*cursor)->next;
        free(aux);
    }
}

void menu(WORD** cursor){
    char mode;
    int t = 1;

    begin(cursor);
    while(t){
        print(*cursor);
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

    cursor=insert();

    archive[0]=getchar();
    getchar();
    archive[1]='\0';
    strcat(archive, ".ext\0");
    
    read_file(&cursor, archive);
    menu(&cursor);
    begin(&cursor);
    write_file(cursor, archive);
    free_all(&cursor);

    return 0;
}