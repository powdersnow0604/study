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
	char* curr_path = curr_dir_path;
	char opcode[32] = "\0"; //��ɾ� 32 byte����
	char argbuf[512] = ""; //���� 512 byte����
	while (1)
	{
		printf("sangmyeong@soomoong ~%s $ ", curr_path);
		ret = scanf_s("%s%*c", opcode, 32);

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
		else if (!strcmp(opcode, "mkdir")) {
			//����ó�� �ʿ�
			scanf_s("%[^\n]", argbuf, (unsigned int)sizeof(argbuf));
			mkdir(argbuf);
		}
		else if (!strcmp(opcode, "cd")) {
			//����ó�� �ʿ�
			scanf_s("%[^\n]", argbuf, (unsigned int)sizeof(argbuf));
			cd(argbuf);
		}
		else if (!strcmp(opcode, "rm")) {
			//����ó�� �ʿ�
			scanf_s("%[^\n]", argbuf, (unsigned int)sizeof(argbuf));
			rm(curr_dir, argbuf);
		}
		else if (!strcmp(opcode, "rmdir")) {
			//����ó�� �ʿ�
			scanf_s("%[^\n]", argbuf, (unsigned int)sizeof(argbuf));
			rmdir(curr_dir, argbuf);
		}
		else {
			printf("check whether '%s' is a correct instruction\n", opcode);
		}

		argbuf[0] = '\0';
		puts("");
		rewind(stdin);
	}
}
