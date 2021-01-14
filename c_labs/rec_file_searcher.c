#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#define STR_LEN 256
#define START_DIR "./labyrinth"
#define START_FILE "file.txt"
#define KEY_WORD "Minotaur"

typedef struct Storage{
	char** paths;
	int len;
	int capacity;
}Storage;

int get_content(char* filename, Storage** storage);
int search_dir(char* start_path, char* key_fname, Storage** storage);
void resizeStorage(Storage** storage);

void resizeStorage(Storage** storage){
	int i = 0;
	(*storage)->capacity += 5;
	(*storage)->paths = realloc((*storage)->paths, (*storage)->capacity*sizeof(char*));
	for (i = (*storage)->len; i < (*storage)->capacity; ++i){
		(*storage)->paths[i] = malloc(STR_LEN*sizeof(char));
	}	
}

int get_content(char* filename, Storage** storage){
	int i = 0;
	Storage* ptr = *storage;
	FILE* file;
	if(!(file = fopen(filename, "r"))){
		printf("File can't be opened due to some problems\n");
		return 0;
	}
	char* buffer = malloc(STR_LEN*sizeof(char));
	char* temp = malloc(STR_LEN*sizeof(char));
	while(fgets(buffer, STR_LEN, file)){
		if(buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
		if(strcmp(buffer, KEY_WORD) == 0){
				sprintf(ptr->paths[ptr->len], "%s", filename);
				ptr->len++;
				if(ptr->len == ptr->capacity - 1)
					resizeStorage(storage);
				fclose(file);
				free(temp);
				free(buffer);
				return 1;
		}
		if(strcmp(buffer, "Deadlock") != 0){
			sscanf(buffer, "@include %s", temp);
			sprintf(buffer, "%s", temp);
			if(search_dir(START_DIR, temp, storage)){
				sprintf(ptr->paths[ptr->len], "%s", filename);
				ptr->len++;
				if(ptr->len == ptr->capacity - 1)
					resizeStorage(storage);
				fclose(file);
				free(temp);
				free(buffer);
				return 1;
			}
		}
		temp[0] = '\0';	
	}
	free(buffer);
	free(temp);
	fclose(file);
	return 0;	
}


int search_dir(char* start_path, char* key_fname, Storage** storage){
	char* next_path = malloc(STR_LEN*sizeof(char));
	strcpy(next_path, start_path);
	DIR* dir;
	if(!(dir = opendir(start_path))){
		printf("Something's wrong\n");
		return 0 ;
	}
	struct dirent* info = readdir(dir);
	while(info){
		int len = strlen(next_path);
        strcat(next_path, "/");
        strcat(next_path, info->d_name);
        if(info->d_type == DT_REG && strcmp(info->d_name, key_fname) == 0){
			if(get_content(next_path, storage)){
				closedir(dir);
				free(next_path);
				return 1;	
			}
		}	
		if(info->d_type == DT_DIR && strcmp(info->d_name, ".") != 0 && strcmp(info->d_name, "..") != 0){
			if(search_dir(next_path, key_fname, storage)){
				closedir(dir);
				free(next_path);
				return 1;
			}
		}	
		next_path[len] = '\0';
		info = readdir(dir);
	}
	closedir(dir);
	free(next_path);
	return 0;
}

int main(int argc, char** argv){
	FILE* res = fopen("result.txt", "w");
	Storage* storage = malloc(sizeof(Storage));
	storage->len = 0;
	storage->capacity = 5;
	storage->paths = malloc(storage->capacity*sizeof(char*));
	int i = 0;
	for (i = 0; i < storage->capacity; ++i){
		storage->paths[i] = malloc(STR_LEN*sizeof(char));
	}
	search_dir(START_DIR, START_FILE, &storage);
	for (i = storage->len - 1; i >= 0; i--){
		fprintf(res, "%s", storage->paths[i]);
		if (i > 0) fprintf(res, "\n");
	}

	for (i = 0; i < storage->capacity; i++){
		free(storage->paths[i]);
	}
	free(storage);
	fclose(res);
	return 0;
}