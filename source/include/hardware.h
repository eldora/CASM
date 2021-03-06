#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#define FALSE						0
#define TRUE						1

#define BIT_TYPE				16
#define OPCODE_NUMBER		16
#define MEM_CODE_SIZE		1024
#define MEM_STACK_SIZE	256
#define MEM_DATA_SIZE		128
#define CLU_TABLE_SIZE	11
#define CLU_MODE				4

#define PC_NEXT					1

#define IP_REG_INDEX		12
#define SP_REG_INDEX		13
#define LR_REG_INDEX		14
#define PC_REG_INDEX		15

// CLU_TABLE_MACRO
#define CLU_R						0
#define CLU_W						1
#define CLU_RW					2
#define UNUSED					0

// SHIFT MACRO
#define MASK_OP					0xF000
#define MASK_RD					0x0F00
#define MASK_RN					0x00F0
#define MASK_OR					0x000F
#define MASK_FG					0x00C0
#define MASK_IM					0x003F
#define MASK_OS					MASK_OR
#define MASK_SF					0x0030
#define MASK_R1					MASK_RN
#define MASK_R2					MASK_OR
#define MASK_TP					MASK_RD
#define MASK_CF					MASK_RN

#define POS_OP					12
#define POS_RD					8
#define POS_RN					4
#define POS_OR					0
#define POS_FG					6
#define POS_IM					0						// Immediate Value	
#define POS_OS					POS_OR			// Offset
#define POS_SF					POS_RN			// Shift Flag
#define POS_R1					POS_RN
#define POS_R2					POS_OR
#define POS_TP					POS_RD			// Type
#define POS_CF					POS_RN			// CPSR Flag

// MEMORY MACRO FUNCTION
#define SWITCH_MASK(binary, pos)	((binary >> pos)&0x01)

// MEM_MAP MACRO
#define	MEM_MAP_SIZE		30

//
#define WORD_CHAR_SIZE	15
#define LINE_CHAR_SIZE	(WORD_CHAR_SIZE*4)

#define OPCODE_GAP_B		(BL-B-1)		// 17 - 10 - 1: -1의 이유는 type BL의 값이 1이므로
#define USING_ENUM_TYPE
// OPCODE 순서와 OP_TABLE 순서는 동일해야 함
// OPCODE 기본 명령어(ORIGINAL_OPCODE)는 0b0XXXX로 시작하고, 구별을 위해 필요한 부가적인 명령어(EXTERNAL_OPCODE)는 0b1XXXX로 시작한다.
#ifdef USING_ENUM_TYPE
	enum OPCODE{
		ADD=0b00000, SUB, MUL, DIV, MOV, AND, ORR, CMP,			// ORIGINAL_OPCODE: 0~15
		LDR, STR, B, PUSH, POP, HALT=0b01111,
		BL=0b10000, IRET,																		// EXTERNAL_OPCODE: 16~
	};
	typedef enum OPCODE optype_t;
#else
	typedef unsigned short optype_t;
#endif


// CLU SWITCH Binary Bit의 Index를 가리키는 MACRO
// 새로운 컴포넌트나 먹스가 추가되면 앞에서부터 추가
/*
 * S_ALUFN: ALU
 * S_WERF: 
 *
 */
enum CLU_SWITCH_COMPONENT{
	S_WESF=0, S_WEDF, S_WERF, S_RERF, S_ALUFN,
};

enum CLU_SWITCH_MUX{
	S_BRYN=0, S_RA2SEL, S_RA1SEL, S_WASEL, S_WDSEL,
	S_ADSEL, S_PCSEL, S_BSEL, S_LRSF,
};

typedef unsigned int  clu_t;
typedef unsigned char cpsr_t;
typedef unsigned short xxbit_t;				// CASM binary Data Type
typedef short basic_t;								// CASM basic  Data Type	
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
/* 1) CLU(Control Logic Unit) */
struct CLU_STRUCT{
	struct CLU_SWITCH{
		clu_t component:5;							// component들을 사용할지 안할지를 저장하고 있는 switch binary
		clu_t mux:9;										// mux에 대한 switch binary
	}SWITCH;
	basic_t  (*ALUFN)(basic_t param1, basic_t param2);
	clu_t WERF:2;
	clu_t WEDF:1;
	clu_t WESF:1;
	clu_t LRSF:1;
	clu_t BSEL:2;
	clu_t PCSEL:2;
	clu_t ADSEL:1;
	clu_t WDSEL:1;
	clu_t WASEL:1;
	clu_t RA1SEL:1;
	clu_t RA2SEL:1;
};

/* 1-1) CPSR(Process State Register) */
struct CPSR_STRUCT{
	cpsr_t z:1;												// if set(1) then Equal else Not Equal
	cpsr_t n:1;												// if set(1) then Negative Number else 0 or Positive Number
	cpsr_t v:1;												// if set(1) then Overflow else No Overflow
	cpsr_t br_yn:1;										// if set(1) then Branch else No Branch
};

/* 2) Register Bank */
struct REG_STRUCT{
	basic_t data;										// Register Data Storage
	char *name;											// Register Name
};

/* 3) Instruction Set */
struct OP_STRUCT{
	char *opcode;														// OPCODE STRING 
	xxbit_t binary;													// Binary CODE
	const struct CLU_STRUCT (*table)[CLU_TABLE_SIZE];		// CLU 2-dim TABLE POINTER: pTABLE[CLU_MODE][CLU_TABLE_SIZE]
};

/** 2. Memory **/
// LDR, STR 주소연산을 할 때 32->16bit 전환을 위해 기본 베이스 주소를 두고 offset(변위)값으로 주소에 접근
// BASEADDR_CODE + offset = MEM.CODE[offset] address
struct MEM_STRUCT{
	xxbit_t code[MEM_CODE_SIZE];
	xxbit_t data[MEM_DATA_SIZE];	// struct로 data부분을 재구성해야함
	xxbit_t stack[MEM_STACK_SIZE];
	unsigned int code_lastIndex;	// Code영역에서 마지막으로 기록된 주소 Index
	unsigned int code_baseAddr;		// Code영역의 Base Address = &(Code[0])
	unsigned int data_lastIndex;	// Data영역에서 마지막으로 기록된 주소 Index
	unsigned int data_baseAddr;		// Data영역의 Base Address = &(Data[0])
	unsigned int stack_lastIndex;	// Data영역에서 마지막으로 기록된 주소 Index
	unsigned int stack_baseAddr;		// Data영역의 Base Address = &(Data[0])
};

struct MEM_MAP_STRUCT{
	struct MODULE_STRUCT{
		char name[30];
		unsigned int index;	
	}MODULE[MEM_MAP_SIZE];
	unsigned int lastIndex;
	unsigned int lineIndex;
};

/*
 * MEM_STRUCT MEM[3]; // 0:CODE, 1:DATA, 2:STACK 사용법도 고려
struct MEM_STRUCT{
	xxbit_t 
	unsigned int index;
	unsigned int baseAddress;
}
*/

/*** 하드웨어 구조체 전역변수 선언 ***/
/** 1. CPU **/
extern struct CLU_STRUCT *pCLU;														// CLU Register
extern const struct CLU_STRUCT CLU_TABLE[BIT_TYPE][CLU_MODE][CLU_TABLE_SIZE];			// CLU TABLE
extern struct CPSR_STRUCT CPSR;
extern struct REG_STRUCT REG[BIT_TYPE];										// Register Bank
extern const struct OP_STRUCT OP_TABLE[OPCODE_NUMBER];		// CPU에서 실행가능한 Instruct SET의 집합
extern struct MEM_STRUCT MEM;															// Memory
extern struct MEM_MAP_STRUCT MEM_MAP;											// Memory Map

/*** Debug 함수 ***/
void printMemory();
void printMemMap();
void printCPU();

/*** OPCODE 함수 선언 ***/
basic_t add_func(basic_t, basic_t);
basic_t sub_func(basic_t, basic_t);
basic_t mul_func(basic_t, basic_t);
basic_t div_func(basic_t, basic_t);
basic_t mov_func(basic_t, basic_t);
basic_t and_func(basic_t, basic_t);
basic_t orr_func(basic_t, basic_t);
basic_t xor_func(basic_t, basic_t);

#endif
