#include "hardware.h"
#include "parsing.h"

// if return 0 then, syntax error!!
xxbit_t wordParsing(char *pWord[], int wordNumber){
	xxbit_t binaryCode;
	xxbit_t rd, fg, rn, or;
	optype_t opcode;

	int i;
	char ch, *ps, *pd, temp[10];

	binaryCode = rd = fg = rn = or = 0;

	// Step1. 읽어온 라인의 OPCODE를 OPTABLE에서 찾는다
	for(i=0; i<BIT_TYPE; i++){					
		if(!strcmp(OPTABLE[i].opcode, pWord[0])){
			opcode = OPTABLE[i].binary;
			binaryCode |= (opcode<<POS_OP);
			break;
		}
	}

	switch(opcode){																							// OPTYPE별로 포맷에 따라 해석
		case ADD:
		case SUB:
			rd = atoi(pWord[1]+1);
			rn = atoi(pWord[2]+1);
			or = atoi(pWord[3]+1);
			binaryCode |= ((rd<<POS_RD) | (rn<<POS_RN) | (or<<POS_OR));
			break;
		case MOV:
		case AND:
		case ORR:
		case CMP:
			rd = atoi(pWord[1]+1);
			ch = *(pWord[2]);																			// pWord[2]의 첫번째 문자를 파악하여 어떤 형식(레지스터<r>, 상수<#>, 시프트 상수연산<<>)으로 썼는지 확인한다
			if(ch == 'r'){
				fg = 0;
				or = atoi(pWord[2]+1);
			}else if(ch == '#'){
				fg = 1;
				or = atoi(pWord[2]+1);
			}else if(ch == '<'){
				fg = 2;
				rn = atoi(pWord[2]+1);
				or = atoi(pWord[3]+1);
			}
			binaryCode |= ((rd<<POS_RD) | ((fg&0x03)<<POS_FG) | ((rn&0x03)<<POS_RN) | (or<<POS_OR));
			break;
		case STR:
		case LDR:
			rd = atoi(pWord[1]+1);
			rn = atoi(pWord[2]+1);
			or = atoi(pWord[3]+1);
			binaryCode |= ((rd<<POS_RD) | (rn<<POS_RN) | (or<<POS_OR));
			break;
		case B:
		case BL:
		case IRET:
			or = atoi(pWord[1]+1);
			binaryCode |= (or<<POS_OR);
			break;
		case PUSH:
		case POP:
			ch = *(pWord[1]);
			if(ch == 'r'){
				fg = 0;
				or = atoi(pWord[1]+1);
			}else if(ch == '#'){
				ch = *(pWord[1]+1);	
				ch = (char)toupper(ch);
				if(ch == 'V') fg = 1;
				if(ch == 'A') fg = 2;
			}
			binaryCode |= (((fg&0x03)<<POS_FG) | (or<<POS_OR));
			break;
		default:
			break;
	}

	printf("wordNumber: %d\n", wordNumber);
	for(i=0; i<wordNumber; i++){
		printf("%d: %s\n", (int)opcode, pWord[i]);
	}

	return binaryCode;
}

/*** ASEMMBLY -> BINARY Function ***/
bool asm2bin(char *asmtxt){
	char buf[255];
	char *pLine[10], *pWord[5];
	int fd, lineNumber, wordNumber, i, j, optSize;

	if((fd = open(asmtxt, O_RDONLY)) < 0){											// 어셈블리어 텍스트 파일 로드
		printf("%s file can't open!!\n", asmtxt);
		close(fd);
		return FALSE;
	}

	read(fd, buf, 255);																					// 버퍼만큼 읽음(추후 수정)

	lineNumber=0;
	pLine[lineNumber++] = strtok(buf, "\n");											// 라인별로 나눠서 해당 포인터를 pLine배열에 저장해놓음
	while(pLine[lineNumber++] = strtok(NULL, "\n"));

	for(i=0; i<lineNumber-1; i++){																// 라인을 읽어와 각 워드를 읽고 이에 맞는 binary값을 써줌
		memset(pWord, 0, sizeof(pWord));
		wordNumber = 0;

		pWord[wordNumber++] = strtok(pLine[i], "\t ");
		while(pWord[wordNumber++] = strtok(NULL, "\t "));

		MEM.code[MEM.code_lastIndex++] = wordParsing(pWord, wordNumber-1);
	}
	return TRUE;
}

/*** BINARY -> COMMAND Function ***/
/*
 * ## CODE_INDEX까지 한 라인씩 바이너리를 읽어와 수행
 * 1. 현재 행의 OPCODE를 파악한다
 * 2. OPCODE에 맞게 나머지 12bit에서 정보를 읽어온다.
 * 3. OPCODE에 맞는 함수를 실행시킨다. 이 때 매개변수는 12bit에서 읽어온 정보를 바탕으로 정해준다.
 * 4. 실행된 함수는 각 함수에 맞는 일을 수행한다.
 * 5. CODE_INDEX까지 루프를 돌며 1-4의 작업을 반복한다.
 *
 */

bool bin2com(){
	optype_t opcode;
	xxbit_t binary;
	int i;
	
	for(i=0; i<MEM.code_lastIndex; i++){
		binary = MEM.code[i];
		opcode = (binary & MASK_OP) >> POS_OP;
		OPTABLE[opcode].op_func(binary);
	}

	return TRUE;
}

bool runCPU(){
	optype_t opcode;
	xxbit_t binary;
	struct REG_STRUCT *pIP_REG = &(REG[IP_REG_INDEX]);
	struct REG_STRUCT *pSP_REG = &(REG[SP_REG_INDEX]);
	struct REG_STRUCT *pLR_REG = &(REG[LR_REG_INDEX]);
	struct REG_STRUCT *pPC_REG = &(REG[PC_REG_INDEX]);

	for(;;){
		binary = MEM.code[pPC_REG->data];
		opcode = (binary & MASK_OP) >> POS_OP;				// OPCODE에 따라 CLU상태를 바꾼다
			
	}
}
