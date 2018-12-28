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

void function(char [], int);
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

//TODO
//volume이 나왔을 때 해당 str을 함수로 보내 = 뒤로 분리하고 return 해서 출력..
//배열에 다시 어떻게 넣느냐가 관건이려나..... 

void function(char *str, int fp)
{
	char buf[1] = {'\0'};
	int i=0, j;
	//char *p = NULL;
	p = str;
	
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
				printf("result1 : %s\n", p/*list.name*/);
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
				for(j = 0; j < strlen(p); j++)
				{
					if(*(p+j) == 'x')
					{
						list.size = htoi(p);
						break;
					}
					else
						list.size = atoi(p);
				}
				
				//strncpy(list.size,  p, strlen(p));
				printf("result2 : %s\n", p/*list.size*/);
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

				for(j = 0; j < strlen(p); j++)
				{
					if(*(p+j) == 'x')
					{
						list.offset = htoi(p);
						break;
					}
					else
						list.offset = atoi(p);
				}
			//	strncpy(list.offset, p, strlen(p));
				printf("result3 : %s\n", p/*list.offset*/);
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
	

int htoi(char *buf)
{
	int i;
	int decimal = 0;
	int position = 0;

	for( i = strlen(buf) -1; i >= 0; i--)
	{
		char ch = *(buf+i);

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
