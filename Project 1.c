#pragma warning(disable: 4996) 

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MaxLenofLine 10000
#define Max 1000
#define MaxLineNum 1000
#define UP 72
#define DOWN 80
#define APP 65 //append line
#define DEL 68
#define EDIT 69
#define INS 73

typedef struct command_box
{
	char work;
	char input[50];
}COMMAND;

typedef struct file_info
{
	int current_line;
	int total_line;
	char* text[1000];
}INFO;

int view_file(FILE* fp, INFO* fi);
void print_line(int command_line, INFO* fi);
//int read_line(char* buffer, int num, FILE* fp);
int myFgets(char* buffer, int max, FILE* fp);
int get_command(COMMAND* c);
int move_line(const int num, const char sign, INFO* fi);
void append_line(INFO* fi);
void save_file(FILE* fp, INFO* fi, int count);

int main(int argc, char* argv[])   
{
	FILE* file;
	COMMAND c;
	INFO fi;

	int convert_input;
	int append_count = 0;
	char answer;

	if (argc < 2)
	{
		printf("인수의 개수가 부족합니다.\n 아래와 같이 입력하세요.\n");
		printf("> 0704.exe <파일명> \n");
		return 0; 
	}

	else
	{
		file = fopen(argv[1], "a+");

		if (file == NULL)
		{
			printf("File does not exist.");
			return 0;
		}
			
		else
		{
			fi.total_line = view_file(file, &fi);
			fi.current_line = fi.total_line - 1;
			print_line(fi.total_line, &fi);

			while (1)
			{
				if (!get_command(&c))
				{
					if (append_count != 0) //when there is(are) the line(s) that the user appended.
					{
						while (1)
						{
							printf("\n\n   Save your changes? (Y / N) ");
							scanf("%c", &answer);

							if (answer == 'Y' || answer == 'y')
							{
								save_file(file, &fi, append_count);
								break;
							}

							else if (answer == 'N' || answer == 'n')
							{
								//free(fi.text); 동적 할당된 메모리를 어떻게 release 할 건지 다시 생각해 볼 것.
								break;
							}

							else
								printf("Invalid Command");
						}
						printf("\n   Quit Line Editior.\n");
						fclose(file);
						return 0;
					}
					else
						break;
				}
				else
				{
					printf("\n");
					switch (c.work)
					{
						case'M':
						{
							convert_input = atoi(c.input);
							move_line(convert_input, c.input[0], &fi);
							break;
						}

						case 'A':
						{
							append_line(&fi);
							++append_count;
							break;
						}
						default:
						{
							printf("\n   Please enter the valid command.(Line Number / A / D / E / I / Up Arrow / Down Arrow / Pg Up / Pg Dn)\n");
							break;
						}
					}
				}
			}
			printf("\n   Quit Line Editior.\n");
			fclose(file);
			return 0;
		}
	}
}

int view_file(FILE* fp, INFO* fi)
{
	char* buffer = NULL;
	char* second_buffer = NULL;

	int line_num = 1;
	int ptr_num = 0;
	int text_len = 0;
	
	while(line_num)	//while (read_line(buffer, MaxCount, fp) != NULL)
	{
		buffer = (char*)malloc(MaxLenofLine * sizeof(char));

		if (!buffer)
		{
			printf("Memory allocation failed.");
			exit(1);
		}

		else
		{
			if (myFgets(buffer, MaxLenofLine, fp) == NULL)
			{
				if (line_num == 1)
				{
					printf("\n\n   Empty File.\n");
					exit(1);
				}

				else
				{
					//printf("\n%5d: %s\n", line_num - 1, ptr_arr[ptr_num - 1]);
					break;
				}
			}

			else
			{
				text_len = strlen(buffer);
				second_buffer = (char*)realloc(buffer, (sizeof(char) * text_len) + 1);

				if (second_buffer == NULL)
				{
					printf("Memory reallocation failed.");
					free(buffer);
					exit(1);
				}

				else 
				{
					fi->text[ptr_num] = second_buffer;
					printf("%5d %s\n", line_num++, fi->text[ptr_num++]);
				}
			}
		}
	};
	rewind(fp);
	printf("\n");
	//free(buffer);
	//free(second_buffer);
	fi->total_line = line_num - 1;

	return line_num - 1;
}

void print_line(const int command_line, INFO* fi)
{
	char arr[Max] = "";
	int str_size = 0;

	if (command_line > 0 && command_line <= fi->total_line)
	{
		printf("%5d:%s\n", command_line, fi->text[command_line - 1]);
	}

	else if (command_line < 0 || command_line > fi->total_line || command_line == 0 && (isdigit(command_line)) != 0)
	{
		printf("\n   Out of range value.\n");
		printf("   Enter the value between 1 and %d.\n", fi->total_line);
	}

	else
		printf("   Invalid Command.\n");
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

//Use myFgets ftn instead of read_line ftn by replacing fgets to the fread ftn.
int myFgets(char* buffer, int max, FILE* fp)
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
	return 1;
}

int get_command(COMMAND* c)
{
	int i = 1;
	int k;

	char answer[50];
	unsigned char ch;
	char sc;

	printf("\n   > ");
	
	ch = _getch();

	if (ch == 224) //If the user entered arrow keys
	{
		sc = _getch();
		switch (sc)
		{
			case 72: //Up arrow key
			{
				printf("Up");
				c->work = 'M';
				strcpy(c->input, "-1");
				break;
			}

			case 80: //Down arrow key
			{
				printf("Down");
				c->work = 'M';
				strcpy(c->input, "+1");
				break;
			}

			case 73: //Page Up
			{
				printf("Pg Up");
				c->work = 'M';
				strcpy(c->input, "-25");
				break;
			}

			case 81: //Page Down
			{
				printf("Pg Dn");
				c->work = 'M';
				strcpy(c->input, "+25");
				break;
			}

			default:
				break;
		}
		return 1;
	}

	else if (isdigit(ch))
	{
		answer[0] = ch;
		printf("%c", ch);

		while (1)
		{
			sc = _getche();
			if (sc != 13)
				answer[i++] = sc;
			else
			{
				answer[i] = '\0';
				break;
			}
		}

		c->work = 'M';
		c->input[0] = '0';

		for (k = 1; k < i + 1; k++)
		{
			c->input[k] = answer[k - 1];
		}
		c->input[k] = '\0';
		return 1;
	}

	else if (ch == 81 || ch == 113)
	{
		printf("%c", ch);
		return 0;
	}

	else if (isalpha(ch)) //사용자가 'A', 'E', 'D', 'I', 'a', 'e', 'd', 'i' 중에 한 명령어를 입력했을 경우
	{
		if (islower(ch))
			ch = toupper(ch);
		printf("%c", ch);

		switch(ch)
		{
			case APP:
			{
				c->work = 'A';
				break;
			}
			case DEL:
			{
				c->work = 'D';
				break;
			}
			case EDIT:
			{
				c->work = 'E';
				break;
			}
			case INS:
			{
				c->work = 'I';
				break;
			}
			default:
			{
				//printf("Enter the valid commmand. (A / D / E / I)");
				break;
			}
		}
		return 1;
	}
	printf("\n");
	return 0;
}

int move_line(const int num, const char sign, INFO* fi)
{
	int tmp;
	int i, k;

	switch (sign)
	{
		case'0': //num 번째 줄로 이동
		{
			if (0 < num && num <= fi->total_line)
			{
				fi->current_line = num - 1;
				print_line(num, fi);
				break;
			}
			else
			{
				printf("\n   Out of range value.\n");
				printf("   Enter the value between 1 and %d.\n", fi->total_line);
				break;
			}
		}

		case'-': //(current line - num) 번째 줄로 이동
		{
			if (num == -1) //Up Arrow Key
			{
				if (0 < (fi->current_line) + 1 + num)
				{
					fi->current_line = fi->current_line - 1;
					print_line(fi->current_line + 1, fi);
					break;
				}
			}
					
			else if(num == -25) //Page Up Key
			{
				tmp = fi->current_line;
				if (tmp != 0)
				{
					fi->current_line = fi->current_line - 24;
					if (fi->current_line + 1 >= 0)
					{
						for (i = fi->current_line + 1; i <= tmp + 1; i++)
							print_line(i, fi);
					}
					else
					{
						fi->current_line = 0;
						for (k = 1; k <= tmp + 1; k++)
							print_line(k, fi);
					}
					break;
				}
			}
		}
		print_line(1, fi);
		printf("   This is the end of file.\n");
		break;

		case'+': //(current line + num) 번째 줄로 이동
		{
			if (num == 1) //Down Arrow Key
			{
				if (fi->current_line + 1 + num <= fi->total_line)
				{
					fi->current_line = fi->current_line + 1;
					print_line(fi->current_line + 1, fi);
					break;
				}
			}
				
			else if (num == 25) //Page Down Key
			{
				tmp = fi->current_line;
				if (tmp != fi->total_line - 1)
				{
					fi->current_line = fi->current_line + 24;
					if (fi->current_line + 1 <= fi->total_line)
					{
						for (i = tmp + 1; i <= fi->current_line + 1; i++)
							print_line(i, fi);
					}
					else if (fi->current_line + 1 > fi->total_line)
					{
						fi->current_line = fi->total_line - 1;
						for (k = tmp + 1; k <= fi->total_line; k++)
							print_line(k, fi);
					}
					break;
				}
			}
		}
		print_line(fi->total_line, fi);
		printf("   This is the end of file.\n");
		break;
	}
	return 1;
}

void append_line(INFO* fi)
{
	int new_line;
	int data_len;
	char* tmp_buffer;
	char* new_buffer;

	new_line = fi->total_line + 1;
	fi->total_line++;

	printf("\n   Enter the new line. \n");
	printf("\n%5d:", new_line);

	new_buffer = (char*)malloc(MaxLenofLine * sizeof(char));

    if (!new_buffer)
    {
	   printf("Additional memory allocation failed.");
 	   exit(1);
    }

    else
    {
	   gets_s(new_buffer, MaxLenofLine * sizeof(char));
	   data_len = strlen(new_buffer);
	    
	   tmp_buffer = (char*)realloc(new_buffer, (sizeof(char) * data_len) + 1);
	   if (tmp_buffer == NULL)
	   {
		  printf("Additional memory reallocation failed.");
		  free(new_buffer);
		  exit(1);
	   }
	   else
	   {
		  fi->text[new_line - 1] = tmp_buffer;
	   }

    }

	fi->current_line = new_line - 1;
}

void save_file(FILE* fp, INFO* fi, int count)
{
	int append_count = count;

	for (int i = count; i > 0; i--)
	{
		fputs(fi->text[fi->total_line - i], fp);
		fputc('\n', fp);
	}
}