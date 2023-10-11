#include "filesys.h"
#include <string.h>
#include <stdio.h>

//variables

CURRENT_DIR curr_dir;

i_node* root_dir;

const size_t PATH_MAX = 512;

//definition
void init_filesystem(void)
{
	strcpy_s(curr_dir, PATH_MAX, "/");
	root_dir = (i_node*)(auxmem + *((size_t*)(auxmem + root_directory_addr_ptr)));
}

void cat(const char* arg)
{
	char* saveptr = arg;
	char* token;
	char buf[256] = "";

	while (token = strtok_s(NULL, " ", &saveptr))
	{
		if (!strcmp(token, ">")) {
			token = strtok_s(NULL, "\n ", &saveptr);

			scanf_s("%[^\n]", buf, (unsigned int)sizeof(buf));

			MEMBLOCK node_addr = mmtInsert(sizeof(i_node), 1);
			MEMBLOCK str_addr = mmtInsert(strlen(buf) + 1, 2);
			MEMBLOCK name_addr = mmtInsert(strlen(token) + 1, 2);

			strcpy_s(auxmem + str_addr, strlen(buf)+1, buf);
			strcpy_s(auxmem + name_addr, strlen(token)+1, token);

			i_node* nnode = (i_node*)(auxmem + node_addr);
			nnode->addr = str_addr;
			nnode->size = strlen(buf) + 1;
			nnode->type = 1;
			nnode->name = name_addr;

			root_dir->addr = node_addr;

			break;
		}
		else {
			i_node* fnode = (i_node*)(auxmem + root_dir->addr);
			char* name = (char*)(auxmem + fnode->name);
			char* str = (char*)(auxmem + fnode->addr);
			if (!strcmp(name, token)) {
				printf("%s\n", str);
			}
			else{
				printf("'%s' is not a filename\n", token);
			}

			break;
		}
	}
}
