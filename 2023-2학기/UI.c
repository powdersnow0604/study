#include "UI.h"
#include <stdio.h>
#include <string.h>


//variables
const char* root_user_name;

const char* host_user_name;

const char* password;


//definitions
int mainloop(void)
{
	int ret = 0;
	char* curr_path;
	char opcode[32] = "\0"; //명령어 32 byte까지
	char argbuf[512] = ""; //명령어 512 byte까지
	while (1)
	{
		if (!strcmp(curr_dir_path, "/")) curr_path = "~";
		else curr_path = curr_dir_path;

		printf("sangmyeong@soomoong %s $ ", curr_path);
		ret = scanf_s("%s", opcode, 32);

		//case: opcode buffer overflow
		if (!(ret == 1)) {
			puts("instruction is too long");
			rewind(stdin);
			puts("");
			continue;
		}

		//check instructions
		if (!strcmp(opcode, "cat")) {
			scanf_s("%[^\n]", argbuf, (unsigned int)sizeof(argbuf));
			rewind(stdin);


			if (strlen(argbuf) == 0) {
				printf("'%s' need arguments\n\n", opcode);
				continue;
			}
			
			cat(argbuf);
		}
		else if (!strcmp(opcode, "ls")) {
			ls();
		}
		else if(!strcmp(opcode, "exit")) {
			return 0;
		}
		else {
			printf("check whether '%s' is a correct instruction\n", opcode);
		}

		argbuf[0] = '\0';
		puts("");
		rewind(stdin);
	}
}
