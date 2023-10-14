#ifndef __FILESYS_H__
#define __FILESYS_H__


//include 
#include "auxiliary_mem.h"


//typedef

//[[deprecated]] dir node
/*
typedef struct struct_dir dir_node;

typedef struct struct_dir {

	// type 0: dir	1: file
	int type;
	MEMBLOCK name;
	size_t size;
	MEMBLOCK addr;
}dir_node;
*/


typedef struct {
	MEMBLOCK name;
	size_t num;		//for dir, total number of members
	size_t capacity;	 
	MEMBLOCK addr;		//for file, address of data / for dir, address of array of  file/dir address
	int type; // type 0: dir	1: file
}i_node;



typedef char CURRENT_DIR[512];


//variables
extern CURRENT_DIR curr_dir_path;

extern i_node* curr_dir;

extern i_node* root_dir;

extern const size_t PATH_MAX;

extern const size_t init_dir_member_num;


//functions
void init_filesystem(void);

void dirInsert(i_node* parent_dir, MEMBLOCK child_node, int type, MEMBLOCK name, size_t num, size_t capacity, MEMBLOCK addr);

i_node* dirSearch(i_node* dir, MEMBLOCK name);

i_node* dirSearchWithChar(i_node* dir, const char* name);


//instructions
void cat(const char* arg);

void ls(void);
#endif
