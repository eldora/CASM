#ifndef __PARSING_H__
#define __PARSING_H__

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

xxbit_t wordParsing(char *pWord[], int wordNumber);
bool asm2bin(char *asmtxt);

#endif
