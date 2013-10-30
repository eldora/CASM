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

### 해야할 일 ###
- workdParsing 함수 수정-보완된 포맷 형식에 맞게 변환
- ALUFN 함수 결과값을 CPSR 상태 레지스터에 적용
- runCPU 함수 완성 - CLU Table를 참조하여 동작하게끔 구현

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

