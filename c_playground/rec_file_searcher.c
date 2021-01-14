#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define STR_LEN 256
#define M_INC_LEN 3
#define INC_LEN 3

char** get_content(char* filename, char** includes){
	FILE* file;
	if(!(file = fopen(filename, "r"))){
		printf("File can't be opened due to some problems\n");
		fclose(file);
		return 0;
	}
	char buffer[STR_LEN];
	while(fgets(buffer, STR_LEN, file)){
		buffer[strlen(buffer) - 1] = '\0';
		char temp[STR_LEN];
		if(strcmp(buffer, "Deadlock") != 0 && strcmp(buffer, "Minotaur") != 0){
			sscanf(buffer, "@include %s", temp);
		}
		else sscanf(buffer, "%s", temp);
		sprintf(*includes, "%s", temp);
		includes++;	
	}
	fclose(file);
	return 0;
}


void search_dir(char* start_path, char* key_fname, char* buffer, char** includes){
	char next_path[STR_LEN] = "";
	strcpy(next_path, start_path);
	DIR* dir;
	if(!(dir = opendir(start_path))){
		printf("Something's wrong\n");
	}
	struct dirent* info = readdir(dir);
	while(info){
		int len = strlen(next_path);
        strcat(next_path, "/");
        strcat(next_path, info->d_name);
        if(info->d_type == DT_REG && strcmp(info->d_name, key_fname) == 0){
			get_content(next_path, includes);
			strcat(buffer, next_path);
		}	
		if(info->d_type == DT_DIR && strcmp(info->d_name, ".") != 0 && strcmp(info->d_name, "..") != 0){
			search_dir(next_path, key_fname, buffer, includes);			
		}	
		next_path[len] = '\0';
		info = readdir(dir);
	}
	closedir(dir);
}

void find(char* start_path, char* key_fname, char* buffer, FILE* source){
	int len = INC_LEN;
	char** includes = malloc(len*sizeof(char*));
	for (int i = 0; i < len; ++i)
	{
		includes[i] = malloc(STR_LEN*sizeof(char));
		includes[i][0] = '\0';
	}

	search_dir(start_path, key_fname, buffer, includes);
	printf("PATH: %s\n", buffer);
	fprintf(source, "%s\n", buffer);
	buffer[0] = '\0';

	for (int i = 0; i < INC_LEN; ++i)
	{
		printf("CONTENT:%s\n", includes[i]);
		if(strcmp(includes[i], "Minotaur") == 0){
			printf("---------WELL DONE---------\n");
		}
		else{
			if(strcmp(includes[i], "") !=0 && strcmp(includes[i], "Deadlock") !=0){ 
				find(start_path, includes[i], buffer, source);
			}
		}
	}
	
	for (int i = 0; i < INC_LEN; ++i)
	{
		free(includes[i]);
	}
	free(includes);
}

int main(){
	FILE* res = fopen("result.txt", "w");
	char path[STR_LEN] = ""; 
	char** root_includes = malloc(M_INC_LEN*sizeof(char*));
	for (int i = 0; i < M_INC_LEN; ++i)
	{
		root_includes[i] = malloc(STR_LEN*sizeof(char));
		root_includes[i][0] = '\0';
	}
	search_dir("./dir1", "file.txt", path, root_includes);
	//find("./dir1", "file.txt", path);
	fprintf(res, "%s\n", path);
	path[0] = '\0';
	int i = 0;
	while(i < M_INC_LEN){
		find("./dir1", root_includes[i], path, res);
		//fprintf(res, "%s\n", path);
		i++;
	}
	for (int i = 0; i < M_INC_LEN; ++i)
	{
		free(root_includes[i]);
	}
	free(root_includes);
	fclose(res);
	return 0;
}
