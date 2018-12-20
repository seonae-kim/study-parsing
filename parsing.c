/**
 * @author kimseonae
 * @brief read file, and save datas in struct list
 * @date 2018-12-20
 * @file parsing.c
 * @version 1.0.0
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOPEN 1


FILE* fp = NULL;	//global variable

/**
 * @struct save value
 */
struct List {	//structure
	char name[10];
	char size[10];
	char offset[10];
};

void set_list( FILE*, struct List* );
void print_list(struct List*);

int main(void)
{
	char buf[100] = {'\0',};
	int f_size;		//print buf value
	size_t result;	//print buf value
	
	struct List list1, list2, list3;
	
	fp = fopen("fwenv.config", "r");	//file open

	if(fp == NULL)	
		printf("file open fail\n");
	else
		printf("file open success\n");

	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//first volume
	{
		if( !strncmp(buf, "volume", 6))		//enter set_list with volume.
		{
			set_list(fp, &list1);
			break;	//if return, break; for next volume
		}
	
	}

	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//second volume
	{
		if( !strncmp(buf, "volume", 6)){
		
			set_list(fp, &list2);
			break;
		}

	}

	
	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//third volume
	{
		if( !strncmp(buf, "volume", 6))
		{
			set_list(fp, &list3);
			break;
		}
	}

	print_list(&list1);
	print_list(&list2);
	print_list(&list3);
	
	
	if(fclose(fp) == EOF) {
		perror("fwenv.config\n");
	}
	else
		printf("It's over! closed the file.\n");

	return 0;

}

/**
 * @param fp file pointer
 * @param list struct
 * @brief store value in list
 */
void set_list(FILE* fp, struct List *list)
{


	char *str;
	//struct List list;
	char buf[100] = {'\0'};
	char push[100] = {'\0'};

	while((fgets(buf, sizeof(buf), fp)) != NULL)	//buf가 pointer일 경우 EOF, 배열이면 NULL
	{
		if(!strncmp(buf, "\t\tname", 6))	//name
		{
			str = strstr(buf, "= ");
			strncpy(list->name, str+2, 20);	//str is "= name" so to get name, moved pointer +2
		}
	
		else if(!strncmp(buf, "\t\tsize", 6))	//size
		{
			str = strstr(buf, "= ");
			strcpy(list->size, str+2);
		}
		else if(!strncmp(buf, "\t\toffset", 8))	//offset
		{
			str = strstr(buf, "= ");
			strcpy(list->offset, str+2);
		}
		else if(!strncmp(buf, "}",1))	//finish
			return;
	}
	return;
}

/**
 * @brief print data
 * @param list structure
 */
void print_list(struct List *list)
{
	printf("%s", list->name);
	printf("%s", list->size);
	printf("%s", list->offset);

	printf("\n");

	return;
}
