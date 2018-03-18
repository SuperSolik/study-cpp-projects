#include <stdio.h>
#include <stdlib.h>


#define START_SIZE 5
#define STEP 1

typedef int type;

typedef struct Queue{
    type* data;
    int begin;
    int end;
    int len;
    int capacity;//how much elements can be in queue without overwriting
}Queue;

void initQ(Queue* q);
void enq(Queue* q, type value);
type deq(Queue* q);
int isFull(Queue* q);
int isEmpty(Queue* q);
void delQ(Queue** q);

void initQ(Queue* q){
    q->capacity = START_SIZE;
    q->data = calloc(q->capacity, sizeof(type));
    q->begin = 0;
    q->end = 0;
    q->len = 0;
}

void enq(Queue* q, type value){
    if(isFull(q)){
        q->capacity += STEP;
        q->data = realloc(q->data, q->capacity*sizeof(type));
        q->end = q->len;
    }
    q->data[q->end] = value;
    q->end = (q->end+1)%q->capacity;
    printf("end:[%d]\n", q->end);
    q->len++;
}

type deq(Queue* q){
    if(isEmpty(q)){
        printf("Q is empty, next value is not an element, it's \n");
        return -1;
    }
    type temp = q->data[q->begin];
    q->begin = (q->begin+1)%q->capacity;
    printf("begin:[%d]\n", q->begin);
    q->len--;
    return temp;
}

int isFull(Queue* q){
    return !(q->len - q->capacity);
}

int isEmpty(Queue* q){
    return !q->len;
}

void delQ(Queue** q){
    Queue* temp = *q;
    free(temp->data);
    free(*q);
    *q = NULL;
}

int main(){
    Queue* q = malloc(sizeof(Queue));
    initQ(q);
    printf("%d\n", q->capacity);
    enq(q, 0);
    enq(q, 1);
    enq(q, 2);
    enq(q, 3);
    enq(q, 4);
    //printf("out:% d\n", deq(q));
    enq(q, 5);
    printf("out:% d\n", deq(q));
    printf("out:% d\n", deq(q));
    printf("out:% d\n", deq(q));
    printf("out:% d\n", deq(q));
    printf("out:% d\n", deq(q));
    printf("out:% d\n", deq(q));
    printf("%d\n", q->capacity);

    delQ(&q);
    return 0;
}
