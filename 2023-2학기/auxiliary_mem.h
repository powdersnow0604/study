#ifndef __AUXILIARY_MEM_h__
#define __AUXILIARY_MEM_h__

/*								summary								*/	
// auxmem_size: < 1/4: kernel,	1/4 <=  < 2/4: filesystem,	2/4 <= <= 4/4: global
// addr is  8 bytes
//kernel
//	0 ~ 7: root_directory addr
//	8 ~ 15: memory management table address




///////////////////////////////////////////////////////////////////////

//include 
#include <stdlib.h>

//typedef
typedef char* AUXMEM;

typedef size_t MEMBLOCK;

typedef struct {
	MEMBLOCK s;
	MEMBLOCK e;
}mmt_node;

typedef struct {
	size_t node_num;
	size_t capacity;
	mmt_node* nodes;
}MMT;

//variables
extern AUXMEM auxmem;

extern const char* auxmem_path;

extern MMT mmt;

extern const size_t auxmem_size;

extern const size_t init_mmt_node_num;

extern size_t directory_size;

extern const MEMBLOCK root_directory_addr_ptr; //where root_directory addr place

//functions
int loadAuxmem();

int changeAuxmemPath(const char* path);

MEMBLOCK mmtInsert(size_t size, int mode);

#endif