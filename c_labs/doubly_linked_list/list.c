#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LEN 81

// Описание структуры MusicalComposition

typedef struct MusicalComposition{
	char name[LEN];
	char author[LEN];
	int year;

	struct MusicalComposition* next;
	struct MusicalComposition* prev;
}MusicalComposition;

// Создание структуры MusicalComposition

MusicalComposition* createMusicalComposition(char* name, char* author,int year);

// Функции для работы со списком MusicalComposition

MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n);

int count(MusicalComposition* head);

void print_data(MusicalComposition* head);

void clean_list(MusicalComposition** head);

void swap_pointers(MusicalComposition** a, MusicalComposition** b);

void swap(MusicalComposition* a, MusicalComposition* b, MusicalComposition** head);

int comp(MusicalComposition* a, MusicalComposition* b);

void sort(MusicalComposition** head);

int main(){
    int length;
    scanf("%d\n", &length);  
    if (length==0){
	printf("Seems like you don't want to create list :(\n");
	return 0;
	}
    char** names = (char**)malloc(sizeof(char*)*length);
    char** authors = (char**)malloc(sizeof(char*)*length);
    int* years = (int*)malloc(sizeof(int)*length);

    for (int i=0;i<length;i++)
    {
        char name[80];
        char author[80];

        fgets(name, 80, stdin);
        fgets(author, 80, stdin);
        fscanf(stdin, "%d\n", &years[i]);

        (*strstr(name,"\n"))=0;
        (*strstr(author,"\n"))=0;

        names[i] = (char*)malloc(sizeof(char*) * (strlen(name)+1));
        authors[i] = (char*)malloc(sizeof(char*) * (strlen(author)+1));

        strcpy(names[i], name);
        strcpy(authors[i], author);

    }
    MusicalComposition* head = createMusicalCompositionList(names, authors, years, length);

    printf("BEFORE SORTING:\n");
    print_data(head);

    int k = count(head);
    printf("%d\n", k);
    
    printf("\n");
    printf("AFTER SORTING:\n");
    sort(&head);
    print_data(head);
    k = count(head);
    printf("%d\n", k);
    clean_list(&head);
    for (int i=0;i<length;i++){
        free(names[i]);
        free(authors[i]);
    }
    free(names);
    free(authors);
    free(years);
    return 0;

}

MusicalComposition* createMusicalComposition(char* name, char* author,int year){
	MusicalComposition* item = malloc(sizeof(MusicalComposition));
	strncpy(item->name, name, LEN);
	strncpy(item->author, author, LEN);
	item->year = year;
	
	item->next = NULL;
	item->prev = NULL;
	return item;
}//создание элемента списка

MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n){
	int i = 0;
	MusicalComposition* items[n];
	items[i] = createMusicalComposition(array_names[i], array_authors[i], array_years[i]);
	for(i = 1; i<n; i++){
		items[i] = createMusicalComposition(array_names[i], array_authors[i], array_years[i]);
		items[i]->prev = items[i-1];
		items[i-1]->next = items[i];
	}
	return items[0];
}//создание списка, возвращает указатель на первый элемент

int count(MusicalComposition* head){
	int amount = 0;
	MusicalComposition* current = head;
	while(current){
		amount++;
		current=current->next;
		}
	return amount;
}//подсчет количества элементов в списке

void print_data(MusicalComposition* head){
	if(head==NULL){
		printf("List is empty\n");
		return;
		}
	MusicalComposition* current = head;
	while(current){
		printf("<%s> by %s, %d\n", current->name, current->author, current->year);
		if(current->next) printf("---\n");
		current=current->next;
	}
}//вывод названий всех композиций

void clean_list(MusicalComposition** head){
	if(*head==NULL) return;
	MusicalComposition* current = *head;
	MusicalComposition* current2;
	while(current->next != NULL){
		current2 = current->next;
		current->next = NULL;
		current->prev = NULL;
		free(current);
		current = current2;
	}
	*head = NULL;
}
void swap_pointers(MusicalComposition** a, MusicalComposition** b){
	MusicalComposition* temp = *a;
	*a = *b;
	*b = temp;
}

void swap(MusicalComposition* a, MusicalComposition* b, MusicalComposition** head){
	if(a == b) return;
	MusicalComposition* current = *head;
	MusicalComposition* a_next = a->next;
	MusicalComposition* a_prev = a->prev;
	MusicalComposition* b_next = b->next; 
	MusicalComposition* b_prev = b->prev; 
	MusicalComposition* temp = createMusicalComposition("", "", 0);//фиктивный head
	MusicalComposition* temp2 = createMusicalComposition("", "", 0);//фиктивный tail
	int pos1 = 0, pos2 = 0;//переменные для определения положения элементов в списке
	int if_head = 0, if_tail = 0;
	
	for(current = *head; current!=a; current = current->next) ++pos1;
	for(current = *head; current!=b; current = current->next) ++pos2;
	//если элементы были переданы в обратном порядке
	if(pos1 > pos2){
		swap_pointers(&a, &b);
		swap_pointers(&a_prev, &b_prev); 
		swap_pointers(&a_next, &b_next); 
	} 
	//если 1-й переданный элемент - head, для облегчения перестановки создается элемент перед ним
	if(a==*head){
		if_head = 1;
		a_prev = temp;
		a->next = a_next;
		temp->next = a; 
	}

	//если второй элемент - последний элемент списка, за ним создается еще один 
	if(!b_next){
		if_tail = 1;
		b_next = temp2;
		b->next = b_next;
		temp2->prev = b;
	}

	//если два элемента стоят друг за другом
	if(a->next == b){
		b->next = a;
		b->prev = a_prev;
		a->next = b_next;
		a->prev = b;
		b_next->prev = a;
		a_prev->next = b;
		//очистка фиктивных элементов
		if(if_head){ 
			free(temp);
			b->prev = NULL;
			*head = b;
		}
		else free(temp);

		if(if_tail){ 
			free(temp2);
			a->next = NULL;
		}
		else free(temp2);
		return;
	}
	//если между элементами есть как минимум один элемент
	a_prev->next = b;
	b->next = a->next;
	b_prev->next = a;
	a->next = b_next;
	b->prev = a_prev;
	b_next->prev = a;
	a->prev = b_prev;
	a_next->prev = b;
	//очистка фиктивных элементов
	if(if_head){
		free(temp);
		b->prev = NULL;
		*head = b;
	}
	else free(temp);

	if(if_tail){ 
		free(temp2);
		a->next = NULL;
	}
	else free(temp2);
}


int comp(MusicalComposition* a, MusicalComposition* b){
	return (a->year) - (b->year);
}//сравнивает поля year переданных элементов списка
	
void sort(MusicalComposition** head){
	MusicalComposition* current = *head;
	MusicalComposition* current2;
	for(; current; current = current->next){
		for(current2 = current->next; current2; current2 = current2->next){
			if(comp(current, current2) > 0){
			 swap(current, current2, head);
			 swap_pointers(&current, &current2);
			}
		}
	}
}//осуществляет сортировку по возрастанию года написания