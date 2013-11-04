#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>

bool runCPU();
bool cReadRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t *reg);
bool cWriteRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t data);
bool cExecuteALU(const struct CLU_STRUCT *table, basic_t *reg, basic_t *result);
bool selectBRYN(xxbit_t binary);
optype_t getOPCODE(xxbit_t binary);
const struct CLU_STRUCT *getCLU_TABLE(optype_t opcode, xxbit_t binary);

#endif
