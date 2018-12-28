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

#define FOPEN 1


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
int htoi(char []);
void set_list(FILE*, linkedList*);

int main(int argc, char *argv[])
{
	char buf[100] = {'\0',};
	int i;	//for for문
	int c, opt = 0, optf = 0;	//switch

	struct List* list;

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
		printf("if you want to help, type '-h'\n");
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
			
			if( strchr(buf, 'x') != NULL){
				decimal = htoi(buf);
				list->cur->size = decimal;
			}
			else
			{
				list->cur->size = atoi(str+2);
			}
		}
		else if(!strncmp(buf, "\t\toffset", 8))	//offset
		{
			str = strstr(buf, "=");
			strcpy(buf, str+2);

			if( strchr(buf, 'x') != NULL)
			{
				decimal = htoi(buf);
				list->cur->offset = decimal;
			}
			else 
			{
				list->cur->offset = atoi(str+2);
			}
		}
		else if(!strncmp(buf, "}",1))	//finish
			return;
	}
	return;

}
/**
 * @param buf 
 */
int htoi(char buf[])
{
	int i;
	int decimal = 0;	//10진수를 저장할 변수
	int position = 0;	//자리 수
	

	for( i = strlen(buf) -1; i >= 0; i--)
	{
		char ch = buf[i];

		if( ch == 'x')
		{
			return decimal;
		}
		else if( ch >= '0' && ch <= '9')
		{
			decimal += (ch - '0') * (1 << (4 * position));
		}
		else if (ch >= 'a' && ch <= 'f')
		{
			decimal += (ch - ('a' - 0xa)) * (1 << (4 *  position));
		}
		else if (ch >= 'A' && ch <= 'F')
		{
			decimal += (ch - ('A' - 0xa)) * (1 << (4 *  position));
		}
		else
		{
			continue;
		}


		position++;

	}
	return -1;
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


