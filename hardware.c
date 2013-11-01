#include "hardware.h"

/*** 하드웨어 구조체 전역변수 초기화 ***/
struct CLU_STRUCT *pCLU;
const struct CLU_STRUCT CLU_TABLE[BIT_TYPE][CLU_MODE][CLU_TABLE_SIZE] = {
	{{add_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, 0, 0, 0}, },			// ADD, MODE TYPE:1
	{{sub_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, 0, 0, 0}, },			// SUB, MODE TYPE:1
	{{mul_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, 0, 0, 0}, },			// MUL, MODE TYPE:1
	{{div_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, 0, 0, 0}, },			// DIV, MODE TYPE:1
	{{NULL,			CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, 0, UNUSED, 0}, 
	 {NULL,			CLU_W,	UNUSED, UNUSED, UNUSED, 2, 0, 0, 0, 0, UNUSED, UNUSED}, 
	 {NULL,			CLU_W,	UNUSED, UNUSED, UNUSED, 3, 0, 0, 0, 0, UNUSED, UNUSED}, },						// MOV, MODE TYPE:3
	{{and_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, UNUSED, 1, 0}, 
	 {and_func, CLU_W,	UNUSED, UNUSED, UNUSED, 2, 0, 0, 0, UNUSED, 1, UNUSED}, 
	 {and_func, CLU_W,	UNUSED, UNUSED, UNUSED, 3, 0, 0, 0, UNUSED, 1, UNUSED}, },						// AND, MODE TYPE:3
	{{orr_func, CLU_W,	UNUSED, UNUSED, UNUSED, 0, 0, 0, 0, UNUSED, 1, 0}, 
	 {orr_func, CLU_W,	UNUSED, UNUSED, UNUSED, 2, 0, 0, 0, UNUSED, 1, UNUSED}, 
	 {orr_func, CLU_W,	UNUSED, UNUSED, UNUSED, 3, 0, 0, 0, UNUSED, 1, UNUSED}, },						// ORR, MODE TYPE:3
	{{xor_func,	UNUSED, UNUSED, UNUSED, UNUSED, 0, 0, UNUSED, UNUSED, UNUSED, 1, 0}, 
	 {xor_func,	UNUSED, UNUSED, UNUSED, UNUSED, 2, 0, UNUSED, UNUSED, UNUSED, 1, UNUSED}, 
	 {xor_func,	UNUSED, UNUSED, UNUSED, UNUSED, 3, 0, UNUSED, UNUSED, UNUSED, 1, UNUSED}, },	// CMP, MODE TYPE:3
	{{add_func,	CLU_W,	CLU_R,	UNUSED, UNUSED, 1, 0, 1, 0, 0, 0, UNUSED}, },									// LDR, MODE TYPE:1
	{{add_func,	UNUSED,	CLU_W,	UNUSED, UNUSED, 1, 0, UNUSED, UNUSED, 0, UNUSED, 1}, },				// STR, MODE TYPE:1
	{{NULL,			UNUSED, UNUSED, UNUSED, UNUSED, 0, 1, UNUSED, UNUSED, UNUSED, UNUSED, 0}, },	// B, MODE TYPE:1
	{{NULL,			UNUSED, UNUSED, UNUSED, 1,			0, 1, UNUSED, UNUSED, UNUSED, UNUSED, 0}, },	// BL, MODE TYPE:1
	{{NULL,			UNUSED, UNUSED, UNUSED, UNUSED, UNUSED, 2, UNUSED, UNUSED, UNUSED, UNUSED, UNUSED}, },	// IRET, MODE TYPE:1
	{{NULL,			UNUSED, UNUSED, CLU_W,	UNUSED, 0, 2, 0, UNUSED, UNUSED, UNUSED, 0}, },				// PUSH, MODE TYPE:1
	{{NULL,			CLU_W,	UNUSED, CLU_R,	UNUSED, UNUSED, UNUSED, UNUSED, 1, 1, UNUSED, UNUSED}, },				// POP, MODE TYPE:1
};
struct CPSR_STRUCT CPSR = {0,};
struct REG_STRUCT REG[BIT_TYPE] = {
	{0, "r0"},	{0, "r1"},	{0, "r2"},	{0, "r3"},
	{0, "r4"},	{0, "r5"},	{0, "r6"},	{0, "r7"},
	{0, "r8"},	{0, "r9"},	{0, "r10"},	{0, "r11"},
	{0, "r12"},	{0, "r13"},	{0, "r14"},	{0, "r15"},
};
const struct OP_STRUCT OP_TABLE[OPCODE_NUMBER] = {
	{"ADD", ADD, CLU_TABLE[0]}, {"SUB", SUB, CLU_TABLE[1]},
	{"MUL", MUL, CLU_TABLE[2]}, {"DIV", DIV, CLU_TABLE[3]},
	{"MOV", MOV, CLU_TABLE[4]}, {"AND", AND, CLU_TABLE[5]},
	{"ORR", ORR, CLU_TABLE[6]}, {"CMP", CMP, CLU_TABLE[7]},
	{"LDR", LDR, CLU_TABLE[8]}, {"STR", STR, CLU_TABLE[9]}, 
	{"B"	, B	 , CLU_TABLE[10]}, {"BL", BL , CLU_TABLE[10]},
	{"IRET", IRET, CLU_TABLE[10]},		
	{"PUSH", PUSH, CLU_TABLE[11]}, {"POP", POP, CLU_TABLE[12]},
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
		printf("REG%02d: %d\n", i, REG[i].data);
}

/*** ALU Function ***/
basic_t add_func(basic_t param1, basic_t param2){
	basic_t result = param1 + param2;

	if(SHRT_MAX - param1 < param2)
		CPSR.v = 1;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
basic_t sub_func(basic_t param1, basic_t param2){
	basic_t result = param1 - param2;

	if(SHRT_MIN + param1 > param2)
		CPSR.v = 1;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
basic_t mul_func(basic_t param1, basic_t param2){
	basic_t result = param1 * param2;
	
	if(param1 != 0 && result / param1 != param2)
		CPSR.v = 1;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
// Overflow가 나올 경우의 수가 있나?
basic_t div_func(basic_t param1, basic_t param2){
	basic_t result = param1 / param2;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
basic_t and_func(basic_t param1, basic_t param2){
	basic_t result = param1 & param2;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
basic_t orr_func(basic_t param1, basic_t param2){
	basic_t result = param1 | param2;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}
basic_t xor_func(basic_t param1, basic_t param2){
	basic_t result = param1 ^ param2;

	CPSR.n = result < 0 ? 1 : 0;
	CPSR.z = (param1==param2) ? 1 : 0;

	return result;
}

/*
bool add_func(xxbit_t param){
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
*/
