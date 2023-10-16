#include "filesys.h"
#include <string.h>
#include <stdio.h>
#include "Utils.h"


//variables

CURRENT_DIR_PATH curr_dir_path;

i_node* curr_dir;

i_node* curr_parent_dir;

i_node* root_dir;

const size_t PATH_MAX = 260;

const size_t init_dir_member_num = 10;


//definition
void init_filesystem(void)
{
	strcpy_s(curr_dir_path, PATH_MAX, "/");
	root_dir = c_AT(*c_AT(root_directory_addr_ptr, MEMBLOCK*), i_node*);
	curr_dir = root_dir;
	curr_parent_dir = NULL;
}

void dirInsert(i_node* parent_dir, MEMBLOCK child_node, int type, MEMBLOCK name, size_t num, size_t capacity, MEMBLOCK addr)
{
	if (parent_dir->num == parent_dir->capacity) {
		parent_dir->capacity *= 2;
		MEMBLOCK naddr = mmtInsert(parent_dir->capacity * sizeof(MEMBLOCK), 1);
		memcpy(AT(naddr), AT(parent_dir->addr), sizeof(MEMBLOCK) * parent_dir->num);
		mmtDelete(parent_dir->addr, parent_dir->capacity / 2 * sizeof(MEMBLOCK));
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

void dirDelete(i_node* parent_dir, MEMBLOCK child_node)
{
	//just delete from dir addr array, doesn't care about deleted i_node's data/members/etc...
	//to delete file/dir completely, please use this function with rm and rmdir
	size_t i = 0;
	MEMBLOCK* addr_arr = c_AT(parent_dir->addr, MEMBLOCK*);
	for (i = 0; i < parent_dir->num; ++i) {
		if (addr_arr[i] == child_node) break;
	}

	for (size_t j = i; j < parent_dir->num - 1; ++j) {
		addr_arr[i] = addr_arr[i + 1];
	}

	--(parent_dir->num);
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

i_node* dirSearchWithPath(i_node* dir, char* path)
{
	char* saveptr = path;
	char* backup = saveptr;
	const char* token;
	i_node* temp_dir = dir;

	if (path[0] == '/' || dir == NULL) temp_dir = root_dir;

	while (token = strtok_s(NULL, "/", &saveptr)) {
		temp_dir = dirSearchWithChar(temp_dir, token);

		if (temp_dir == NULL) return NULL;
	}

	return temp_dir;
}


//instructions
void cat(char* arg)
{
	char* saveptr = arg;
	char* token;
	char buf[256] = "";

	while (token = strtok_s(NULL, " ", &saveptr))
	{
		if (!strcmp(token, ">")) {
			token = strtok_s(NULL, "\n ", &saveptr);

			if (dirSearchWithChar(curr_dir, token) != NULL) {
				puts("same name is already exist");
				return;
			}

			scanf_s("%[^\n]", buf, (unsigned int)sizeof(buf));

			MEMBLOCK node_addr = mmtInsert(sizeof(i_node), 1);
			MEMBLOCK str_addr = mmtInsert(strlen(buf) + 1, 2);
			MEMBLOCK name_addr = mmtInsert(strlen(token) + 1, 2);

			strcpy_s(AT(str_addr), strlen(buf) + 1, buf);
			strcpy_s(AT(name_addr), strlen(token) + 1, token);

			dirInsert(curr_dir, node_addr, 1, name_addr, strlen(buf)+1, 0, str_addr);

			break;
		}
		else {
			i_node* target = dirSearchWithChar(curr_dir, token);
			if ((target != NULL) && (!strcmp(c_AT(target->name, char*), token)) && (target->type == 1)) {
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

void mkdir(const char* arg)
{
	MEMBLOCK node_addr = mmtInsert(sizeof(i_node), 1);
	MEMBLOCK name_addr = mmtInsert(strlen(arg) + 1, 2);
	MEMBLOCK node_mem_addr_addr = mmtInsert(init_dir_member_num * sizeof(MEMBLOCK), 1);

	strcpy_s(AT(name_addr), strlen(arg) + 1, arg);

	dirInsert(curr_dir, node_addr, 0, name_addr, 0, init_dir_member_num, node_mem_addr_addr);
}

void cd(char* arg)
{
	char* saveptr = arg;
	const char* token;
	i_node* dir_temp = curr_dir;
	i_node* target;

	while (token = strtok_s(NULL, "/", &saveptr)) {
		if (!strcmp(token, "..")) {
			//수정 필요 -> curr_parent_dir의 부모 디렉토리는?
			//현재 두번 사용 못함
			if (curr_parent_dir == NULL) continue;

			dir_temp = curr_parent_dir;

			if (dir_temp == root_dir) {
				*(strrchr(curr_dir_path, '/')+1) = '\0';
				curr_parent_dir = NULL;
			}
			else {
				*strrchr(curr_dir_path, '/') = '\0';

				char* path_temp = _strdup(curr_dir_path);
				*strrchr(path_temp, '/') = '\0';
				curr_parent_dir = dirSearchWithPath(NULL, path_temp);
				free(path_temp);
			}
		}
		else {
			target = dirSearchWithChar(dir_temp, token);

			if (target == NULL) {
				puts("path does not exist");
				return;
			}

			if (strcmp(curr_dir_path, "/")) {
				strcat_s(curr_dir_path, PATH_MAX, "/");
			}
			strcat_s(curr_dir_path, PATH_MAX, token);

			curr_parent_dir = dir_temp;
			dir_temp = target;
		}
	}

	curr_dir = dir_temp;
}

void rm(i_node* dir, const char* arg)
{
	i_node* target = dirSearchWithChar(dir, arg);

	if (target == NULL) {
		printf("\"%s\" is not a correct file name\n",arg);
		return;
	}
	if (target->type != 1) {
		printf("\"%s\" is not a file, but a  directory\n", arg);
		return;
	}

	dirDelete(dir, (MEMBLOCK)(target - auxmem));
	mmtDelete(target->addr, target->num);
	mmtDelete(target->name, strlen(AT(target->name)) + 1);
}

void rmdir(i_node* dir, const char* arg)
{
	i_node* target = dirSearchWithChar(dir, arg);

	if (target == NULL) {
		printf("\"%s\" is not a correct directory name\n", arg);
		return;
	}
	if (target->type != 0) {
		printf("\"%s\" is not a directory, but a file\n", arg);
		return;
	}

	for (size_t i = 0; i < target->num; ++i) {
		i_node* temp = c_DAT(target->addr + i * sizeof(MEMBLOCK), MEMBLOCK*, i_node*);
		if (temp->type == 1) rm(target, AT(temp->name));
		else if (temp->type == 0) rmdir(target, AT(temp->name));
	}

	dirDelete(dir, (MEMBLOCK)(target - auxmem));
	mmtDelete(target->name, strlen(AT(target->name)) + 1);
}