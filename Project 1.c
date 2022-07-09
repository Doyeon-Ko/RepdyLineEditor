#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#define MaxCount 1000
#define MaxLen 500

int view_file(FILE* fp);
void print_line(FILE* fp, int line_num, int total_line);

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
			total_line = view_file(file);

		else if (file == NULL)
		{
			printf("File does not exist.");
			return 0;
		}
	}
	
	while (1)
	{
		printf("\n   Line Number : ");
		scanf("%s", &line_num);

		if (line_num[0] == '\\' && strlen(line_num) == 2)
		{
			if (line_num[1] == 'q' || line_num[1] == 'Q')
			{
				printf("   Quit Line Editior.\n");
				fclose(file);
				exit(1);
			}
		}

		convert_linenum = atoi(line_num);
		if (!convert_linenum)
		{
			printf("   Invalid Command.\n");
			continue;
		}

		if (convert_linenum > 0 && convert_linenum <= total_line)
		{
			print_line(file, convert_linenum, total_line);
			break;
		}

		else if (convert_linenum > total_line)
		{
			printf("\n   Out of range value.\n");
			printf("   Enter the value between 1 and %d.\n", total_line);
		}
	}

	return 0;
}

int view_file(FILE* fp)
{
	char buffer[MaxCount];
	int line_num = 1;
	while (fgets(buffer, MaxCount, fp) != NULL)
	{
		printf("%5d %s", line_num++, buffer);
	}
	printf("\n");
	//int fpPosition = ftell(fp);
	//printf("\n현재 파일 포인터의 위치 : %d", fpPosition);
	//rewind(fp); 함수 독립성 문제, 한 함수는 하나의 기능만 독립적으로 수행하도록 함.
	return (line_num - 1);
}

void print_line(FILE* fp, int linenum, int total_line)
{
	int line = 1;
	char arr[MaxCount];
	char* get_string;
	char answer[10] = "";
	int convert_answer;

	while (1)
	{
		get_string = fgets(arr, MaxCount, fp);

		if (line++ == linenum)
		{
			printf("   %s", get_string);
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
					rewind(fp);
					print_line(fp, convert_answer, total_line);
					break;
				}

				else if (convert_answer > total_line)
				{
					printf("\n   Out of range value.\n");
					printf("   Enter the value between 1 and %d.\n", total_line);
					continue;
				}

				else
					printf("   Invalid Command.\n");
			}
		}
	}
}