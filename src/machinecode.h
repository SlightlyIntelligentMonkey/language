#pragma once
#include "utility.h"

typedef enum OperandType
{
    IMM,
    MEM,
    REG
} OperandType;

typedef struct MachineCode
{
    char class[8];
    int feature_flags;
    uint8_t opcode[8];
    int opsize;
    OperandType type[4];
    char size[4];
    //optimization information
    int est_latency;
    int est_rthroughput;
} MachineCode;

typedef struct Register
{
    char name[8];
    //char accessor[4][8];
    int size;
    int purpose;
} Register;

typedef struct Architecture
{
    char name[8];
    Register *registers;
    int register_count;
    MachineCode *mcode;
    int mcode_count;
} Architecture;

typedef enum IR_Inst
{
    INST_OPERATION,
    INST_BRANCH,
    INST_ASSIGN
} IR_Inst;