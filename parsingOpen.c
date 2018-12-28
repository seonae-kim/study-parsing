#include <stdio.h>
#include <string.h>
#include <fcntl.h>

char str[100] = {'\0',};
struct List list;
char *p = NULL;
//p = str;

struct List {
	char name[10];
	int size;
	int offset;
};

void set_list(char [], int);
int htoi(char *);
int main(void)
{
	char buf[1] = {'\0',};	//data buf
	int i=0;
	int fp;
	fp = open("fwenv.config", O_RDONLY);
	
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
				function(str, fp);
			}

			memset(str, '\0', 100);
		}
		else
		{
			str[i] = *buf;
			i++;
		}
		
	}
	
	printf("END : %s %d %d \n", list.name, list.size, list.offset);

	close(fp);
	return 0;
}


void set_list(char *str, int fp)
{
	char buf[1] = {'\0'};
	int i=0, j;
	//char *p = NULL;
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
				strncpy(list.name,  p, strlen(p));
				printf("result1 : %s\n", list.name);
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
					list.size = -1;
				else
					list.size = decimal;
				
				printf("result2 : %d\n", list.size);
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
					list.offset = -1;
				else
					list.offset = decimal;
				
				printf("result3 : %d\n", list.offset);
			}
			
			else if(!strncmp(buf, "}",1))
				return;

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
	

