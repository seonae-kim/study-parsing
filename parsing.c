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
#include <unistd.h>

FILE* fp = NULL;	//global variable

typedef struct _list {
	struct _node *cur;
	struct _node *head;
	struct _node *tail;
} linkedList;

typedef struct _node {
	char name[10];
	int size;
	int offset;
	struct _node *next;
} node;

void createNode(linkedList *);
void printNodes(linkedList *);
void set_list(FILE*, linkedList*);

int main(int argc, char *argv[])
{
	char buf[100] = {'\0',};
	int i;	//for for문
	int c, opt = 0, optf = 0;	//switch

	char *file;
	char *default_f = "fwenv.config";

	linkedList *L = (linkedList *)malloc(sizeof(linkedList));
	
	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;

	while( (c = getopt(argc, argv, "hvf:")) != -1)
	{
		opt = 1;
		switch (c) {
			case 'h':
				printf("Usage:\n");
				printf("./a.out [option] filename\n");
				printf("Options\n");
				printf("-h			help\n");
				printf("-v			version\n");
				printf("-f <filename>		filename\n");
				exit(0);
				break;
			case 'v':
				printf("\nversion 1.0\n\n");
				break;
			case 'f':
					file = optarg;
					optf = 1;
					break;
			default:
				break;
		}
	}
	if(opt < 1)
	{	
		printf("type '-h'\n");
		exit(0);
	}

	if(optf != 1)
		file = default_f;

	fp = fopen(file, "r");	//file open

	if(fp == NULL)	
		printf("file open fail\n\n");
	else
		printf("file open success\n\n");
		
	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//first volume
		
	{
		if( !strncmp(buf, "volume", 6))		//enter set_list with volume.
		{
			createNode(L);
			set_list(fp, L);
			continue;	
		}
	
		}
		printNodes(L);

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
void set_list(FILE* fp, linkedList* list)// struct List list[], int i)
{

	char *str;
	char *endptr;
	char buf[100] = {'\0'};
	int decimal;

	while((fgets(buf, sizeof(buf), fp)) != NULL)	//buf가 pointer일 경우 EOF, 배열이면 NULL
	{
		if(!strncmp(buf, "\t\tname", 6))	//name
		{
			str = strstr(buf, "=");
			strncpy(list->cur->name, str+2, 20);
		}
	
		else if(!strncmp(buf, "\t\tsize", 6))	//size
		{
			str = strstr(buf, "=");
			strcpy(buf, str+2);

			decimal = strtoul(buf, &endptr, 0);
			
			if(*endptr != '\n')//'\n')
				list->cur->size = -1;
			else
				list->cur->size = decimal;

		}
		else if(!strncmp(buf, "\t\toffset", 8))	//offset
		{
			str = strstr(buf, "=");
			strcpy(buf, str+2);

			decimal = strtoul(buf, &endptr, 0);

			if(*endptr != '\n')
				list->cur->offset = -1;
			else
				list->cur->offset = decimal;
		}
		else if(!strncmp(buf, "}",1))	//finish
			return;
	}
	return;

}

void createNode(linkedList *L)
{
	node *newNode = (node*)malloc(sizeof(node));
	newNode->next = NULL;
	newNode->size = 0;
	newNode->offset = 0;

	if(L->head == NULL && L->tail == NULL)
		L->head = L->tail = newNode;
	else
	{
		L->tail->next = newNode;
		L->tail = newNode;
	}
	
	L->cur = newNode;
}

void printNodes(linkedList *L)
{
	node *p = L->head;

	while(p != NULL)
	{
		printf("name : %s", p->name);
		printf("size : %d\n", p->size);
		printf("offset : %d\n", p->offset);
		p = p->next;
		printf("\n");
	}
	putchar('\n');
}


