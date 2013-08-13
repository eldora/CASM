#include <stdio.h>
#include "hardware.h"
#include "parsing.h"

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Using: %s Assembly_File_Name\n", argv[0]);
		return -1;
	}

	asm2bin(argv[1]);
	bin2com();

	printMemory();
	printCPU();

	//printf("0x%X, 0x%X\n", &(MEM.code[0]), &(MEM.code[1]));
	return 0;
}
