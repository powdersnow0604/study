#include "filesys.h"
#include <string.h>
#include <stdio.h>
#include "Utils.h"


//variables

CURRENT_DIR curr_dir_path;

i_node* curr_dir;

i_node* root_dir;

const size_t PATH_MAX = 260;

const size_t init_dir_member_num = 10;


//definition
void init_filesystem(void)
{
	strcpy_s(curr_dir_path, PATH_MAX, "/");
	root_dir = c_AT(*c_AT(root_directory_addr_ptr, MEMBLOCK*), i_node*);
	curr_dir = root_dir;
}

void dirInsert(i_node* parent_dir, MEMBLOCK child_node, int type, MEMBLOCK name, size_t num, size_t capacity, MEMBLOCK addr)
{
	if (parent_dir->num == parent_dir->capacity) {
		parent_dir->capacity *= 2;
		MEMBLOCK naddr = mmtInsert(parent_dir->capacity * sizeof(MEMBLOCK), 1);
		memcpy(AT(naddr), AT(parent_dir->addr), sizeof(MEMBLOCK) * parent_dir->num);
		parent_dir->addr = naddr;
	}

	*(c_AT(parent_dir->addr, MEMBLOCK*) + parent_dir->num) = child_node;
	i_node* nnode = c_AT(child_node, i_node*);
	nnode->type = type;
	nnode->name = name;
	nnode->num = num;
	nnode->capacity = capacity;
	nnode->addr = addr;

	++(parent_dir->num);
}

i_node* dirSearch(i_node* dir, MEMBLOCK name)
{
	MEMBLOCK* nptr = c_AT(dir->addr, MEMBLOCK*);
	for (size_t i = 0; i < dir->num; ++i, ++nptr) {
		if (!strcmp(AT(name), AT(c_AT(*nptr, i_node*)->name))) return c_AT(*nptr, i_node*);
	}

	return NULL;
}

i_node* dirSearchWithChar(i_node* dir, const char* name)
{
	MEMBLOCK* nptr = c_AT(dir->addr, MEMBLOCK*);
	for (size_t i = 0; i < dir->num; ++i, ++nptr) {
		if (!strcmp(name, AT(c_AT(*nptr, i_node*)->name))) return c_AT(*nptr, i_node*);
	}

	return NULL;
}


//instructions
void cat(const char* arg)
{
	const char* saveptr = arg;
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

			strcpy_s(AT(str_addr), strlen(buf) + 1, buf);
			strcpy_s(AT(name_addr), strlen(token) + 1, token);

			dirInsert(curr_dir, node_addr, 1, name_addr, 0, 0, str_addr);

			break;
		}
		else {
			i_node* target = dirSearchWithChar(curr_dir, token);
			if ((target != NULL) && (!strcmp(c_AT(target->name, char*), token))) {
				printf("%s\n", c_AT(target->addr, char*));
			}
			else {
				printf("'%s' is not a filename\n", token);
			}

			break;
		}
	}
}

void ls(void)
{
	MEMBLOCK* nptr = c_AT(curr_dir->addr, MEMBLOCK*);
	for (size_t i = 0; i < curr_dir->num; ++i, ++nptr) {
		printf("%s ", AT(c_AT(*nptr, i_node*)->name));
	}

	puts("");
}
