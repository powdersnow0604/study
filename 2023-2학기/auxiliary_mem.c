#include "auxiliary_mem.h"
#include <stdio.h>
#include <string.h>
#include "Utils.h"

//variables
AUXMEM auxmem;

MMT mmt;

const char* auxmem_path = "C:\\Users\\darak\\OneDrive\\Desktop\\C Derived\\c\\data";

const size_t auxmem_size = 8000000;

const size_t init_mmt_node_num = 10;

size_t directory_size = sizeof(size_t[4]) + sizeof(int);

const MEMBLOCK root_directory_addr_ptr = 0llu; //where root_directory addr place


//declaration
static void init_mem();

//definition
int loadAuxmem()
{
	FILE* file;
	fopen_s(&file, auxmem_path, "rb");

	if (file == NULL) {
		(char*)auxmem = (char*)malloc(auxmem_size);
		init_mem();
		return 1;
	}
	else {
		fseek(file, 0L, SEEK_END);

		if (ftell(file) != auxmem_size) {
			(char*)auxmem = (char*)malloc(auxmem_size);
			init_mem();
			fclose(file);
			return 1;
		}
		else { 
			fread((void*)auxmem, 1, auxmem_size, file);
			fclose(file);
			return 0;
		}
	}
}

void storeAuxmem(void)
{
	FILE* file;
	fopen_s(&file, auxmem_path, "wb");
	if (file == NULL) return;

	fwrite(auxmem, 1, auxmem_size, file);

	fclose(file);
}

int changeAuxmemPath(const char* path)
{
	auxmem_path = path;
	return 0;
}

void init_mem()
{
	// initialize mmt
	mmt.capacity = init_mmt_node_num;

	mmt.nodes = (mmt_node*)malloc(mmt.capacity * sizeof(mmt_node));
	if (mmt.nodes == NULL) return;

	//for kernel
	mmt.nodes[0].s = 16;
	mmt.nodes[0].e = 16;

	//for file system
	mmt.nodes[1].s = auxmem_size / 4;
	mmt.nodes[1].e = auxmem_size / 4;

	//global
	mmt.nodes[2].s = auxmem_size  / 2;
	mmt.nodes[2].e = auxmem_size  / 2;

	mmt.node_num = 3;

	MEMBLOCK root_directory_addr = mmtInsert(directory_size, 1);

	MEMBLOCK root_name_addr = mmtInsert(sizeof("root"), 2);

	MEMBLOCK root_dir_node_ptr_array_addr = mmtInsert(10 * sizeof(MEMBLOCK), 1);

	strcpy_s(auxmem + root_name_addr,sizeof("root"), "root");
	
	*c_AT(root_directory_addr_ptr,MEMBLOCK*) = root_directory_addr;


	*c_AT(root_directory_addr, MEMBLOCK*) = root_name_addr; //root dir name ptr
	*c_AT(root_directory_addr + sizeof(MEMBLOCK), MEMBLOCK*) = 0llu; //root dir num
	*c_AT(root_directory_addr + sizeof(MEMBLOCK) * 2, MEMBLOCK*) = 10llu; //root dir capacity
	*c_AT(root_directory_addr + sizeof(MEMBLOCK) * 3, MEMBLOCK*) = root_dir_node_ptr_array_addr; //root dir node ptr
	*c_AT(root_directory_addr + sizeof(MEMBLOCK) * 4, int*) = 0; //root dir type == dir
}

MEMBLOCK mmtInsert(size_t size, int mode)
{
	//mode	 0: kernel	1: file system	else: global

	if (mmt.nodes == NULL) return 0; //not initialized

	if (mmt.capacity == mmt.node_num) {
		mmt.capacity *= 2;
		mmt.nodes = (mmt_node*)realloc(mmt.nodes, mmt.capacity);
		if (mmt.nodes == NULL) return 0;
	}

	size_t i, temp = auxmem_size * 2 / 4;
	
	switch (mode)
	{
	case 0:
		for (i = 0; i < mmt.node_num - 1; ++i) {
			if (((mmt.nodes[i].e+size) < auxmem_size / 4) && ((mmt.nodes[i + 1].s - mmt.nodes[i].e - 1) >= size)) {
				temp = mmt.nodes[i].e + 1;
				mmt.nodes[i].e += size;
				return temp;
			}
		}

		if ((mmt.nodes[mmt.node_num - 1].e + size) < auxmem_size / 4) {
			temp = mmt.nodes[mmt.node_num - 1].e + 1;
			mmt.nodes[mmt.node_num - 1].e += size;
		}

		return temp;

	case 1:
		for (i = 0; i < mmt.node_num - 1; ++i) {
			if (((mmt.nodes[i].e + 1) >= auxmem_size / 4) &&((mmt.nodes[i].e + size) < auxmem_size * 2 / 4) && ((mmt.nodes[i + 1].s - mmt.nodes[i].e - 1) >= size)) {
				temp = mmt.nodes[i].e + 1;
				mmt.nodes[i].e += size;
				return temp;
			}
		}

		if ((mmt.nodes[mmt.node_num - 1].e + size) < auxmem_size * 2 / 4) {
			temp = mmt.nodes[mmt.node_num - 1].e + 1;
			mmt.nodes[mmt.node_num - 1].e += size;
		}

		return temp;
	default:
		for (i = 0; i < mmt.node_num - 1; ++i) {
			if (((mmt.nodes[i].e + 1) >= auxmem_size * 2 / 4) && ((mmt.nodes[i].e + size) <= auxmem_size) && ((mmt.nodes[i + 1].s - mmt.nodes[i].e - 1) >= size)) {
				temp = mmt.nodes[i].e + 1;
				mmt.nodes[i].e += size;
				return temp;
			}
		}

		if ((mmt.nodes[mmt.node_num - 1].e + size) < auxmem_size) {
			temp = mmt.nodes[mmt.node_num - 1].e + 1;
			mmt.nodes[mmt.node_num - 1].e += size;
		}

		return temp;
	}
	
}

int mmtDelete(MEMBLOCK start, size_t size)
{
	size_t i;
	for (i = 0; i < mmt.node_num; ++i) {
		if (mmt.nodes[i].s <= start && start <= mmt.nodes[i].e) {
			if (mmt.nodes[i].s == start && start == mmt.nodes[i].e) {
				
			}

			if (mmt.capacity == mmt.node_num) {
				mmt.capacity *= 2;
				mmt.nodes = (mmt_node*)realloc(mmt.nodes, mmt.capacity);
				if (mmt.nodes == NULL) return 0;
			}


		}
	}

	return -1;
}


