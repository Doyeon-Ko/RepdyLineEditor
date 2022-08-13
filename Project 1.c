#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#define MaxCount 1000
#define MaxLen 500

int view_file(FILE* fp);
void print_line(FILE* fp, int total_line);
int read_line(char* buffer, int num, FILE* fp);
int read_line2(char* buffer, int max, FILE* fp);

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
	char buffer[MaxCount] = { 'a', 'b', 'c', NULL, 'e', 'f'}; //in the case that the buffer(memory) is not initialized with 0.

	int line_num = 1;

	while(line_num)	//while (read_line(buffer, MaxCount, fp) != NULL)
	{
		if (read_line2(buffer, MaxCount, fp) == NULL)
		{
			if (line_num == 1)
			{
				printf("\n   Empty File.\n");
				exit(1);
			}

			break;
		}

		else
			printf("%5d %s\n", line_num++, buffer);
	};
	printf("\n");

	return (line_num - 1);
}

void print_line(FILE* fp, int total_line)
{
	char arr[MaxCount] = { 'a', 'b', 'c', NULL, 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', NULL, 'p', 'T', 'a', 'e', 'y', 'e', 'o', 'n', 'J', 'i', '\n', 'i', 'n'}; //In the case that the arr(memory) is not initialized with 0
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
				read_line2(arr, MaxCount, fp); //read_line(arr, MaxCount, fp);
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
	int str_len;

	if (fgets(buffer, num, fp) != NULL)
	{
		str_len = strlen(buffer);
		if (buffer[str_len - 1] == '\n')
			buffer[str_len - 1] = '`';
		return str_len;
	}

	return NULL;
}

//Use read_line2 ftn instead of read_line ftn by replacing fgets to the fread ftn.
int read_line2(char* buffer, int max, FILE* fp)
{
	int result;
	int i = 0;

	char dat = 0;

	while (i < max - 1)
	{
		result = fread(&dat, sizeof(dat), 1, fp); //result is the length of characters that the fread ftn succeed to read.

		if (result == 0 || result == EOF) //the case that fail to read the file or the file pointer points to the end of file.
		{
			buffer[i] = 0;
			return i;
		}

		if (dat == '\n')
		{
			buffer[i] = '`';
			buffer[i + 1] = 0;
			return i + 1;
		}

		buffer[i++] = dat;
	}
}

