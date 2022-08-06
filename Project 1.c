#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#define MaxCount 1000
#define MaxLen 500

int view_file(FILE* fp);
void print_line(FILE* fp, int total_line);
int read_line(char* buffer, int num, FILE* fp);

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
	while (read_line(buffer, MaxCount, fp) != NULL)
	{
		printf("%5d %s\n", line_num++, buffer);
	}
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
				read_line(arr, MaxCount, fp);

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

int read_line(char* buffer, int num, FILE* fp)
{
	int str_size;

	if (fgets(buffer, num, fp) != NULL)
	{
		str_size = strlen(buffer);
		if (buffer[str_size - 1] == '\n')
			buffer[str_size - 1] = '`';
		return str_size;
	}

	return NULL;
}



