#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXT_SIZE 3000
#define TEG_SIZE 30
#define STEP 10

typedef char type;

typedef struct Stack{
    struct StackEl* top;
    int size;
} Stack;

typedef struct StackEl{
    type* value;
    struct StackEl* next;
} StackEl;

Stack* initStack(){
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    memset(stack, 0, sizeof(Stack));
    return stack;
}

void push(Stack* stack, type* data){
    StackEl* new_element = (StackEl*)malloc(sizeof(StackEl));
    new_element->value = data;
    new_element->next = stack->top;
    stack->top = new_element;
    stack->size++;
}

type* pop(Stack* stack){
    type* result = stack->top->value;
    free(stack->top);
    stack->top = stack->top->next;
    stack->size--;
    return result;
}

type* top(Stack* stack){
    return stack->top->value;
}
int isEmpty(Stack* stack){
    return !stack->size;
}

int count(Stack* stack){
    return stack->size;
}


void delStack(Stack* stack){
    while(stack->top != NULL){
        StackEl* temp = stack->top->next;
        free(stack->top);
        stack->top = temp;
    }
}

int main(){
    Stack* stack = initStack();
    int error = 0;
    int i = 0, j = 0;
    int buf_size;
    type* str = malloc((TEXT_SIZE+2)*sizeof(type));
    fgets(str, TEXT_SIZE+2, stdin);
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') str[len-1] = '\0';
    puts(str);
    type* ptr = strtok(str, ">");

    if(ptr == NULL){
        printf("wrong\n");
        free(str);
        return 0;
    }

    while(ptr != NULL){
        if(ptr[0] != '<'){
            if((ptr = strstr(ptr, "<")) == NULL){
                error = 1;
                break;
            }
	    }

        ptr = ptr+1*sizeof(type);
        if(ptr[0] != '/'){
            if(strcmp(ptr, "br") != 0 && strcmp(ptr, "hr") != 0){
                push(stack, ptr);
            }
        }
        else{
            if(isEmpty(stack)){
                error = 1;
                break;
            }
            if(strcmp(ptr, top(stack))){
                pop(stack);
            }
            else{
                error = 1;
                break;
            }
        }
        ptr = strtok(NULL, ">");
    }
    if(error || !isEmpty(stack)) printf("wrong\n");
    else printf("correct\n");
    delStack(stack);
    free(str);
    return 0;
}
