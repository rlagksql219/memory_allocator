#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "alloc.h"


typedef struct {
	char* ptr;
	int size;
} Block;

char* start_ptr = NULL;
Block block[512];


int init_alloc()
{
	if ((start_ptr = mmap(NULL, sizeof(PAGESIZE), PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		return 1;
	}
	else {
		for(int i=0; i<512; i++) {
			block[i].ptr = NULL;
			block[i].size = 0;
		}
		block[0].ptr = start_ptr;
	}

	return 0;
}


int cleanup()
{

//	printf("\n\n\n\n");
//	printf("VSZ check\n\n");
//	printf("Before munmap\n");
//	system("ps -u");


	if(munmap(start_ptr, PAGESIZE) == -1) {
		return 1;
	}

	start_ptr = NULL;


//	printf("\n\n\n\n");
//	printf("After munmap\n");
//	system("ps -u");

	return 0;
}


char* alloc(int size)
{
	int i = 0;
	
	if(size % 8 != 0) {
		return NULL;
	}


	while(1) {
		if(block[i].size == 0) {
			break;
		}
		i++;
	}

	for(int j=0; j<size/8; j++) {
		block[i].ptr = start_ptr + 8*i;
		block[i].size = size;
		i++;
	}

	if(i > 512) {
		return NULL;
	}


//	Debug
//	printf("alloc\n");
//	printf("i %d\n", i);
//	printf("%p\n", block[i-(size/8)].ptr);
//	printf("%c\n", *(block[i-(size/8)].ptr));

	return block[i-(size/8)].ptr;
}


void dealloc(char* ptr)
{
	int i = 0;
	int iterate_num = 0;

	i = (int)((ptr-start_ptr)/8);
	iterate_num = (block[i].size)/8;

//	Debug	
//	printf("dealloc\n");
//	printf("i %d\n", i);


	for(int j=0; j<iterate_num; j++) {
		block[i].ptr = NULL;
		block[i].size = 0;
		i++;
	}

	ptr[0] = '\0';
}
