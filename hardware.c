#include "hardware.h"

/*** 하드웨어 구조체 전역변수 초기화 ***/
struct CLU_STRUCT CLU = {0,};
struct REG_STRUCT REG[BIT_TYPE] = {
	{0, "r0"},	{0, "r1"},	{0, "r2"},	{0, "r3"},
	{0, "r4"},	{0, "r5"},	{0, "r6"},	{0, "r7"},
	{0, "r8"},	{0, "r9"},	{0, "r10"},	{0, "r11"},
	{0, "r12"},	{0, "r13"},	{0, "r14"},	{0, "r15"},
};
struct OPSTRUCT OPTABLE[BIT_TYPE] = {
	{"ADD", ADD, NULL}, {"SUB", SUB, NULL},
	{"MOV", MOV, NULL}, {"AND", AND, NULL},
	{"ORR", ORR, NULL}, {"CMP", CMP, NULL},
	{"STR", STR, NULL}, {"LDR", LDR, NULL}, 
	{"B"	, B  , NULL}, {"BL"	, BL , NULL},
	{"IRET", IRET, NULL},		
	{"PUSH", PUSH, NULL}, {"POP", POP, NULL},
};
struct MEM_STRUCT MEM = {0,};

/*** 하드웨어 관련 함수 ***/
void printMemory(){
	int i;
	printf("===CODE SECTION===\n");
	for(i=0; i<MEM.code_lastIndex; i++)
		printf("%x\n", MEM.code[i]);

	printf("===DATA SECTION===\n");
	for(i=0; i<MEM.data_lastIndex; i++)
		printf("%x\n", MEM.data[i]);
}
