#ifndef __CORE_H__
#define __CORE_H__

bool runCPU();
bool cReadRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t *reg);
bool cWriteRegister(const struct CLU_STRUCT *table, xxbit_t binary, basic_t data);
bool cReadDataMemory(basic_t *data, basic_t address);
bool cWriteDataMemory(basic_t data, basic_t address);
bool cReadStackMemory(basic_t *data);
bool cWriteStackMemory(basic_t data);
bool cExecuteALU(const struct CLU_STRUCT *table, basic_t *reg, basic_t *result);
basic_t getModuleAddress(xxbit_t binary);
bool setEntryPoint();
bool selectBRYN(xxbit_t binary);
optype_t getOPCODE(xxbit_t binary);
const struct CLU_STRUCT *getCLU_TABLE(optype_t opcode, xxbit_t binary);

#endif
