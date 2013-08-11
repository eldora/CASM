#include "hardware.h"
#include "parsing.h"

xxbit_t wordParsing(char *pWord[], int wordNumber){
	int i;
	xxbit_t binaryCode = 0;
	optype_t opcode;

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
			break;
		case MOV:
		case AND:
		case ORR:
		case CMP:
			break;
		case STR:
		case LDR:
			break;
		case B:
		case BL:
			break;
		case IRET:
			break;
		case PUSH:
		case POP:
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
void asm2bin(char *asmtxt){
	char buf[255];
	char *pLine[10], *pWord[5];
	int fd, lineNumber, wordNumber, i, j, optSize;

	if((fd = open(asmtxt, O_RDONLY)) < 0){											// 어셈블리어 텍스트 파일 로드
		printf("%s file can't open!!\n", asmtxt);
		close(fd);
		return;
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
void bin2com(){

}
