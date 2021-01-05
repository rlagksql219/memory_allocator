#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "ealloc.h"


typedef struct {
	char* ptr;
	int size;
} Block;

char* start_ptr1 = NULL;
char* start_ptr2 = NULL;
char* start_ptr3 = NULL;
char* start_ptr4 = NULL;

Block block1[16];
Block block2[16];
Block block3[16];
Block block4[16];

int alloc_mark1 = 0;
int alloc_mark2 = 0;
int alloc_mark3 = 0;
int alloc_mark4 = 0;


void init_alloc(void) 
{
	for(int i=0; i<16; i++) {
		block1[i].ptr = NULL;
		block2[i].ptr = NULL;
		block3[i].ptr = NULL;
		block4[i].ptr = NULL;

		block1[i].size = 0;
		block2[i].size = 0;
		block3[i].size = 0;
		block4[i].size = 0;
	}
}


char* alloc(int size)
{
	int i1 = 0;
	int i2 = 0;
	int i3 = 0;
	int i4 = 0;

	if(size % 256 != 0) {
		return NULL;
	}

	if(size > 4096) {
		return NULL;
	}


	if(alloc_mark1 == 0) {
		if ((start_ptr1 = mmap(NULL, sizeof(PAGESIZE), PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
			return NULL;
		}
		alloc_mark1 = 1;

	}
	for(int count=0; count<16; count++) {
		if(block1[i1].size == 0) {
			break;
		}
		i1++;
	}

	if(i1>=16 && alloc_mark2 == 0) {
		if ((start_ptr2 = mmap(NULL, sizeof(PAGESIZE), PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
			return NULL;
		}
		alloc_mark2 = 1;
	}
	for(int count=0; count<16; count++) {
		if(block2[i2].size == 0) {
			break;
		}
		i2++;
	}

	if(i2>=16 && alloc_mark3 == 0) {
		if ((start_ptr3 = mmap(NULL, sizeof(PAGESIZE), PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
			return NULL;
		}
		alloc_mark3 = 1;
	}
	for(int count=0; count<16; count++) {
		if(block3[i3].size == 0) {
			break;
		}
		i3++;
	}

	if(i3>=16 && alloc_mark4 == 0) {
		if ((start_ptr4 = mmap(NULL, sizeof(PAGESIZE), PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
			return NULL;
		}
		alloc_mark4 = 1;
	}
	for(int count=0; count<16; count++) {
		if(block4[i4].size == 0) {
			break;
		}
		i4++;
	}


//	Debug	
//	printf("i1 i2 i3 i4 %d %d %d %d\n", i1, i2, i3, i4);

	if(i1>=0 && i1<16) {
		for (int j = 0; i1 < size / 256; j++) {
			block1[i1].ptr = start_ptr1 + 256 * i1;
			block1[i1].size = size;
			i1++;
		}

		return block1[i1 - (size / 256)].ptr;
	}
	else if (i1 == 16 && i2 >= 0 && i2 < 16) {
		for (int j = 0; i2 < size / 256; j++) {
			block2[i2].ptr = start_ptr2 + 256 * i2;
			block2[i2].size = size;
			i2++;
		}

		return block2[i2 - (size / 256)].ptr;
	}
	else if (i1 == 16 && i2 == 16 && i3 >= 0 && i3 < 16) {
		for (int j = 0; i3 < size / 256; j++) {
			block3[i3].ptr = start_ptr3 + 256 * i3;
			block3[i3].size = size;
			i3++;
		}

		return block3[i3 - (size / 256)].ptr;
	}
	else {
		if (i1 == 16 && i2 == 16 && i3 == 16 && i4 >= 0 && i4 < 16) {
			for (int j = 0; i4 < size / 256; j++) {
				block4[i4].ptr = start_ptr4 + 256 * i4;
				block4[i4].size = size;
				i4++;
			}

			return block4[i4 - (size / 256)].ptr;
		}
	}
}


void dealloc(char* ptr)
{
	int page_num = 0;
	int i = 0;
	int iterate_num = 0;

	for (int j = 0; j < 16; j++) {
		if (ptr == (start_ptr1 + 4 * j)) {
			page_num = 1;
			i = (int)((ptr - start_ptr1) / 256);
			iterate_num = (block1[i].size) / 256;

			for (int k = 0; k < iterate_num; k++) {
				block1[i].ptr = NULL;
				block1[i].size = 0;
				i++;
			}
		}

		else if (ptr == (start_ptr2 + 4 * j)) {
			page_num = 2;
			i = (int)((ptr - start_ptr2) / 256);
			iterate_num = (block2[i].size) / 256;

			for (int k = 0; k < iterate_num; k++) {
				block2[i].ptr = NULL;
				block2[i].size = 0;
				i++;
			}
		}

		else if (ptr == (start_ptr3 + 4 * j)) {
			page_num = 3;
			i = (int)((ptr - start_ptr3) / 256);
			iterate_num = (block3[i].size) / 256;

			for (int k = 0; k < iterate_num; k++) {
				block3[i].ptr = NULL;
				block3[i].size = 0;
				i++;
			}
		}

		else {
			page_num = 4;
			i = (int)((ptr - start_ptr4) / 256);
			iterate_num = (block4[i].size) / 256;

			for (int k = 0; i < iterate_num; k++) {
				block4[i].ptr = NULL;
				block4[i].size = 0;
				i++;
			}
		}
	}

	ptr[0] = '\0';
}


void cleanup(void)
{
	start_ptr1 = NULL;
	start_ptr2 = NULL;
	start_ptr3 = NULL;
	start_ptr4 = NULL;

	alloc_mark1 = 0;
	alloc_mark2 = 0;
	alloc_mark3 = 0;
	alloc_mark4 = 0;
}
