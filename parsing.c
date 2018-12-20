#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOPEN 1


FILE* fp = NULL;	//global variable

struct List {	//structure
	char name[10];
	char size[10];
	char offset[10];
};

void set_list( FILE*, struct List );

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
			set_list(fp, list1);
			break;	//if return, break; for next volume
		}
	
	}

	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//second volume
	{
		if( !strncmp(buf, "volume", 6))
		{
			set_list(fp, list2);
			break;
		}

	}

	
	while( (fgets(buf, sizeof(buf), fp)) != NULL)	//third volume
	{
		if( !strncmp(buf, "volume", 6))
		{
			set_list(fp, list3);
			break;
		}
	}
	
	fclose(fp);

	return 0;

}

/**
 * @param fp list
 * store value in list
 * */
void set_list(FILE* fp, struct List list)
{


	char *str;
	//struct List list;
	char buf[100] = {'\0'};

	printf("\n");

	while((fgets(buf, sizeof(buf), fp)) != NULL)	//buf가 pointer일 경우 EOF, 배열이면 NULL
	{
		if(!strncmp(buf, "\t\tname", 6))	//name
		{
			str = strstr(buf, "= ");
			strcpy(list.name, str+2);	//str is "= name" so to get name, moved pointer +2
			printf("%s", list.name);	//debug
		}
	
		else if(!strncmp(buf, "\t\tsize", 6))	//size
		{
			str = strstr(buf, "= ");
			strcpy(list.size, str+2);
			printf("%s", list.size);
		}
		else if(!strncmp(buf, "\t\toffset", 8))	//offset
		{
			str = strstr(buf, "= ");
			strcpy(list.offset, str+2);
			printf("%s", list.offset);
		}
		else if(!strncmp(buf, "}",1))	//finish
			return;
		else
			return;
	}
	return;
}

//}
/**
 * print buf
 * use malloc
 * use ftell
 */
	/**
	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	rewind(fp);

	buf = (char*)malloc(sizeof(char) * f_size);
	if(buf == NULL)
		printf("Memory error");
	
	result = fread(buf, 1, f_size, fp);
	
	printf("%s\n", buf);
*/

//---------------------------------------------------------------------

