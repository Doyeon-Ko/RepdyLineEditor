#pragma warning(disable: 4996) 

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

#define MaxLenofLine 10000
#define Max 1000
#define MaxLineNum 1000
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define APP 65 //append line at the bottom of file
#define DEL 68 //delete line
#define EDIT 69 //edit text
#define INS 73 //insert line
#define END 79 //end editing

typedef struct command_box
{
	char work;
	char input[50];
}COMMAND;

typedef struct file_info
{
	char file_name[100];
	int current_line;
	int total_line;
	char* text[1000];
	int edited_num[MaxLineNum];//몇 번째 줄을 편집했는지 기억하는 공간
	char* edited_line[MaxLineNum]; //편집한 새로운 line의 주소를 기억하는 공간
}INFO;

void load_file(FILE* fp, INFO* fi);
void view_file(FILE* fp, INFO* fi);
void print_line(int command_line, INFO* fi);
//int read_line(char* buffer, int num, FILE* fp);
int myFgets(char* buffer, int max, FILE* fp);
int get_command(COMMAND* c);
int move_line(const int num, const char sign, INFO* fi);
void append_line(INFO* fi);
void insert_line(INFO* fi);
void delete_line(INFO* fi);
void gotoxy(int x, int y);
void edit_line(INFO* fi, FILE* fp);
void save_file(FILE* fp, INFO* fi);

int main(int argc, char* argv[])   
{
	FILE* fpToRead;
	COMMAND c;
	INFO fi;

	int convert_input;

	int append_count = 0;
	int insert_count = 0;
	int delete_count = 0;
	int edit_count = 0;

	char answer;

	if (argc < 2)
	{
		printf("인수의 개수가 부족합니다.\n 아래와 같이 입력하세요.\n");
		printf("> 0704.exe <filename> \n");
		return 0; 
	}

	else
	{
		fpToRead = fopen("TheVeryFirstNight.txt", "r+");
		strcpy(fi.file_name, argv[1]);

		if (fpToRead == NULL)
		{
			printf("File does not exist.");
			return 0;
		}
			
		else
		{
			load_file(fpToRead, &fi);
			view_file(fpToRead, &fi);
			fclose(fpToRead);

			while (1)
			{
				if (!get_command(&c))
				{
					if (append_count != 0 || insert_count != 0 || delete_count != 0 || edit_count != 0) //when there is(are) the line(s) that the user appended or inserted.
					{
						getchar();
						while (1)
						{
							printf("\n\n   Save your changes? (Y / N) ");
							scanf("%c", &answer);

							if (answer == 'Y' || answer == 'y')
							{
								if (edit_count > 0)
								{
									for (int i = 0; i < edit_count; i++)
									{
										free(fi.text[fi.edited_num[i] - 1]);
										fi.text[fi.edited_num[i] - 1] = fi.edited_line[i];
									}
								}
								save_file(fpToRead, &fi);
								break;
							}

							else if (answer == 'N' || answer == 'n')
							{
								//free(fi.text); Need to consider the memory release !
								break;
							}

							else
								printf("Invalid Command");
						}
						printf("\n   Quit Line Editior.\n");
						fclose(fpToRead);
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

						case 'I':
						{
							++insert_count;
							insert_line(&fi);
							break;
						}

						case 'D':
						{
							++delete_count;
							delete_line(&fi);
							break;
						}

						case 'E':
						{
							++edit_count;
							edit_line(&fi, fpToRead);
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
			fclose(fpToRead);
			return 0;
		}
	}
}

void load_file(FILE* fp, INFO* fi)
{
	char* buffer = NULL;
	char* second_buffer = NULL;

	int line_num = 1;
	int ptr_num = 0;
	int text_len = 0;

	while (line_num)	//while (read_line(buffer, MaxCount, fp) != NULL)
	{
		buffer = (char*)malloc(MaxLenofLine * sizeof(char));

		if (!buffer)
		{
			printf("Memory allocation for saving text file failed.");
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
					printf("Memory reallocation for saving text file failed.");
					free(buffer);
					exit(1);
				}

				else
				{
					fi->text[ptr_num++] = second_buffer;
					line_num++;
				}
			}
		}
	};

	fi->total_line = line_num - 1;
	fi->current_line = line_num - 1;

	rewind(fp);
	//free(buffer);
	//free(second_buffer);
}

void view_file(FILE* fp, INFO* fi)
{
	for (int i = 0; i < fi->total_line; i++)
	{
		print_line(i + 1, fi);
		printf("\n");
	}

	printf("\n");
	print_line(fi->current_line, fi);
	printf("\n");
}

void print_line(const int command_line, INFO* fi)
{
	int i;
	int str_size = 0;
	str_size = strlen(fi->text[command_line - 1]);

	if (command_line > 0 && command_line <= fi->total_line)
	{
		printf("%5d ", command_line);
		for (i = 0; fi->text[command_line - 1][i] != '\n'; i++)
			printf("%c", fi->text[command_line - 1][i]);
			
		if (fi->text[command_line - 1][i] == '\n')
		{
			printf("`");
			//printf("\n");
		}
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
			buffer[i] = dat;
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

	else if (isalpha(ch)) //When the user entered command('A', 'E', 'D', 'I', 'a', 'e', 'd', 'i' ).
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
		case'0': //move to the specific line
		{
			if (0 < num && num <= fi->total_line)
			{
				fi->current_line = num - 1;
				print_line(num, fi);
				printf("\n");
				break;
			}
			else
			{
				printf("\n   Out of range value.\n");
				printf("   Enter the value between 1 and %d.\n", fi->total_line);
				break;
			}
		}

		case'-': //move to the (current line - num)th line
		{
			if (num == -1) //Up Arrow Key
			{
				if (0 < (fi->current_line) + 1 + num)
				{
					fi->current_line = fi->current_line - 1;
					print_line(fi->current_line + 1, fi);
					printf("\n");
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
						{
							print_line(i, fi);
							printf("\n");
						}
							
					}
					else
					{
						fi->current_line = 0;
						for (k = 1; k <= tmp + 1; k++)
						{
							print_line(k, fi);
							printf("\n");
						}
					}
					break;
				}
			}
		}
		print_line(1, fi);
		printf("\n   This is the end of file.\n");
		break;

		case'+': //move to the (current line + num)th line
		{
			if (num == 1) //Down Arrow Key
			{
				if (fi->current_line + 1 + num <= fi->total_line)
				{
					fi->current_line = fi->current_line + 1;
					print_line(fi->current_line + 1, fi);
					printf("\n");
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
						{
							print_line(i, fi);
							printf("\n");
						}
					}
					else if (fi->current_line + 1 > fi->total_line)
					{
						fi->current_line = fi->total_line - 1;
						for (k = tmp + 1; k <= fi->total_line; k++)
						{
							print_line(k, fi);
							printf("\n");
						}
					}
					break;
				}
			}
		}
		print_line(fi->total_line, fi);
		printf("\n   This is the end of file.\n");
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
	++fi->total_line;

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
	   new_buffer[data_len] = '\n';
	   new_buffer[data_len + 1] = 0;

	   tmp_buffer = (char*)realloc(new_buffer, (sizeof(char) * data_len) + 2);
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

void insert_line(INFO* fi)
{
	int data_len;
	int ins_line;
	int ori_line = fi->total_line;
	char ch;
	char* last_line = NULL;

	printf("\n   Line Number : ");
	scanf("%d", &ins_line);
	
	last_line = (char*)malloc(MaxLenofLine * sizeof(char));
	if (!last_line)
	{
		printf("New memory allocation for saving the last line of file failed.");
		exit(1);
	}

	strcpy(last_line, fi->text[ori_line - 1]);
	fi->text[ori_line] = last_line;

	for (int k = 0; k < (fi->total_line - ins_line) + 1; k++)
	{
		fi->text[ori_line - 1] = fi->text[ori_line - 2];
		--ori_line;
	}

	char* new_text = (char*)malloc(MaxLenofLine * sizeof(char));

	if (!new_text)
	{
		printf("New memory allocation for insertion failed.");
		exit(1);
	}

	ch = getchar(); //clean the buffer
	printf("\n   Input new text : ");
	gets_s(new_text, MaxLenofLine * sizeof(char));

	data_len = strlen(new_text);
	new_text[data_len] = '\n';
	new_text[data_len + 1] = 0;

	fi->text[ins_line - 1] = new_text;

	++(fi->total_line);
}

void delete_line(INFO* fi)
{
	int del_line;
	int k;
	char ch;

	printf("\n   Line Number : ");
	scanf("%d", &del_line);
	ch = getchar();

	free(fi->text[del_line - 1]);

	for (k = 0; k < fi->total_line - del_line; k++)
	{
		fi->text[(del_line - 1) + k] = fi->text[del_line + k];
	}

	fi->text[fi->total_line - 1] = NULL;
	fi->current_line = del_line - 1;
	--(fi->total_line);
}

void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos); //커서 위치 이동 함수
}

void edit_line(INFO* fi, FILE* fp)
{
	static int e = 0;
	int edit_line;
	static int m = 0;
	unsigned char ch;
	char sc;
	char* edited_line; //편집된 text를 담을 배열. 사용자가 변경된 내용을 저장하고자 하면 return된 이 배열을 사용하고, 저장을 원치 않을 땐 활용하지 않는다.

	edited_line = (char*)malloc(MaxLenofLine * sizeof(char));
	CONSOLE_SCREEN_BUFFER_INFO curInfo; 

	printf("\n   Line Number : ");
	scanf("%d", &edit_line);
	fi->edited_num[m++] = edit_line; //m : 편집한 줄의 개수

	if (edited_line)
	{
		strcpy(edited_line, fi->text[edit_line - 1]);

		printf("\n");

		print_line(edit_line, fi);
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo); //현재 콘솔창의 커서 좌표 알아내는 함수

		int x = curInfo.dwCursorPosition.X;
		int y = curInfo.dwCursorPosition.Y;

		gotoxy(x, y);

		while (1)
		{
			ch = _getch();

			if (ch == 224)
			{
				sc = _getch();
				switch (sc)
				{
					case LEFT: //left arrow
					{
						gotoxy(--x, y);
						break;
					}
					case RIGHT: //right arrow
					{
						gotoxy(++x, y);
						break;
					}
					case END: //enter end
					{
						printf("\n");
						fi->edited_line[e++] = edited_line;
						return 0;
					}
					default:
					{
						printf("You can only enter left / right arrow key.");
						break;
					}
				}
			}

			if (ch == 8) //If the user entered the backspace
			{
				printf("\b"); //커서 위치가 좌로 한 칸 이동함. 근데 그 한 칸 내용이 지워지진 않음.
				printf(" "); //커서 위치를 변경한 후 해당 칸에 공백을 출력해 지워진 것처럼 보이게 함.
				printf("\b"); //공백을 출력한 후에 다시 한 칸 밀려난 커서를 왼쪽으로 한 칸 이동시킴.
				if (x - 7 >= 0)
				{
					edited_line[x - 7] = ' ';
					--x;
				}
			}

			else if (isalpha(ch))
			{
				printf("%c", ch);
				if (x - 6 >= 0)
				{
					edited_line[x - 6] = ch;
					x++;
				}
			}
		}
	}
}

void save_file(FILE* fp, INFO* fi)
{
	FILE* fpToWrite;
	fpToWrite = fopen(fi->file_name, "w+");

	for (int k = 0; k < fi->total_line; k++)
		fputs(fi->text[k], fpToWrite);
}