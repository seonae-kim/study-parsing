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
char num[100];

/**
 * @struct save value
 */
struct List {	//structure
	char name[10];
	int size;	//16진수로 된 문자열
	int offset;
};

int htoi(char []);
void set_list( FILE*, struct List*, int );
void print_list(struct List[], int);

int main(int argc, char *argv[])
{
	char buf[100] = {'\0',};
	int i;	//for for문
	int count = 0;
	int c, opt = 0, optf = 0;	//switch

	struct List* list;

	char *file;
	char *default_f = "fwenv.config";

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


	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//count volmue
	{
		if( !strncmp(buf, "volume", 6))
		{
			count++;
		}
	}

	list = (struct List*) malloc(sizeof(struct List)*count);

	if(list == NULL)
			printf("malloc fail\n");

	rewind(fp);

	for(i=0; i<count; i++)
	{

		while( (fgets(buf, sizeof(buf), fp)) != NULL)	//first volume
		{
			if( !strncmp(buf, "volume", 6))		//enter set_list with volume.
			{
				set_list(fp, list, i);
				break;	//if return, break; for next volume
			}
	
		}
		print_list(list, i);
	}

	
	free(list);

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
void set_list(FILE* fp, struct List list[], int i)
{

	char *str;
	char buf[100] = {'\0'};
	int decimal;

	while((fgets(buf, sizeof(buf), fp)) != NULL)	//buf가 pointer일 경우 EOF, 배열이면 NULL
	{
		if(!strncmp(buf, "\t\tname", 6))	//name
		{
			str = strstr(buf, "=");
			strncpy(list[i].name, str+2, 20);	//str is "= name" so to get name, moved pointer +2
		}
	
		else if(!strncmp(buf, "\t\tsize", 6))	//size
		{
			str = strstr(buf, "=");
			strcpy(buf, str+2);
			
			if( strchr(buf, 'x') != NULL){
				decimal = htoi(buf);
				list[i].size = decimal; //change_decimal(buf);
			}
			else
			{
				list[i].size = atoi(str+2);
			}
		}
		else if(!strncmp(buf, "\t\toffset", 8))	//offset
		{
			str = strstr(buf, "=");
			strcpy(buf, str+2);

			if( strchr(buf, 'x') != NULL)
			{
				decimal = htoi(buf);
				list[i].offset = decimal;
			}
			else
			{
				list[i].offset = atoi(str+2);
			}
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
	
void print_list(struct List list[], int i)
{
	printf("name : %s", list[i].name);
	printf("size : %d\n", list[i].size);
	printf("offset : %d\n", list[i].offset);

	printf("\n");

	return;
}

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
