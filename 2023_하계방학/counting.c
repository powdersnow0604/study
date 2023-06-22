#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#pragma region declaration
typedef struct {
	char name[32];
	int chosen;
	int unchosen;
	int useless;
}info;

void init(info* dir);
void dClass(info* dir, int dindex);
void insert(info** dir);

char input = '?', iter = 1, changed = 'X';
char* saveptr;
int temp, cnt = 0, curr = 0, size = 50;
char buf[32];
#pragma endregion

char path[260] = "C:\\Users\\darak\\OneDrive\\Desktop\\2023_하계방학_스터디\\static.txt";   //change here for own path


int main(void)
{

	int i;
	info* stack = (info*)malloc(size * sizeof(info));

#pragma region load and start
	FILE* file = fopen(path, "r");
	if (!file)
	{
		puts("start program\n");
		puts("please make new class\n");
		init(&stack[0]);
	}
	else
	{
		while (fgets(buf, sizeof(buf), file) != NULL)
		{
			if (cnt >= size)
			{
				size += 10;
				stack = (info*)realloc(stack, sizeof(info) * size);
			}
			if (strlen(buf) != 0 && strcmp(strtok_s(buf, " ", &saveptr), "class:") == 0)
			{
				*strchr(saveptr, '\n') = '\0';
				strcpy(stack[cnt].name, saveptr);
				fscanf(file, "%*s %d %*s %d %*s %d %*s", &stack[cnt].chosen, &stack[cnt].unchosen, &stack[cnt].useless);
				cnt++;
			}
		}
		fclose(file);
	}
#pragma endregion

#pragma region iterate
	while (iter)
	{
		if (!isgraph(input))
			input = '?';
		printf("[current class is: %s]\n", stack[curr].name);
		printf("\nchosen: %d\tchanged: %c\nunchosen: %d\tlast input: %c\nuseless: %d\n\n", stack[curr].chosen, changed, stack[curr].unchosen,input, stack[curr].useless);
		printf("| chosen: q(j)| unchosen: w(k)| useless: e(l)|\n| reset: o | quit: p | (char in \"( )\" is for sub)\n");
		printf("| change class: g | make new class: h |\n");
		printf("| delete class: b | show list: n |\n\n입력: ");
		input = _getche();
		switch (input)
		{
		#pragma region genral
		case 'q':
		{
			stack[curr].chosen++; 
			changed = 'O';
			break;
		}
		case 'w':
		{
			stack[curr].unchosen++;
			changed = 'O';
			break;
		}
		case 'e':
		{
			stack[curr].useless++;
			changed = 'O';
			break;
		}
		case 'j':
		{
			if (stack[curr].chosen)
			{
				stack[curr].chosen--;
				changed = 'O';
			}
			else
				changed = 'X';
			break;
		}
		case 'k':
		{
			if (stack[curr].unchosen)
			{
				stack[curr].unchosen--;
				changed = 'O';
			}
			else
				changed = 'X';
			break;
		}
		case 'l':
		{
			if (stack[curr].useless)
			{
				stack[curr].useless--;
				changed = 'O';
			}
			else
				changed = 'X';
			break;
		}
		case 'o':
		{
			stack[curr].chosen = 0;
			stack[curr].unchosen = 0;
			stack[curr].useless = 0;
			changed = 'O';
			break;
		}
		#pragma endregion
		case 'g':
		{
			printf("\nwhich class?(count of class is: %d)\n", cnt);
			if (curr == 0 && cnt > 1)
			{
				printf("current: %s(%d)\tnext: %s(%d)\n", stack[curr].name, curr,stack[curr + 1].name,curr+1);
			}
			else if ((curr == 0 && cnt == 1) || (curr == cnt - 1 && cnt == 1))
			{
				printf("current: %s(%d)\n", stack[curr].name,curr);
			}
			else if (curr == cnt - 1 && cnt > 1)
			{
				printf("previous: %s(%d)\tcurrent: %s(%d)\n", stack[curr-1].name, curr-1, stack[curr].name, curr);
			}
			else if (cnt > 3)
			{
				printf("previous: %s(%d)\tcurrent: %s(%d)\tnext: %s(%d)\n", stack[curr - 1].name, curr-1, stack[curr].name, curr, stack[curr+1].name, curr+1);
			}

		rechange:
			temp = curr;
			printf("\ninput(-1 for cancel): ");
			scanf("%d", &curr);
			getchar();
			if (curr ==  -1)
				curr = temp;
			else if (curr > cnt - 1)
			{
				goto rechange;
			}
			break;

		}
		case 'h':
		{
			insert(&stack);
			curr = cnt - 1;
			break;
		}
		case 'b':
		{
			int dindex;
			printf("\ncount of class: %d\n", cnt);
			deleteClass:
			printf("choose index(-1 for cancel): ");
			scanf("%d", &dindex);
			getchar();
			if (dindex > cnt - 1)
				goto deleteClass;
			if (dindex != -1)
			{
				dClass(stack, dindex);
				curr = cnt - 1;
			}
			break;
		}
		case 'n':
		{
			printf("\n[ ");
			for (i = 0; i < cnt; i++)
			{
				printf("%s ", stack[i].name);
			}
			puts("]");
			puts("press any key for next move");
			_getch();
			break;
		}
		case 'p':
			iter = 0;
		default:
			changed = 'X';
			break;
		}
		system("cls");
	}
#pragma endregion

#pragma region calculate total
	puts("\ncalculating total\n");
	info total;
	strcpy(total.name, "Total");
	total.chosen = 0;
	total.unchosen = 0;
	total.useless = 0;
	for (i = 0; i < cnt; i++)
	{
		total.chosen += stack[i].chosen;
		total.unchosen += stack[i].unchosen;
		total.useless += stack[i].useless;
	}
#pragma endregion

#pragma region store
	puts("\nstoring\n");

	file = fopen(path, "w+t");
	fprintf(file, "[-----Total-----]\n");
	fprintf(file, "chosen: %d\n",total.chosen);
	fprintf(file, "unchosen: %d\n", total.unchosen);
	fprintf(file, "useless: %d\n", total.useless);
	fprintf(file, "(chosen) ratio: %f%%\n", 100 * ((float)total.chosen / (float)(total.chosen + total.unchosen + total.useless)));
	fprintf(file, "\n");

	for (i = 0; i < cnt; i++) {
		fprintf(file, "class: %s\n",stack[i].name);
		fprintf(file, "chosen: %d\n", stack[i].chosen);
		fprintf(file, "unchosen: %d\n", stack[i].unchosen);
		fprintf(file, "useless: %d\n", stack[i].useless);
		fprintf(file, "(chosen) ratio: %f%%\n", 100 * ((float)stack[i].chosen / (float)(stack[i].chosen + stack[i].unchosen + stack[i].useless)));
		fprintf(file, "\n");
	}
	fclose(file);

#pragma endregion


	puts("\nquit program");

	free(stack);

	return 0;
}

void init(info* dir)
{
	char buff[32];
	printf("\nwhat is the name?: ");
	scanf_s("%s", buff, 32);
	getchar();
	strcpy(dir->name, buff);
	dir->chosen = 0;
	dir->unchosen = 0;
	dir->useless = 0;
	cnt++;
	system("cls");
}

void dClass(info* dir, int dindex)
{
	int i;
	for (i = dindex; i < cnt-1; i++)
	{
		dir[i] = dir[i + 1];
	}
	cnt--;
}

void insert(info** dir)
{
	if (cnt >= size)
	{
		size += 10;
		*dir = (info*)realloc(*dir, sizeof(info) * size);
	}
	init(&(*dir)[cnt]);
}
