#include "hardware.h"
#include "parsing.h"

// if return 0 then, syntax error!!
xxbit_t wordParsing(char *pWord[], int wordNumber){
	xxbit_t binaryCode;
	xxbit_t rd, flag, rn, or, r1, r2, type, cpsr;
	optype_t opcode;

	int i;
	char ch, *suffix[] = {"EQ", "NE", "GT", "LT", "GE", "LE"};

	binaryCode = rd = flag = rn = or = r1 = r2 = type = cpsr = 0;

	// Step1. 읽어온 라인의 OPCODE를 OP_TABLE에서 찾는다
	for(i=0; i<OPCODE_NUMBER; i++){					
		if(!strcmp(OP_TABLE[i].opcode, pWord[0])){
			opcode = OP_TABLE[i].binary;
			binaryCode |= (opcode<<POS_OP);
			break;
		}
	}

	switch(opcode){																							// OPTYPE별로 포맷에 따라 해석
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			// ADD R3 R1 R2
			// R3 = R1 + R2
			rd = atoi(pWord[1]+1);																	// R3
			r1 = atoi(pWord[2]+1);																	// R1
			r2 = atoi(pWord[3]+1);																	// R2
			binaryCode |= ((rd<<POS_RD) | (r1<<POS_R1) | (r2<<POS_R2));
			break;
		case MOV:
		case AND:
		case ORR:
		case CMP:
			// MOV R2 R1, MOV R2 #F0, MOV R2 <2 F
			// R2 = R1, R2 = 0xF0, R2 = F<<4*2
			rd = atoi(pWord[1]+1);
			ch = *(pWord[2]);																			// pWord[2]의 첫번째 문자를 파악하여 어떤 형식(레지스터<r>, 상수<#>, 시프트 상수연산<<>)으로 썼는지 확인한다
			if((char)tolower(ch) == 'r'){
				flag = 0;
				or = atoi(pWord[2]+1);
			}else if(ch == '#'){
				flag = 1;
				or = atoi(pWord[2]+1);
			}else if(ch == '<'){
				flag = 2;
				rn = atoi(pWord[2]+1);
				or = atoi(pWord[3]+1);
			}
			binaryCode |= ((rd<<POS_RD) | ((flag&0x03)<<POS_FG) | ((rn&0x03)<<POS_RN) | (or<<POS_OR));
			break;
		case LDR:
		case STR:
			// LDR R2 R1 #1
			// LDR: R2 = *(R1(0x0030)+0x1))
			// STR: *(R1(0x00F0)+0xF) = R2
			rd = atoi(pWord[1]+1);
			rn = atoi(pWord[2]+1);
			or = atoi(pWord[3]+1);
			binaryCode |= ((rd<<POS_RD) | (rn<<POS_RN) | (or<<POS_OR));
			break;
		case B:
			//	B	R1(0x0100), B NE R2(0x00F0)
			type = 0;
			goto B_COMMON;
		case BL:
			type = 1;
			binaryCode &= (0x00<<POS_OP);
			binaryCode |= (B<<POS_OP);
			goto B_COMMON;
		case IRET:
			type = 2;
			binaryCode &= (0x00<<POS_OP);
			binaryCode |= ((B<<POS_OP) | (type<<POS_TP));
			break;
B_COMMON:
			ch = *(pWord[1]);
			if((char)tolower(ch) == 'r')
				or = atoi(pWord[1]+1);
			else if((char)ch == '$'){
				for(i=0; i<MEM_MAP.lastIndex; i++){
					if(!strcmp(MEM_MAP.MODULE[i].name, pWord[1])){
						or = i;
						break;
					}
				}
			}
			else{
				for(i=0; i<6; i++){
					if(!strcmp(suffix[i], pWord[1])){
						cpsr = i+3;														// CPSR 조건 플레그는 0b0010부터 시작함
						break;
					}
				}
				or = atoi(pWord[2]+1);
			}
			binaryCode |= ((type<<POS_TP) | (cpsr<<POS_CF)  | (or<<POS_OR));
			break;
		case PUSH:
		case POP:
			// PUSH r1, PUSH #V, POP #A
			ch = *(pWord[1]);
			if((char)tolower(ch) == 'r'){
				flag = 0;
				or = atoi(pWord[1]+1);
			}else if(ch == '#'){
				ch = *(pWord[1]+1);	
				if((char)toupper(ch) == 'V') flag = 1;
				if((char)toupper(ch) == 'A') flag = 2;
			}
			binaryCode |= (((flag&0x03)<<POS_FG) | (or<<POS_OR));
			break;
		case HALT:
			break;
		default:
			break;
	}
#if 0
	printf("wordNumber: %d\n", wordNumber);
	for(i=0; i<wordNumber; i++){
		printf("%d: %s\n", (int)opcode, pWord[i]);
	}
#endif

	return binaryCode;
}

/*** ASEMMBLY -> BINARY Function ***/
bool asm2bin(char *asmtxt){
	char buf[255];
	char *pLine[50], *pWord[5];
	int fd, lineNumber, wordNumber, i, j, optSize;

	if((fd = open(asmtxt, O_RDONLY)) < 0){											// 어셈블리어 텍스트 파일 로드
		printf("%s file can't open!!\n", asmtxt);
		close(fd);
		return FALSE;
	}

	read(fd, buf, sizeof(buf));																					// 버퍼만큼 읽음(추후 수정)

#if 0
	printf("buf: %s\n", buf);
#endif

	lineNumber = 0;
	pLine[lineNumber++] = strtok(buf, "\n");											// 라인별로 나눠서 해당 포인터를 pLine배열에 저장해놓음
	while(pLine[lineNumber++] = strtok(NULL, "\n"));

#if 1
	printf("===CODE===\n");
	for(i=0; i<lineNumber-2; i++)
		printf("%d: %s\n", i, pLine[i]);
#endif

	for(i=0; i<lineNumber-2; i++){
		if(pLine[i][0] == '$'){
			strcpy(MEM_MAP.MODULE[MEM_MAP.lastIndex].name, pLine[i]);
			MEM_MAP.MODULE[MEM_MAP.lastIndex].index = (i-MEM_MAP.lastIndex);
			MEM_MAP.lastIndex++;
		}
	}

	for(i=0; i<lineNumber-2; i++){																// 라인을 읽어와 각 워드를 읽고 이에 맞는 binary값을 써줌
		memset(pWord, 0, sizeof(pWord));
		wordNumber = 0;

		pWord[wordNumber++] = strtok(pLine[i], "\t ");
		while(pWord[wordNumber++] = strtok(NULL, "\t "));

#if 0
		printf("===WORD===\n");
		printf("LINE:%s\n", pLine[i]);
		for(j=0; j<wordNumber; j++)
			printf("%d: %s\n", j, pWord[j]);
#endif

		if(*pWord[0] == '$')
			continue;

		MEM.code[MEM.code_lastIndex++] = wordParsing(pWord, wordNumber-1);	// Debug용으로 workNumber를 매개변수로 넘겨줌
	}
	return TRUE;
}
