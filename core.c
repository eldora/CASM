#include "hardware.h"
#include "core.h"

/*** runCPU Function ***/
/*
 * Fetch - Decode - Execute 의 3단계로 실행(추후 파이프라인화를 진행하기 위해)
 * Fetch: Instruction 명령어를 메모리에서 CPU내부로 가져오는 단계
 * Decode: Binary 해석(Opcode Check), Register값 확인
 * Execute: Decoding된 Instruction을 해석
 *
 * Component단위로 실행 Process를 정함
 * CPU Cycle: Instruction Memory -> Register Bank -> ALU -> Data Memory -> Stack Memory -> PC
 *
 * runCPU 함수에서 컴포넌트 비트를 확인하여 컴포넌트 함수를 호출하고,
 * 컴포넌트 함수 안에서는 MUX비트를 확인하여 동작하는 스타일 구조
 */
bool runCPU(){
	optype_t opcode;
	xxbit_t binary;
	const struct CLU_STRUCT *table;

	struct REG_STRUCT *pIP_REG = &(REG[IP_REG_INDEX]);
	struct REG_STRUCT *pSP_REG = &(REG[SP_REG_INDEX]);
	struct REG_STRUCT *pLR_REG = &(REG[LR_REG_INDEX]);
	struct REG_STRUCT *pPC_REG = &(REG[PC_REG_INDEX]);

	// 각 컴포넌트간 이동시 임시로 저장할 값
	basic_t regRD[2] = {0,}, result, bryn;
	basic_t dataWD, dataRD, dataAddr;
	basic_t stackWD, stackRD, stackAddr;

	// main함수의 Entry Pointer를 PC값으로 세팅: FALSE이면 main을 못찾았으므로 종료
	if(!setEntryPoint())
		return FALSE;

	for(;;){
#if 0
		printf("STEP:%d --CPU --\n", pPC_REG->data);
		printCPU();
#endif

#if 1
		// PC값이 MEM_CODE_SIZE를 넘어가면 break;
		if(pPC_REG->data > MEM_CODE_SIZE)
			break;
#endif

		//Fetch Start
		binary = MEM.code[pPC_REG->data];
		//Fetch End

		//Decode Start
		opcode = getOPCODE(binary);								// EXTERNAL_OPCODE를 얻기 위해 getOPCODE로 opcode를 얻는다
		table = getCLU_TABLE(opcode, binary);			// OPCODE의 CLU_TABLE를 얻는다

#if 0
		printf("PC:%d, opcode:%d\n", pPC_REG->data, opcode);
		printf("TABLE:%0X, %0X\n", table->SWITCH.component, table->SWITCH.mux);
#endif

		if(opcode == (optype_t)HALT)							// PROGRAM END CONDITION
			break;

		if(SWITCH_MASK(table->SWITCH.component, S_RERF))
			cReadRegister(table, binary, regRD);
		//Decode End
		
		//Execute Start
		
		dataWD = regRD[1];
		if(SWITCH_MASK(table->SWITCH.mux, S_BSEL)){
			switch(table->BSEL){
				case 0:
					break;
				case 1:
					regRD[1] = (binary & MASK_OS) >> POS_OS;
					break;
				case 2:
					regRD[1] = (binary & MASK_IM) >> POS_IM;
					break;
				case 3:
					regRD[1] = ((binary & MASK_OR) >> POS_OR)<<(4 * ((binary & MASK_SF) >> POS_SF));
					break;
				default:
					//error
					break;
			}// 이 Switch문을 함수로 나눌지는 고민
		}

		if(SWITCH_MASK(table->SWITCH.component, S_ALUFN))
			cExecuteALU(table, regRD, &result);
		else
			result = regRD[1];

		dataAddr = bryn = result;

#if 0
		printf("Result: %d\n", result);
#endif

		if(SWITCH_MASK(table->SWITCH.component, S_WEDF)){
			if(table->WEDF == CLU_R)
				cReadDataMemory(&dataRD, dataAddr);
			if(table->WEDF == CLU_W)
				cWriteDataMemory(dataWD, dataAddr);
		}

		if(SWITCH_MASK(table->SWITCH.mux, S_ADSEL))
			result = stackWD = (table->ADSEL==1) ? dataRD : result;

		//Stack Function
		if(SWITCH_MASK(table->SWITCH.component, S_WESF)){
			if(table->WESF == CLU_R)
				cReadStackMemory(&stackRD);
			if(table->WESF == CLU_W)
				cWriteStackMemory(stackWD);
		}

		if(SWITCH_MASK(table->SWITCH.mux, S_WDSEL))
			result = (table->WDSEL==1) ? stackRD : result;

		if(SWITCH_MASK(table->SWITCH.component, S_WERF))
			cWriteRegister(table, binary, result);

		if(SWITCH_MASK(table->SWITCH.mux, S_BRYN))
			CPSR.br_yn = selectBRYN(binary);

		if(SWITCH_MASK(table->SWITCH.mux, S_LRSF))
			pLR_REG->data = pPC_REG->data + PC_NEXT;


		// Branch할 주소를 리턴
		if(CPSR.br_yn)
			bryn = getModuleAddress(binary);

		if(SWITCH_MASK(table->SWITCH.mux, S_PCSEL)){
			switch(table->PCSEL){
				case 0:
					pPC_REG->data += PC_NEXT;				break;
				case 1:
					pPC_REG->data = bryn;						break;
				case 2:
					pPC_REG->data = pLR_REG->data;	break;
				break;
			}
		}

		// pc값 변화 적용
		
		//Execute End
#if 0
		printf("\nbinary: %0X\n", binary);
		printMemory();
		printCPU();
#endif
	}
}

bool cReadRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t *reg){
	int regIndex[2];
	regIndex[0] = (table->RA1SEL==1) ? ((binary & MASK_RD) >> POS_RD) : ((binary & MASK_R1) >> POS_R1);
	regIndex[1] = (table->RA2SEL==1) ? ((binary & MASK_RD) >> POS_RD) : ((binary & MASK_R2) >> POS_R2);


	switch(regIndex[0] | regIndex[1]){
		case 0 ... 15:
			break;
		default:
			return FALSE;
	}

	reg[0] = REG[regIndex[0]].data;
	reg[1] = REG[regIndex[1]].data;

#if 0
	printf("binary: %x\n", binary);
	printf("regIndex[0]:%d, regIndex[1]:%d\n", regIndex[0], regIndex[1]);
	printf("reg[0]:%d, reg[1]:%d\n", reg[0], reg[1]);
#endif

	return TRUE;
}

bool cWriteRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t data){
	unsigned int regIndex;
	regIndex = (table->WASEL==1) ? ((binary & MASK_R2) >> POS_R2) : ((binary & MASK_RD) >> POS_RD);

	REG[regIndex].data = data;
	return TRUE;
}

// 현재는 MEM.data Index로 주소값을 사용
bool cReadDataMemory(basic_t *data, basic_t address){
	if(address<0 || address >= MEM_DATA_SIZE)
		return FALSE;

	*data = MEM.data[address];
	return TRUE;
}
bool cWriteDataMemory(basic_t data, basic_t address){
	if(address<0 || address >= MEM_DATA_SIZE)
		return FALSE;

	MEM.data[address] = data;
	return TRUE;
}
bool cReadStackMemory(basic_t *data){
	unsigned int stackIndex;
	MEM.stack_lastIndex -= PC_NEXT;
	if(MEM.stack_lastIndex < 0){
		MEM.stack_lastIndex = 0;
		return FALSE;
	}
	*data = MEM.stack[MEM.stack_lastIndex];
	return TRUE;
}
bool cWriteStackMemory(basic_t data){
	unsigned int stackIndex;
	stackIndex = MEM.stack_lastIndex;
	if(stackIndex >= MEM_STACK_SIZE)
		return FALSE;
	MEM.stack_lastIndex += PC_NEXT;
	MEM.stack[stackIndex] = data;
	return TRUE;
}

bool cExecuteALU(const struct CLU_STRUCT *table, basic_t *reg, basic_t *result){
	*result = table->ALUFN(reg[0], reg[1]);

	return TRUE;
}

basic_t getModuleAddress(xxbit_t binary){
	unsigned int memMapIndex;
	memMapIndex = (binary & MASK_OR) >> POS_OR;
	return MEM_MAP.MODULE[memMapIndex].index;
}

bool setEntryPoint(){
	int i;
	for(i=0; i<MEM_MAP.lastIndex; i++)
		if(!strcmp(MEM_MAP.MODULE[i].name, "$main")){
			REG[PC_REG_INDEX].data = MEM_MAP.MODULE[i].index;
			return TRUE;
		}
	return FALSE;
}

// 함수명 고민
bool selectBRYN(xxbit_t binary){
	unsigned char flag = 0;
	switch((binary & MASK_CF) >> POS_CF){
		case 0b0000:		// NONE
			flag = TRUE;
			break;
		case 0b0010:		// EQ
			flag = (CPSR.z==1) ? TRUE : FALSE;
			break;
		case 0b0011:		// NE
			flag = (CPSR.z==0) ? TRUE : FALSE;
			break;
		case 0b0100:		// GT
			flag = (CPSR.z==0)&&(CPSR.n==CPSR.v) ? TRUE : FALSE;
			break;
		case 0b0101:		// LT
			flag = (CPSR.n!=CPSR.v) ? TRUE : FALSE;
			break;
		case 0b0110:		// GE
			flag = (CPSR.n==CPSR.v) ? TRUE : FALSE;
			break;
		case 0b0111:		// LE
			flag = (CPSR.z==1)||(CPSR.n!=CPSR.v) ? TRUE : FALSE;
			break;
		default:
			break;
	}
	return (bool)flag;
}

// getOPCODE함수로 실제 사용하는 2^4이상의 OPCODE를 표한하기 위해 사용함
optype_t getOPCODE(xxbit_t binary){
	optype_t opcode;

	opcode = (binary & MASK_OP) >> POS_OP;
	switch(opcode){
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			break;
		case MOV:
		case AND:
		case ORR:
		case CMP:
			break;
		case LDR:
		case STR:
			break;
		case B:
			// 여기서는 기존 Binary를 갖고 오는 것이기 때문에 B밖에 선택되지 않음 B, BL, IRET 구분은 11-8Bit인 Type으로 구분함
			// opcode B는 type bit를 확인하여 BL, IRET으로 나눠줘야 함
			opcode += (((binary & MASK_TP) >> POS_TP) + OPCODE_GAP_B);
			break;
		case PUSH:
		case POP:
			break;
		case HALT:
			break;
	}
	return opcode;
}

const struct CLU_STRUCT *getCLU_TABLE(optype_t opcode, xxbit_t binary){
	const struct CLU_STRUCT *table;
	xxbit_t mode;

	switch(opcode){
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			table = CLU_TABLE[opcode][0];
			break;
		case MOV:
		case AND:
		case ORR:
		case CMP:
			mode = (binary & MASK_FG) >> POS_FG;
			table = CLU_TABLE[opcode][mode];
			break;
		case LDR:
		case STR:
			table = CLU_TABLE[opcode][0];
			break;
		case B:
			table = CLU_TABLE[B][0];
			break;
		case BL:
			table = CLU_TABLE[B][1];
			break;
		case IRET:
			table = CLU_TABLE[B][2];
			break;
		case PUSH:
		case POP:
			table = CLU_TABLE[opcode][0];
			break;
		case HALT:
			table = CLU_TABLE[opcode][0];
			break;
	}
	return table;
}
