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
	{"ADD", ADD, add_func}, {"SUB", SUB, sub_func},
	{"MOV", MOV, mov_func}, {"AND", AND, NULL},
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
		printf("%04x\n", MEM.code[i]);

	printf("===DATA SECTION===\n");
	for(i=0; i<MEM.data_lastIndex; i++)
		printf("%04x\n", MEM.data[i]);
}

void printCPU(){
	int i;
	for(i=0; i<BIT_TYPE; i++)
		printf("REG%02d: %d\n", i, REG[i].reg);
}

/*** OPCODE 함수 ***/
bool add_func(xxbit_t binary){
	int rdIndex, r1Index, r2Index;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	r1Index = (binary & MASK_R1)>>POS_R1;
	r2Index = (binary & MASK_R2)>>POS_R2;
	
	REG[rdIndex].reg = REG[r1Index].reg + REG[r2Index].reg;

	return TRUE;
}

bool sub_func(xxbit_t binary){
	int rdIndex, r1Index, r2Index;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	r1Index = (binary & MASK_R1)>>POS_R1;
	r2Index = (binary & MASK_R2)>>POS_R2;
	
	REG[rdIndex].reg = REG[r1Index].reg - REG[r2Index].reg;

	return TRUE;
}

bool mov_func(xxbit_t binary){
	int rdIndex, operand;
	int flag, shiftValue;

	rdIndex = operand = flag = shiftValue = 0;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	flag		= (binary & MASK_FG)>>POS_FG;

	switch(flag){
		case 0:													// Register
			operand = (binary & MASK_OR);
			REG[rdIndex].reg = REG[operand].reg;
			break;
		case 1:													// Immediate Value
			operand = (binary & MASK_IM);
			REG[rdIndex].reg = operand;
			break;
		case 2:													// Shift & Immediate Value
			shiftValue = (binary & MASK_SF)>>POS_SF;
			operand = (binary & MASK_OR);
			REG[rdIndex].reg = operand<<(4*shiftValue);
			break;
		case 3:
			break;
		default:
			break;
	}

	return TRUE;
}

bool and_func(xxbit_t binary){
	int rdIndex, operand;
	int flag, shiftValue;

	rdIndex = operand = flag = shiftValue = 0;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	flag		= (binary & MASK_FG)>>POS_FG;

	switch(flag){
		case 0:													// Register
			operand = (binary & MASK_OR);
			REG[rdIndex].reg &= REG[operand].reg;
			break;
		case 1:													// Immediate Value
			operand = (binary & MASK_IM);
			REG[rdIndex].reg &= operand;
			break;
		case 2:													// Shift & Immediate Value
			shiftValue = (binary & MASK_SF)>>POS_SF;
			operand = (binary & MASK_OR);
			REG[rdIndex].reg &= operand<<(4*shiftValue);
			break;
		case 3:
			break;
		default:
			break;
	}

	return TRUE;

}

bool orr_func(xxbit_t binary){
	int rdIndex, operand;
	int flag, shiftValue;

	rdIndex = operand = flag = shiftValue = 0;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	flag		= (binary & MASK_FG)>>POS_FG;

	switch(flag){
		case 0:													// Register
			operand = (binary & MASK_OR);
			REG[rdIndex].reg |= REG[operand].reg;
			break;
		case 1:													// Immediate Value
			operand = (binary & MASK_IM);
			REG[rdIndex].reg |= operand;
			break;
		case 2:													// Shift & Immediate Value
			shiftValue = (binary & MASK_SF)>>POS_SF;
			operand = (binary & MASK_OR);
			REG[rdIndex].reg |= operand<<(4*shiftValue);
			break;
		case 3:
			break;
		default:
			break;
	}

	return TRUE;
}

bool cmp_func(xxbit_t binary){
	int rdIndex, operand;
	int flag, shiftValue, result;

	rdIndex = operand = flag = shiftValue = 0;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	flag		= (binary & MASK_FG)>>POS_FG;

	switch(flag){
		case 0:													// Register
			operand = (binary & MASK_OR);
			result = REG[rdIndex].reg - REG[operand].reg;
			break;
		case 1:													// Immediate Value
			operand = (binary & MASK_IM);
			result = REG[rdIndex].reg - operand;
			break;
		case 2:													// Shift & Immediate Value
			shiftValue = (binary & MASK_SF)>>POS_SF;
			operand = (binary & MASK_OR);
			result = REG[rdIndex].reg - (operand<<(4*shiftValue));
			break;
		case 3:
			break;
		default:
			break;
	}

	if(result==0){
		CLU.n = 0;
		CLU.z = 1;
	}else if(result<0){
		CLU.n = 1;
		CLU.z = 0;
	}else{
		CLU.n = 0;
		CLU.z = 0;
	}

	return TRUE;
}

bool str_func(xxbit_t binary){
	int rdIndex, baseIndex, offsetIndex;
	xxbit_t *baseAddr;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	baseIndex = (binary & MASK_RN)>>POS_RN;
	offsetIndex = (binary & MASK_OR)>>POS_OR;

	baseAddr = REG[baseIndex].reg + REG[offsetIndex].reg;
	*baseAddr = REG[rdIndex].reg;

	return TRUE;
}

bool ldr_func(xxbit_t binary){
	int rdIndex, baseIndex, offsetIndex;
	xxbit_t *baseAddr;

	rdIndex = (binary & MASK_RD)>>POS_RD;
	baseIndex = (binary & MASK_RN)>>POS_RN;
	offsetIndex = (binary & MASK_OR)>>POS_OR;

	baseAddr = REG[baseIndex].reg + REG[offsetIndex].reg;
	REG[rdIndex].reg = *baseAddr;

	return TRUE;
}

bool b_func(xxbit_t binary){
}

bool bl_func(xxbit_t binary){
}

bool iret_func(xxbit_t binary){
}

bool push_func(xxbit_t binary){
}

bool pop_func(xxbit_t binary){
}
