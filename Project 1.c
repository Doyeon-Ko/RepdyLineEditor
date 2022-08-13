#pragma warning(disable: 4996) 

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#define MaxCount 1000
#define MaxLen 500

int view_file(FILE* fp);
void print_line(FILE* fp, int total_line);
int read_line(char* buffer, int num, FILE* fp);
int read_line2(char* buffer, int size, FILE* fp);

int main(int argc, char* argv[])   
{
	FILE* file;
	char line_num[MaxLen] = "";
	int total_line;
	int convert_linenum;

	if (argc < 2)
	{
		printf("인수의 개수가 부족합니다.\n 아래와 같이 입력하세요.\n");
		printf("> 0704.exe <파일명> \n");
		return 0; 
	}

	else
	{
		if (file = fopen(argv[1], "r+"))
		{
			total_line = view_file(file);
			print_line(file, total_line);
		}
		
		else if (file == NULL)
		{
			printf("File does not exist.");
			return 0;
		}
	}

	return 0;
}

int view_file(FILE* fp)
{
	char* read = NULL;
	char buffer[MaxCount] = "";
	int line_num = 1;
	int fsize; //파일 크기

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);

	while(1)	//while (read_line(buffer, MaxCount, fp) != NULL)
	{
		if (read_line2(buffer, sizeof(char), fp) == NULL)
			break;
		else
			printf("%5d %s\n", line_num++, buffer);
	};
	printf("\n");

	return (line_num - 1);
}

void print_line(FILE* fp, int total_line)
{
	char arr[MaxCount];
	char* get_string = NULL;
	char answer[MaxLen] = "";
	int convert_answer = 0;
	int str_size = 0;

	while (1)
	{
		printf("\n   Line Number : ");
		scanf("%s", answer);

		if (answer[0] == '\\' && strlen(answer) == 2)
		{
			if (answer[1] == 'q' || answer[1] == 'Q')
			{
				printf("   Quit Line Editior.\n");
				fclose(fp);
				exit(1);
			}
		}

		convert_answer = atoi(answer);

		if (convert_answer > 0 && convert_answer <= total_line)
		{
			int line = 1;
			rewind(fp);
			
			while (1)
			{
				read_line2(arr, sizeof(char), fp); //read_line(arr, MaxCount, fp);
				if (line++ == convert_answer)
				{
					printf("   %s\n", arr);
					break;
				}
			}
		}

		else if (convert_answer < 0 || convert_answer > total_line || convert_answer == 0 && (isdigit(convert_answer)) != 0)
		{
			printf("\n   Out of range value.\n");
			printf("   Enter the value between 1 and %d.\n", total_line);
		}

		else
			printf("   Invalid Command.\n");
	}
}

/*int read_line(char* buffer, int num, FILE* fp)
{
	int str_len;

	if (fgets(buffer, num, fp) != NULL)
	{
		str_len = strlen(buffer);
		if (buffer[str_len - 1] == '\n')
			buffer[str_len - 1] = '`';
		return str_len;
	}

	return NULL;
}*/

//Use read_line2 ftn instead of read_line ftn by replacing fgets to the fread ftn.
int read_line2(char* buffer, int size, FILE* fp)
{
	int result;
	int i = 0;
	int k = 0;

	while (buffer[k] != '\0')
	{
		buffer[k++] = '\0';
	}

	while (1)
	{
		result = fread(&(buffer[i]), size, size, fp); //result is the length of characters that the fread ftn succeed to read.

		if (buffer[i++] == '\n')
		{
			buffer[i - 1] = '`';
			return result;
		}

		else
		{
			if (result == 0 || result == EOF) //the case that fail to read the file or the file pointer points to the end of file.
				return 0;
			else
				continue;
		}
	}
}

