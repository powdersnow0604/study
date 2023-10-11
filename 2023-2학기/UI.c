#include "UI.h"
#include <stdio.h>
#include <string.h>

//definitions
int mainloop(void)
{
	int ret = 0;
	char* curr_path;
	char opcode[32] = "\0";
	char argbuf[512] = "";
	while (1)
	{
		if (!strcmp(curr_dir, "/")) curr_path = "~";
		else curr_path = curr_dir;

		printf("sangmyeong@soomoong %s $ ", curr_path);
		ret = scanf_s("%s", opcode, 32);

		if (!(ret == 1)) {
			puts("instruction is too long");
			rewind(stdin);
			puts("");
			continue;
		}

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

		}
		else if(!strcmp(opcode, "exit")) {
			return 0;
		}
		else {
			printf("check whether '%s' is a correct instruction\n", opcode);
		}

		puts("");
		rewind(stdin);
	}
}
