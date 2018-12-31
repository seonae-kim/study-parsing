#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

char str[100] = {'\0',};
struct List list;
char *p = NULL;

struct List {
	char name[10];
	int size;
	int offset;
};

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
void set_list(char [], int, linkedList*);//(char [], int);

int main(int argc, char *argv[])
{
	char buf[1] = {'\0',};	//data buf
	int i=0;
	int fp;
	char *file;
	char *default_f = "fwenv.config";
	int c, opt = 0, optf = 0;

	linkedList *L = (linkedList *)malloc(sizeof(linkedList));

	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;

	while(( c = getopt(argc, argv, "hvf:")) != -1)
	{
		opt = 1;
		switch (c) {
			case 'h':
				printf("Usage:\n");
				printf("./a.out [option] filename\n");
				printf("Options\n");
				printf("-h		help\n");
				printf("-v		version\n");
				printf("-f <filename>	filename\n");
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

	fp = open(file, O_RDONLY);
	
	if(fp == -1)
		perror("open error");
	else
		printf("file open success.\n");

	while( read(fp, buf, sizeof(buf)) != 0 )
	{
		
		if( *buf == '\n')
		{
			i = 0;
			if( !strncmp(str, "volume",6))
			{
				createNode(L);
				set_list(str, fp, L);
			}

			memset(str, '\0', 100);
		}
		else
		{
			str[i] = *buf;
			i++;
		}
	}
	
		printNodes(L);

	close(fp);
	return 0;
}

/**
 * @param str str is store file data
 * @param fp file pointer
 * @param L linkedList
 */
void set_list(char *str, int fp, linkedList * L)// int fp)
{
	char buf[1] = {'\0'};
	int i=0, j;
	p = str;
	char *endptr;
	int decimal;
	
	while( read(fp, buf, sizeof(buf)) != 0)
	{	
		if( *buf == '\n')
		{
			if(!strncmp(str, "\t\tname", 6))
			{

				for( j = 0; j < strlen(str); j++)
				{
					if(str[j] == '=')
					{
						p = p+j+2;
						break;
					}
					else if(j == strlen(str)-1 && str[j] != '=')
						return;
				}
				strncpy(L->cur->name,  p, strlen(p));
			}

			else if(!strncmp(str, "\t\tsize", 6))
			{
				for( j = 0; j < strlen(str); j++)
				{
					if(str[j] == '=')
					{
						p = p+j+2;
						break;
					}
					else if(j == strlen(str)-1 && str[j] != '=')
						return;
				}

				decimal = strtoul(p, &endptr, 0);
				
				if( *endptr != 0)
					L->cur->size = -1;
				else
					L->cur->size = decimal;
				
			}

			else if(!strncmp(str, "\t\toffset", 8))
			{
				for( j = 0; j < strlen(str)-1; j++)
				{
					if(str[j] == '=')
					{
						p = p+j+2;
						break;
					}
					else if(j == strlen(str)-1 && str[j] != '=')
						return;
				}
				
				decimal = strtoul(p, &endptr, 0);

				if( *endptr != 0)
					L->cur->offset = -1;
				else
					L->cur->offset = decimal;
			}
			
			else if(strchr(str, '}') != NULL)
			{
				return;
			}

			i = 0;
			memset(str, '\0', 100);
			p = str;
		}
		else
		{
			str[i] = *buf;
			i++;
		}

	}

}

/**
 * @param L linkedList
 */
void createNode(linkedList *L)
{
	node *newNode = (node*)malloc(sizeof(node));
	newNode->next = NULL;
	
	if(L->head == NULL && L->tail == NULL)
		L->head = L->tail = newNode;
	else
	{
		L->tail->next = newNode;
		L->tail = newNode;
	}

	L->cur = newNode;

}

/**
 * @param L linkedList
 */
void printNodes(linkedList *L)
{
	node *p = L->head;

	while(p != NULL)
	{
		printf("name : %s\n", p->name);
		printf("size : %d\n", p->size);
		printf("offset : %d\n", p->offset);
		p = p->next;
		printf("\n");
	}
	putchar('\n');
}

