#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <stdio.h>

#define FALSE						0
#define TRUE						1

#define BIT_TYPE				16
#define MEM_CODE_SIZE		1024
#define MEM_DATA_SIZE		128

#define MASK_OP					0xF000
#define MASK_RD					0x0F00
#define MASK_RN					0x00F0
#define MASK_OR					0x000F
#define MASK_FG					0x00C0
#define MASK_IM					0x003F
#define MASK_SF					0x0030
#define MASK_R1					MASK_RN
#define MASK_R2					MASK_OR

#define POS_OP					12
#define POS_RD					8
#define POS_FG					6
#define POS_RN					4
#define POS_OR					0
#define POS_SF					POS_RN
#define POS_R1					POS_RN
#define POS_R2					POS_OR

#define USING_ENUM_TYPE

// OPCODE 순서와 OPTABLE 순서는 동일해야 함
#ifdef USING_ENUM_TYPE
	enum OPCODE{
		ADD=0b0000, SUB, MOV, AND, ORR, CMP,
		STR, LDR, B, BL, IRET, PUSH, POP,
	};
	typedef enum OPCODE optype_t;
#else
	typedef unsigned short optype_t;
#endif

typedef unsigned int  __u32;
typedef unsigned short xxbit_t;
typedef unsigned char bool;

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

/*** 하드웨어 구조체 선언 ***/

/** 1. CPU **/
/* 1) CLU(Control Logic Unit) - 미완 */
struct CLU_STRUCT{
	__u32 n:1;													// 계산의 결과가 음수일 때 1로 체크됨
	__u32 z:1;													// 계산의 결과가 0일때 1로 체크됨
};

/* 2) Register Bank */
struct REG_STRUCT{
	xxbit_t reg;												// Register Storage
	char *regName;											// Register Name
};

/* 3) Instruction Set */
struct OPSTRUCT{
	char *opcode;												// OPCODE 문자열
	optype_t binary;										// 변환될 Binary
	bool (*op_func)(xxbit_t binary);		// Instruct Function
};

/** 2. Memory **/
// LDR, STR 주소연산을 할 때 32->16bit 전환을 위해 기본 베이스 주소를 두고 offset(변위)값으로 주소에 접근
// BASEADDR_CODE + offset = MEM.CODE[offset] address
struct MEM_STRUCT{
	xxbit_t code[MEM_CODE_SIZE];
	xxbit_t data[MEM_DATA_SIZE];	// struct로 data부분을 재구성해야함
	unsigned int code_lastIndex;	// Code영역에서 마지막으로 기록된 주소 Index
	unsigned int code_baseAddr;		// Code영역의 Base Address = &(Code[0])
	unsigned int data_lastIndex;	// Data영역에서 마지막으로 기록된 주소 Index
	unsigned int data_baseAddr;		// Data영역의 Base Address = &(Data[0])
};

/*** 하드웨어 구조체 전역변수 선언 ***/
/** 1. CPU **/
extern struct CLU_STRUCT CLU;											// CLU Register
extern struct REG_STRUCT REG[BIT_TYPE];						// Register Bank
extern struct OPSTRUCT OPTABLE[BIT_TYPE];					// OPTABLE: CPU에서 실행가능한 Instruct SET의 집합
extern struct MEM_STRUCT MEM;											// Memory

/*** Debug 함수 ***/
void printMemory();
void printCPU();

/*** OPCODE 함수 선언 ***/
bool add_func(xxbit_t);
bool sub_func(xxbit_t);
bool mov_func(xxbit_t);
bool and_func(xxbit_t);
bool orr_func(xxbit_t);
bool cmp_func(xxbit_t);
bool str_func(xxbit_t);
bool ldr_func(xxbit_t);
bool b_func(xxbit_t);
bool bl_func(xxbit_t);
bool iret_func(xxbit_t);
bool push_func(xxbit_t);
bool pop_func(xxbit_t);

#endif
