## 나만의 CPU 만들기 ##

#### v1.0(20130811) ####
- 각 기능별로 소스파일과 헤더파일을 구분
- 어셈파일을 읽어와 OPCODE만 파싱되어 메모리에 기록하는 부분까지 완료
- 이후 버전에는 각 OPCODE 포맷별로 나머지 12bit를 해석하여 메모리에 기록하는 부분까지 완료

#### v1.1(20130812) ####
- ADD, SUM, MOV 명령어 파싱 및 명령어 처리 구현 

#### v1.2(20130813) ####
- 모든 명령어 파싱 구현
- README.md MARKDOWN 문법을 사용하여 내용 정리(지속적으로 Update)

#### v1.3(20130816) ####
- 명령어 처리 구현(ldr까지, TEST X)
+ str, ldr 명령어에서 baseAddr를 default Register로 정할지 말지 정해야 한다.

#### v1.4(20130906) ####
- Branch Type Defined
- 각 명령어 포맷별로 회로도 작성
+ 명령어 메모리에서 읽어 온 바이너리를 분석하여 OPCODE 함수를 호출하는 것이 아닌, CLU 설정으로 구성되게끔 프로그램을 작성한다.

#### v2.0(20131030) ####
- CLU TABLE 구성 완료, CLU_TABLE.xlsx
- CASM FORMAT 및 Path Diagram 정리, GradPortfolio.pptx

#### v2.1(20131031) ####
- HARDWARE 구성요소 정의 및 선언 완료

#### v2.2(20131031) ####
- workdParsing 함수 수정 - 보완된 포맷 형식에 맞게 변환

#### v2.3(20131101) ####
- ALUFN 함수 결과값을 CPSR 상태 레지스터에 적용

#### v3.0(20131104) ####
+ runCPU함수 완성, add_asm.txt만 정상작동, 정상 swap코드 테스트 요망
+ CLU_SWITCH를 보고 Component or Mux를 실행할지 안할지 결정
+ CLU_STRUCT에서 RERF추가 및 CLU_RW 메크로 추가
- hardware.h: struct CLU_SWITCH 추가, enum SWITCH_XXX 추가
- hardware.c: CLU_SWITCH Binary 추가
- core.c: cReadRegister, cWriteRegister, cExecuteALU, selectBRYN, getOPCODE, getCLU_TABLE 함수 추가
- Makefile: dubug 명령어 추가
- pptx, xlsx 파일 업데이트(CLU_SWITCH 부분 추가 및 기타 요소 등...)

#### v3.1(20131105) ####
- hardware.h: MEM_MAP_STRUCT 추가
- Add Files: swap_asm.txt, swap_asm_pushpop.txt

#### v3.2(20131106) ####
+ swap.asm_txt까지 정상동작 확인
- hardware.h: MEM_MAP_STRUCT, SWITCH_MASK 매크로 함수 추가
- parsing.c: $FUNCTION 명령어를 전처리 과정 진행, MEM_MAP에 정보 갱신
- parsing.c: pLine 배열 크기 증가(10줄 이상의 정보가 들어오면 세그먼테이션 에러)
- hardware.c: CLU_TABLE 초기화 부분에서 B,BL,IRET 부분 안나눠놨었음
- core.c: getModuleAddress, setEntryPoint 함수 추가
- core.c: getCLU_TABLE함수에서 B,BL,IRET에 적절한 table 정보 대입

#### v3.3(20131106) ####
+ 다수의 txt파일로 동작 가능
- Add Files: swap_asm_1.txt, swap_asm_2.txt

#### v3.4(20131106) ####
+ core.c: runCPU() - PUSH, POP, STR, LDR 기능 추가
- Makefile: Make pushpop, Make data 명령어 추가
- Add Files: swap_asm_data.txt

#### v3.4(20131106) ####
+ core.c: runCPU() - PUSH, POP, STR, LDR 기능 추가
- Makefile: Make pushpop, Make data 명령어 추가
- Add Files: swap_asm_data.txt

#### v3.5(20131130) ####
+ core.c: cReadStackMemory 수정하여 Pop이 제대로 작동하도록 수정
+ hardware.c: debug용의 print 함수들 16진수로 표기

#### v4.0(20131202) ####
+ Makefile: 매크로를 사용해 Makefile를 재작성하고 소스디렉토리와 어셈블리 파일 디렉토리로 구분함
+ parsing.c: asm2bin함수에서 어셈블리 파일을 읽어올 때 파일 크기에 상관없이 읽어올 수 있도록 바꿈, 배열 동적으로 구현
- 헤더 파일에 있었던 C 라이브러리 파일을 소스 파일로 옮김
- printCPU 함수에서 임시적으로 DATA SECTION을 3만큼 보여준 것을 lastIndex만큼 보여주도록 설정
- Wall Warning 제거 
- Makefile에서 DEFINES = 에 SHOW_EXECUTE 매크로 주석을 제거하면 각 틱별로 CPU Resource상태를 볼 수 있음

#### v4.1(20131202) ####
- git 파일 위치 설정 및 Source/include로 해더파일 이동

#### v4.2(20131204) ####
- PPT 한이음 제출 전 최종 파일

### 해야할 일 ###
+ parsing.c에서 전처리 과정으로 ip, sp, lr, pc 문자열을 r12,13,14,15로 변환과정을 추가해야 함
- TICK별로 CPU를 구동할 때 Register Bank를 Read, Write 상태로 설정해야 함(현재는 Write상태만)
- rupCPU 구동 방식을 시뮬레이터에 가깝게 할지 에뮬레이터에 가깝게 할지 고민

#### NOTE ####
- 본 프로그램은 CPU 시뮬레이터에 입각하여 작성하였으며, 하드웨어적인 요소를 최대한 논리적으로 구성하여 소프트웨어로 표현함
- 시뮬레이터는 현실과 동일하게 구성, 에뮬레이터는 실행결과가 동일하게 나오는 것
- 컴포넌트(PC, MEM, ALU etc..)등을 어떻게 처리할지 고민(FLOW를 고정?)
- Tick: CLU상에서의 내부 클럭 처리 단위
- Instruction Clock: 실제 CPU클럭, 명령 실행 클럭 단위

-------------------------------------------------------------------------------------------

## File Description ##

### CPU State Register ###

  Name    | Detail  
  ------- | -------
  N       | 계산의 결과가 음수일 때, set1
  Z       | 계산의 결과가 0일 때, set 1 


### Register Usage Map ###

  Name    | RegNO   | Usage                              | 함수호출후 값 보존여부
  ------- | ------- | -------                            | -------
  a1-a2   | 0-1     | 인수/결과값/스크래치 레지스터      | X
  a3-a4   | 2-3     | 인수/스크래치 레지스터             | X 
  v1-v8   | 4-11    | 지역 루틴용 변수                   | O
  ip      | 12      | 프로시져 내 스크래치 레지스터      | X
  sp      | 13      | 스택 포인터                        | O
  lr      | 14      | 링크 레지스터(복귀 주소)           | O
  pc      | 15      | 프로그램 카운터                    | n.a
  

### OPCODE FORMAT ###
- Usage는 Operand를 어떤 방식으로 사용할지를 서술한 항목임
- 기본적으로 Rd, Rn는 Register F, S 등은 Flag bit로 사용
- 여유 비트 공간은 Rev(Reserved) 항목으로 대체함

1. ADD, SUB

  Opcode  | Rd      | Rn      | Operand | Usage
  ------- | ------- | ------- | ------- | -------
  4bit    | 4b      | 4b      | 4b      | Register

2. MOV, AND, ORR, CMP

  If F=?  | Opcode  | Rd      | F       | S       | Rev     | Operand | Usage     | Detail
  ------- | ------- | ------- | ------- | ------- | ------- | ------- | -------   | -------
  0       | 4bit    | 4b      | 2b      | 0b      | 2b      | 4b      | Register  | Register Index
  1       | 4bit    | 4b      | 2b      | 0b      | 0b      | 6b      | Immediate | 6bit Constant
  2       | 4bit    | 4b      | 2b      | 2b      | 0b      | 4b      | Shift     | Operand<<4*S

3. STR, LDR

  Opcode  | Rd      | Rn      | Operand | Usage    | Detail
  ------- | ------- | ------- | ------- | -------  | -------
  4bit    | 4b      | 4b      | 4b      | Register | Rd = [Rn(BaseAddr) + Operand(Offset)]
  
4. B, BL, BX, IRET

  Opcode   | Type     | CPSR F   | Rd(Addr) | Detail    
  -------  | -------  | -------  | -------  | ------
  4bit     | 4b       | 4b       | 4b       | mov pc, lr

  Type    | CPSR F  | Detail    
  ------- | ------- | ------- 
	0000    | 0000    | B
	0001    | 0000    | BL
	0002    | 0000    | BX
	0003    | 0000    | IRET

  Type    | CPSR F  | Suffix  | Condition
  ------- | ------- | ------- | -------
	0000    | 0000    | NONE    | None
	0000    | 0010    | EQ      | [Z] Set
	0000    | 0011    | NE      | [Z] Clear
	0000    | 0100    | GT      | [Z] Clear && (N=V)
	0000    | 0101    | LT      | [N] ≠ [V]
	0000    | 0110    | GE      | [N] = [V]
	0000    | 0111    | LE      | [Z] Set || ([N]≠[V])

  CPSR    | Set(1)          | Clear(0)    
  ------- | -------         | ------- 
	[Z]     | Equal           | Not Equal
	[N]     | Negative Number | 0 or Positive Number
	[V]     | Overflow        | No Overflow


5. PUSH, POP

  If F=?  | Opcode  | Rev     | F       | Rev     | Operand | Usage    | Detail
  ------- | ------- | ------- | ------- | ------- | ------- | -------  | -------  
  0       | 4bit    | 4b      | 2b      | 2b      | 4b      | Register | Register  Push/Pop
  1       | 4bit    | 4b      | 2b      | 2b      | 4b      | No Use   | Reg.v1-v8 Push/Pop
  2       | 4bit    | 4b      | 2b      | 2b      | 4b      | No Use   | Reg.All   Push/Pop

