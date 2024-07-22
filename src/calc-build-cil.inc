/* calc-parse.inc - Copyright (c) 2024 Frithurik Grint */

#ifndef defasm
/// @brief Placeholder definition of defasm. (CIL)
#   define defasm(name, opcode, instr, argc)
#endif // defasm

/* =---- Common Intermediate Langauge Args ---------------------= */

#ifndef CALC_BUILD_CIL_INC_
#define CALC_BUILD_CIL_INC_

#define _CIL_NOARG 0x00
#define _CIL_INT08 0x01
#define _CIL_INT16 0x02
#define _CIL_INT32 0x04
#define _CIL_INT64 0x08
#define _CIL_FLT08 0x11
#define _CIL_FLT16 0x12
#define _CIL_FLT32 0x14
#define _CIL_FLT64 0x18
/// @brief Method op argument kind.
#define _CIL_METHD 0xE0
/// @brief Call site descr op argument kind.
#define _CIL_SITED 0xE1
/// @brief <uint32, int32, int32 (t1..tN)>
#define _CIL_SWTCH 0xEE

#endif // CALC_BUILD_CIL_INC_

/* =---- Common Intermediate Language --------------------------= */

/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(BREAK, 0x00, "break", _CIL_NOARG)
/// @brief 
defasm(LDARG_0, 0x00, "ldarg.0", _CIL_NOARG)
/// @brief 
defasm(LDARG_1, 0x00, "ldarg.1", _CIL_NOARG)
/// @brief 
defasm(LDARG_2, 0x00, "ldarg.2", _CIL_NOARG)
/// @brief 
defasm(LDARG_3, 0x00, "ldarg.3", _CIL_NOARG)
/// @brief 
defasm(LDLOC_0, 0x00, "ldloc.0", _CIL_NOARG)
/// @brief 
defasm(LDLOC_1, 0x00, "ldloc.1", _CIL_NOARG)
/// @brief 
defasm(LDLOC_2, 0x00, "ldloc.2", _CIL_NOARG)
/// @brief 
defasm(LDLOC_3, 0x00, "ldloc.3", _CIL_NOARG)
/// @brief 
defasm(STLOC_0, 0x00, "stloc.0", _CIL_NOARG)
/// @brief 
defasm(STLOC_1, 0x00, "stloc.1", _CIL_NOARG)
/// @brief 
defasm(STLOC_2, 0x00, "stloc.2", _CIL_NOARG)
/// @brief 
defasm(STLOC_3, 0x00, "stloc.3", _CIL_NOARG)
/// @brief 
defasm(LDARG_S, 0x00, "ldarg.s", _CIL_INT08)
/// @brief 
defasm(LDARGA_S, 0x00, "ldarga.s", _CIL_INT08)
/// @brief 
defasm(STARG_S, 0x00, "starg.s", _CIL_INT08)
/// @brief 
defasm(LDLOC_S, 0x00, "ldloc.s", _CIL_INT08)
/// @brief 
defasm(LDLOCA_S, 0x00, "ldloca.s", _CIL_INT08)
/// @brief 
defasm(STLOC_S, 0x00, "stloc.s", _CIL_INT08)
/// @brief 
defasm(LDNULL, 0x00, "ldnull", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_M1, 0x00, "ldc.i4.m1", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_m1, 0x00, "ldc.i4.M1", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_0, 0x00, "ldc.i4.0", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_1, 0x00, "ldc.i4.1", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_2, 0x00, "ldc.i4.2", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_3, 0x00, "ldc.i4.3", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_4, 0x00, "ldc.i4.4", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_5, 0x00, "ldc.i4.5", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_6, 0x00, "ldc.i4.6", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_7, 0x00, "ldc.i4.7", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_8, 0x00, "ldc.i4.8", _CIL_NOARG)
/// @brief 
defasm(LDC_I4_S, 0x00, "ldc.i4.s", _CIL_INT08)
/// @brief 
defasm(LDC_I4, 0x00, "ldc.i4", _CIL_INT32)
/// @brief 
defasm(LDC_I8, 0x00, "ldc.i8", _CIL_INT64)
/// @brief 
defasm(LDC_R4, 0x00, "ldc.r4", _CIL_FLT32)
/// @brief 
defasm(LDC_R8, 0x00, "ldc.r8", _CIL_FLT64)
/// @brief 
defasm(DUP, 0x00, "dup", _CIL_NOARG)
/// @brief 
defasm(POP, 0x00, "pop", _CIL_NOARG)
/// @brief 
defasm(JMP, 0x00, "jmp", _CIL_METHD)
/// @brief 
defasm(CALL, 0x00, "call", _CIL_METHD)
/// @brief 
defasm(CALLI, 0x00, "calli", _CIL_SITED)
/// @brief 
defasm(RET, 0x00, "ret", _CIL_NOARG)
/// @brief 
defasm(BR_S, 0x00, "br.s", _CIL_INT08)
/// @brief 
defasm(BRFALSE_S, 0x00, "brfalse.s", _CIL_INT08)
/// @brief 
defasm(BRNULL_S, 0x00, "brnull.s", _CIL_INT08)
/// @brief 
defasm(BRZERO_S, 0x00, "brzero.s", _CIL_INT08)
/// @brief 
defasm(BRINST_S, 0x00, "brinst.s", _CIL_INT08)
/// @brief 
defasm(BRTRUE_S, 0x00, "brtrue.s", _CIL_INT08)
/// @brief 
defasm(BEQ_S, 0x00, "beq.s", _CIL_INT08)
/// @brief 
defasm(BGE_S, 0x00, "bge.s", _CIL_INT08)
/// @brief 
defasm(BGT_S, 0x00, "bgt.s", _CIL_INT08)
/// @brief 
defasm(BLE_S, 0x00, "ble.s", _CIL_INT08)
/// @brief 
defasm(BLT_S, 0x00, "blt.s", _CIL_INT08)
/// @brief 
defasm(BNE_UN_S, 0x00, "bne.un.s", _CIL_INT08)
/// @brief 
defasm(BGE_UN_S, 0x00, "bge.un.s", _CIL_INT08)
/// @brief 
defasm(BGT_UN_S, 0x00, "bgt.un.s", _CIL_INT08)
/// @brief 
defasm(BLE_UN_S, 0x00, "ble.un.s", _CIL_INT08)
/// @brief 
defasm(BLT_UN_S, 0x00, "blt.un.s", _CIL_INT08)
/// @brief 
defasm(BR, 0x00, "br", _CIL_INT32)
/// @brief 
defasm(BRFALSE, 0x00, "brfalse", _CIL_INT32)
/// @brief 
defasm(BRNULL, 0x00, "brnull", _CIL_INT32)
/// @brief 
defasm(BRZERO, 0x00, "brzero", _CIL_INT32)
/// @brief 
defasm(BRINST, 0x00, "brinst", _CIL_INT32)
/// @brief 
defasm(BRTRUE, 0x00, "brtrue", _CIL_INT32)
/// @brief 
defasm(BEQ, 0x00, "beq", _CIL_INT32)
/// @brief 
defasm(BGE, 0x00, "bge", _CIL_INT32)
/// @brief 
defasm(BGT, 0x00, "bgt", _CIL_INT32)
/// @brief 
defasm(BLE, 0x00, "ble", _CIL_INT32)
/// @brief 
defasm(BLT, 0x00, "blt", _CIL_INT32)
/// @brief 
defasm(BNE_UN, 0x00, "bne.un", _CIL_INT32)
/// @brief 
defasm(BGE_UN, 0x00, "bge.un", _CIL_INT32)
/// @brief 
defasm(BGT_UN, 0x00, "bgt.un", _CIL_INT32)
/// @brief 
defasm(BLE_UN, 0x00, "ble.un", _CIL_INT32)
/// @brief 
defasm(BLT_UN, 0x00, "blt.un", _CIL_INT32)
/// @brief 
defasm(SWITCH, 0x00, "switch", _CIL_SWTCH)
/// @brief 
defasm(LDIND_I1, 0x00, "ldind.i1", _CIL_NOARG)
/// @brief 
defasm(LDIND_U1, 0x00, "ldind.u1", _CIL_NOARG)
/// @brief 
defasm(LDIND_I2, 0x00, "ldind.i2", _CIL_NOARG)
/// @brief 
defasm(LDIND_U2, 0x00, "ldind.u2", _CIL_NOARG)
/// @brief 
defasm(LDIND_I4, 0x00, "ldind.i4", _CIL_NOARG)
/// @brief 
defasm(LDIND_U4, 0x00, "ldind.u4", _CIL_NOARG)
/// @brief 
defasm(LDIND_I8, 0x00, "ldind.i8", _CIL_NOARG)
/// @brief 
defasm(LDIND_U8, 0x00, "ldind.u8", _CIL_NOARG)
/// @brief 
defasm(LDIND_I, 0x00, "ldind.i", _CIL_NOARG)
/// @brief 
defasm(LDIND_R4, 0x00, "ldind.r4", _CIL_NOARG)
/// @brief 
defasm(LDIND_R8, 0x00, "ldind.r8", _CIL_NOARG)
/// @brief 
defasm(LDIND_REF, 0x00, "ldind.ref", _CIL_NOARG)
/// @brief 
defasm(STIND_REF, 0x00, "stind.ref", _CIL_NOARG)
/// @brief 
defasm(STIND_I1, 0x00, "stind.i1", _CIL_NOARG)
/// @brief 
defasm(STIND_I2, 0x00, "stind.i2", _CIL_NOARG)
/// @brief 
defasm(STIND_I4, 0x00, "stind.i4", _CIL_NOARG)
/// @brief 
defasm(STIND_I8, 0x00, "stind.i8", _CIL_NOARG)
/// @brief 
defasm(STIND_R4, 0x00, "stind.r4", _CIL_NOARG)
/// @brief 
defasm(STIND_R8, 0x00, "stind.r8", _CIL_NOARG)
/// @brief 
defasm(ADD, 0x00, "add", _CIL_NOARG)
/// @brief 
defasm(SUB, 0x00, "sub", _CIL_NOARG)
/// @brief 
defasm(MUL, 0x00, "mul", _CIL_NOARG)
/// @brief 
defasm(DIV, 0x00, "div", _CIL_NOARG)
/// @brief 
defasm(DIV_UN, 0x00, "div.un", _CIL_NOARG)
/// @brief 
defasm(REM, 0x00, "rem", _CIL_NOARG)
/// @brief 
defasm(REM_UN, 0x00, "rem.un", _CIL_NOARG)
/// @brief 
defasm(AND, 0x00, "and", _CIL_NOARG)
/// @brief 
defasm(OR, 0x00, "or", _CIL_NOARG)
/// @brief 
defasm(XOR, 0x00, "xor", _CIL_NOARG)
/// @brief 
defasm(SHL, 0x00, "shl", _CIL_NOARG)
/// @brief 
defasm(SHR, 0x00, "shr", _CIL_NOARG)
/// @brief 
defasm(SHR_UN, 0x00, "shr.un", _CIL_NOARG)
/// @brief 
defasm(NEG, 0x00, "neg", _CIL_NOARG)
/// @brief 
defasm(NOT, 0x00, "not", _CIL_NOARG)
/// @brief 
defasm(CONV_I1, 0x00, "conv.i1", _CIL_NOARG)
/// @brief 
defasm(CONV_I2, 0x00, "conv.i2", _CIL_NOARG)
/// @brief 
defasm(CONV_I4, 0x00, "conv.i4", _CIL_NOARG)
/// @brief 
defasm(CONV_I8, 0x00, "conv.i8", _CIL_NOARG)
/// @brief 
defasm(CONV_R4, 0x00, "conv.r4", _CIL_NOARG)
/// @brief 
defasm(CONV_R8, 0x00, "conv.r8", _CIL_NOARG)
/// @brief 
defasm(CONV_U4, 0x00, "conv.u4", _CIL_NOARG)
/// @brief 
defasm(CONV_U8, 0x00, "conv.u8", _CIL_NOARG)
/// @brief 
defasm(CALLVIRT, 0x00, "callvirt", _CIL_METHD)
/// @brief 
defasm(CPOBJ, 0x00, "cpobj", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "ldobj", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "ldstr", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "newobj", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "castclass", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "isinst", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "conv.r.un", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "unbox", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "throw", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)
/// @brief 
defasm(NOP, 0x00, "nop", _CIL_NOARG)

/* =------------------------------------------------------------= */

#ifdef defasm
#   undef defasm
#endif // UNDEF defasm
