#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <stdio.h>

/*	Hardware Abstruct(추상화)
 *
 *	## List ##
 *	1. CPU
 *		1) CLU
 *		2) Register
 *		3) Instruction Set
 *	2. Memory
 *
 */

/*	OPCODE SYSTEM
 *
 *	## ADD, SUB
 *	Opcode	Rn		Rd		Operand2
 *	4bit		4bit	4bit	4bit
 *
 *	## MOV, AND, ORR, CMP
 *	Opcode	Rn		F			Operand2	
 *	4bit		4bit	2bit	6bit
 *
 *	## STR, LDR
 *	? 이때 Operand2가 값인지 레지스터인지 어떻게 판별하지
 *	Opcode	Rn		Rd		Operand2(REG)
 *	4bit		4bit	4bit	4bit
 *
 *	## B, BL
 *	Opcode	immed_12
 *	4bit		12bit
 *
 *	## PUSH, POP
 *	Opcode	Operand2(REG)
 *	4bit		12bit(using 0x00F)
 *
 *	F에 따른 Operand2의 변화
 *	switch(F){
 *		case 0: operand2 = register(6bit) // 레지스터 방식
 *		case 1: operand2 = rotate-imm(2bit), mm(4bit) // mm * 2^2imm
 *		//case1의 단점:
 *			16bit 위치중 어떤 위치에든 값을 쓸 수 있으나 4bit밖에 쓸 수 없다
 *			Worst Case로 4번의 명령으로 원하는 주소값이나 상수값을 얻을 수 있다
 *		
 */

#define USING_ENUM_TYPE
#ifdef USING_ENUM_TYPE
	// OPCODE 순서와 OPTABLE 순서는 동일해야 함
	enum OPCODE{
		ADD=0b0000, SUB, MOV, AND, ORR, CMP,
		STR, LDR, B, BL, IRET, PUSH, POP,
	};
	typedef enum OPCODE optype_t;
#else
	typedef unsigned short optype_t;
#endif

typedef unsigned char __u8;
typedef unsigned short xxbit_t;
typedef unsigned char bool;

#define BIT_TYPE				16
#define MEM_CODE_SIZE		1024
#define MEM_DATA_SIZE		128

/*** 하드웨어 구조체 선언 ***/

/** 1. CPU **/
/* 1) CLU(Control Logic Unit) - 미완 */
struct CLU_STRUCT{
	__u8 mix:1;
	__u8 ctrl:1;
};

/* 2) Register Bank */
struct REG_STRUCT{
	xxbit_t reg;							// Register Storage
	char *regName;						// Register Name
};

/* 3) Instruction Set */
struct OPSTRUCT{
	char *opcode;							// OPCODE 문자열
	optype_t binary;					// 변환될 Binary
	bool (*op_func)(char *rn, char *rd, char *operand2);		//
};

/** 2. Memory **/
// LDR, STR 주소연산을 할 때 32->16bit 전환을 위해 기본 베이스 주소를 두고 offset(변위)값으로 주소에 접근
// BASEADDR_CODE + offset = MEM.CODE[offset] address
struct MEM_STRUCT{
	xxbit_t code[MEM_CODE_SIZE];
	xxbit_t data[MEM_DATA_SIZE];	// struct로 data부분을 재구성해야함
	unsigned int code_lastIndex;
	unsigned int code_baseAddr;
	unsigned int data_lastIndex;
	unsigned int data_baseAddr;
};

/*** 하드웨어 구조체 전역변수 선언 ***/
/** 1. CPU **/
extern struct CLU_STRUCT CLU;
extern struct REG_STRUCT REG[BIT_TYPE];
extern struct OPSTRUCT OPTABLE[BIT_TYPE];					// OPTABLE: CPU에서 실행가능한 Instruct SET의 집합
extern struct MEM_STRUCT MEM;

/*	Name		RegNO		Using																After Function call, preservation ture or not
 *	a1-a2		0-1			인수/결과값/스크래치 레지스터				X
 *	a3-a4		2-3			인수/스크래치 레지스터							X
 *	v1-v8		4-11		지역 루틴용 변수										O
 *	ip			12			프로시져 내 스크래치 레지스터				X
 *	sp			13			스택 포인터													O
 *	lr			14			링크 레지스터(복귀 주소)						O
 *	pc			15			프로그램 카운터											n.a
 */

void printMemory();

#endif
