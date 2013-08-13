## [한이음 프로젝트] - 나만의 CPU 만들기 ##

#### v1.0(20130811) ####
- 각 기능별로 소스파일과 헤더파일을 구분
- 어셈파일을 읽어와 OPCODE만 파싱되어 메모리에 기록하는 부분까지 완료
- 이후 버전에는 각 OPCODE 포맷별로 나머지 12bit를 해석하여 메모리에 기록하는 부분까지 완료

#### v1.1(20130812) ####
- ADD, SUM, MOV 명령어 파싱 및 명령어 처리 완료

### 해야할 일 ###
- 헤더 파일에 구현한 부분에 있어 설명과 주석 달기
- 각 OPCODE에 대해 그림과 instruct set 명령어를 서술
- 어셈블리 파싱단계에서 라벨처리 부분을 어떻게 구현할지 고려
- **나머지 명령어에 대해 파싱 및 명령어 처리를 구현해야 함**
- **메모리 데이터 영역을 어떻게 사용할 것인지 생각**

-------------------------------------------------------------------------------------------

## File Description ##

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

1. ADD, SUB

  Opcode  | Rd      | Rn      | Operand | Usage
  ------- | ------- | ------- | ------- | -------
  4bit    | 4b      | 4b      | 4b      | Register

2. MOV, AND, ORR, CMP

  If F=?  | Opcode  | Rd      | F       | S       | Operand | Usage     | Detail
  ------- | ------- | ------- | ------- | ------- | ------- | -------   | -------
  0       | 4bit    | 4b      | 2b      | 0b      | 6b      | Register  | Register Index
  1       | 4bit    | 4b      | 2b      | 0b      | 6b      | Immediate | 6bit Constant
  2       | 4bit    | 4b      | 2b      | 2b      | 4b      | Shift     | Operand<<4*S

3. STR, LDR

  Opcode  | Rd      | Rn      | Operand | Usage    | Detail
  ------- | ------- | ------- | ------- | -------  | -------
  4bit    | 4b      | 4b      | 4b      | Register | Rd = [Rn + Operand]
  
4. B, BL, IRET

  Opcode  | Reversed | Operand | Usage     
  ------- | -------  | ------- | -------
  4bit    | 8b       | 4b      | Register

5. IRET

  Opcode  | Reversed | Detail    
  ------- | -------  | -------
  4bit    | 12b      | mov pc, lr

6. PUSH, POP

  If F=?  | Opcode  | Reversed | F       | Reversed | Operand  | Usage    | Detail
  ------- |-------  | -------  | ------- | -------  | -------  | -------  | -------  
  0       | 4bit    | 4b       | 2b      | 2b       | 4b       | Register | Register  Push/Pop
  1       | 4bit    | 4b       | 2b      | 2b       | 4b       | No Use   | Reg.v1-v8 Push/Pop
  2       | 4bit    | 4b       | 2b      | 2b       | 4b       | No Use   | Reg.All   Push/Pop

