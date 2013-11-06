#include <stdio.h>
#include "hardware.h"
#include "parsing.h"
#include "core.h"

int main(int argc, char *argv[]){
	int i;

	if(argc<2){
		printf("Using: %s Assembly_File_Name\n", argv[0]);
		return -1;
	}

	for(i=1; i<argc; i++)
		asm2bin(argv[i]);

	runCPU();
	printMemMap();
	printMemory();
	printCPU();

	//printf("0x%X, 0x%X\n", &(MEM.code[0]), &(MEM.code[1]));
	return 0;
}
