#ifndef __PARSING_H__
#define __PARSING_H__

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define POS_OP		12
#define POS_RD		8
#define POS_RN		4
#define POS_OR		0

xxbit_t wordParsing(char *pWord[], int wordNumber);
void asm2bin(char *asmtxt);
void bin2com();

#endif
