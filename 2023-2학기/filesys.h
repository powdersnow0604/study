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
	int type; // type 0: dir	1: file
	MEMBLOCK name;
	size_t size;
	MEMBLOCK addr;
}i_node;



typedef char CURRENT_DIR[512];


//variables
extern CURRENT_DIR curr_dir;

extern i_node* root_dir;

extern const size_t PATH_MAX;

//functions
void init_filesystem(void);

void cat(const char* arg);
#endif
