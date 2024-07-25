/* calc-parse.inc - Copyright (c) 2024 Frithurik Grint */

#ifndef defasm
/// @brief Placeholder definition of defasm. (CIL)
#   define defasm(name, opcode, instr, argc)
#endif // defasm

/* =---- Common Intermediate Langauge Args ---------------------= */

#ifndef CALC_BUILD_CIL_INC_
#   define CALC_BUILD_CIL_INC_
/// @brief No op argument kind.
#   define _CIL_NOARG 0x00
/// @brief Int8 op argument kind.
#   define _CIL_INT08 0x01
/// @brief Int16 op argument kind.
#   define _CIL_INT16 0x02
/// @brief Int32 op argument kind.
#   define _CIL_INT32 0x04
/// @brief Int64 op argument kind.
#   define _CIL_INT64 0x08
/// @brief Half op argument kind.
#   define _CIL_FLT16 0x12
/// @brief Single op argument kind.
#   define _CIL_FLT32 0x14
/// @brief Double op argument kind.
#   define _CIL_FLT64 0x18
/// @brief String op argument kind.
#   define _CIL_STRNG 0xA0
/// @brief Class op argument kind.
#   define _CIL_CLASS 0xA2
/// @brief Refrence op argument kind.
#   define _CIL_FIELD 0xA4
/// @brief Value type op argument kind.
#   define _CIL_VALTY 0xD0
/// @brief Type token op argument kind.
#   define _CIL_TYTOK 0xD1
/// @brief Method op argument kind.
#   define _CIL_METHD 0xE0
/// @brief Call site descr op argument kind.
#   define _CIL_SITED 0xE1
/// @brief Constructor op argument kind.
#   define _CIL_CCTOR 0xE2
/// @brief <uint32, int32, int32 (t1..tN)>
#   define _CIL_SWTCH 0xEE
#endif // CALC_BUILD_CIL_INC_

/* =---- Common Intermediate Language --------------------------= */

/// @brief 
defasm(NOP,             0x0000,     "nop",          _CIL_NOARG,     "nop")
/// @brief 
defasm(BREAK,           0x0001,     "break",        _CIL_NOARG,     "break")
/// @brief 
defasm(LDARG_0,         0x0002,     "ldarg.0",      _CIL_NOARG,     "ldarg.0")
/// @brief 
defasm(LDARG_1,         0x0003,     "ldarg.1",      _CIL_NOARG,     "ldarg.1")
/// @brief 
defasm(LDARG_2,         0x0004,     "ldarg.2",      _CIL_NOARG,     "ldarg.2")
/// @brief 
defasm(LDARG_3,         0x0005,     "ldarg.3",      _CIL_NOARG,     "ldarg.3")
/// @brief 
defasm(LDLOC_0,         0x0006,     "ldloc.0",      _CIL_NOARG,     "ldloc.0")
/// @brief 
defasm(LDLOC_1,         0x0007,     "ldloc.1",      _CIL_NOARG,     "ldloc.1")
/// @brief 
defasm(LDLOC_2,         0x0008,     "ldloc.2",      _CIL_NOARG,     "ldloc.2")
/// @brief 
defasm(LDLOC_3,         0x0009,     "ldloc.3",      _CIL_NOARG,     "ldloc.3")
/// @brief 
defasm(STLOC_0,         0x000A,     "stloc.0",      _CIL_NOARG,     "stloc.0")
/// @brief 
defasm(STLOC_1,         0x000B,     "stloc.1",      _CIL_NOARG,     "stloc.1")
/// @brief 
defasm(STLOC_2,         0x000C,     "stloc.2",      _CIL_NOARG,     "stloc.2")
/// @brief 
defasm(STLOC_3,         0x000D,     "stloc.3",      _CIL_NOARG,     "stloc.3")
/// @brief 
defasm(LDARG_S,         0x000E,     "ldarg.s",      _CIL_INT08,     "ldarg.s %hhu")
/// @brief 
defasm(LDARGA_S,        0x000F,     "ldarga.s",     _CIL_INT08,     "ldarga.s %hhu")
/// @brief 
defasm(STARG_S,         0x0010,     "starg.s",      _CIL_INT08,     "starg.s %hhu")
/// @brief 
defasm(LDLOC_S,         0x0011,     "ldloc.s",      _CIL_INT08,     "ldloc.s %hhu")
/// @brief 
defasm(LDLOCA_S,        0x0012,     "ldloca.s",     _CIL_INT08,     "ldloca.s %hhu")
/// @brief 
defasm(STLOC_S,         0x0013,     "stloc.s",      _CIL_INT08,     "stloc.s %hhu")
/// @brief 
defasm(LDNULL,          0x0014,     "ldnull",       _CIL_NOARG,     "ldnull")
/// @brief 
defasm(LDC_I4_M1,       0x0015,     "ldc.i4.m1",    _CIL_NOARG,     "ldc.i4.m1")
/// @brief 
defasm(LDC_I4_m1,       0x0016,     "ldc.i4.M1",    _CIL_NOARG,     "ldc.i4.M1")
/// @brief 
defasm(LDC_I4_0,        0x0017,     "ldc.i4.0",     _CIL_NOARG,     "ldc.i4.0")
/// @brief 
defasm(LDC_I4_1,        0x0018,     "ldc.i4.1",     _CIL_NOARG,     "ldc.i4.1")
/// @brief 
defasm(LDC_I4_2,        0x0019,     "ldc.i4.2",     _CIL_NOARG,     "ldc.i4.2")
/// @brief 
defasm(LDC_I4_3,        0x001A,     "ldc.i4.3",     _CIL_NOARG,     "ldc.i4.3")
/// @brief 
defasm(LDC_I4_4,        0x001B,     "ldc.i4.4",     _CIL_NOARG,     "ldc.i4.4")
/// @brief 
defasm(LDC_I4_5,        0x001C,     "ldc.i4.5",     _CIL_NOARG,     "ldc.i4.5")
/// @brief 
defasm(LDC_I4_6,        0x001D,     "ldc.i4.6",     _CIL_NOARG,     "ldc.i4.6")
/// @brief 
defasm(LDC_I4_7,        0x001E,     "ldc.i4.7",     _CIL_NOARG,     "ldc.i4.7")
/// @brief 
defasm(LDC_I4_8,        0x001F,     "ldc.i4.8",     _CIL_NOARG,     "ldc.i4.8")
/// @brief 
defasm(LDC_I4_S,        0x0020,     "ldc.i4.s",     _CIL_INT08,     "ldc.i4.s %hhu")
/// @brief 
defasm(LDC_I4,          0x0021,     "ldc.i4",       _CIL_INT32,     "ldc.i4 %u")
/// @brief 
defasm(LDC_I8,          0x0022,     "ldc.i8",       _CIL_INT64,     "ldc.i8 %I64u")
/// @brief 
defasm(LDC_R4,          0x0023,     "ldc.r4",       _CIL_FLT32,     "ldc.r4 %g")
/// @brief 
defasm(LDC_R8,          0x0024,     "ldc.r8",       _CIL_FLT64,     "ldc.r8 %Lg")
/// @brief 
defasm(DUP,             0x0025,     "dup",          _CIL_NOARG,     "dup")
/// @brief 
defasm(POP,             0x0026,     "pop",          _CIL_NOARG,     "pop")
/// @brief 
defasm(JMP,             0x0027,     "jmp",          _CIL_METHD,     "jmp %s")
/// @brief 
defasm(CALL,            0x0028,     "call",         _CIL_METHD,     "call %s")
/// @brief 
defasm(CALLI,           0x0029,     "calli",        _CIL_SITED,     "calli %s")
/// @brief 
defasm(RET,             0x002A,     "ret",          _CIL_NOARG,     "ret")
/// @brief 
defasm(BR_S,            0x002B,     "br.s",         _CIL_INT08,     "br.s %hhu")
/// @brief 
defasm(BRFALSE_S,       0x002C,     "brfalse.s",    _CIL_INT08,     "brfalse.s %hhu")
/// @brief 
defasm(BRNULL_S,        0x002C,     "brnull.s",     _CIL_INT08,     "brnull.s %hhu")
/// @brief 
defasm(BRZERO_S,        0x002C,     "brzero.s",     _CIL_INT08,     "brzero.s %hhu")
/// @brief 
defasm(BRINST_S,        0x002D,     "brinst.s",     _CIL_INT08,     "brinst.s %hhu")
/// @brief 
defasm(BRTRUE_S,        0x002D,     "brtrue.s",     _CIL_INT08,     "brtrue.s %hhu")
/// @brief 
defasm(BEQ_S,           0x002E,     "beq.s",        _CIL_INT08,     "beq.s %hhu")
/// @brief 
defasm(BGE_S,           0x002F,     "bge.s",        _CIL_INT08,     "bge.s %hhu")
/// @brief 
defasm(BGT_S,           0x0030,     "bgt.s",        _CIL_INT08,     "")
/// @brief 
defasm(BLE_S,           0x0031,     "ble.s",        _CIL_INT08,     "")
/// @brief 
defasm(BLT_S,           0x0032,     "blt.s",        _CIL_INT08,     "")
/// @brief 
defasm(BNE_UN_S,        0x0033,     "bne.un.s",     _CIL_INT08,     "")
/// @brief 
defasm(BGE_UN_S,        0x0034,     "bge.un.s",     _CIL_INT08,     "")
/// @brief 
defasm(BGT_UN_S,        0x0035,     "bgt.un.s",     _CIL_INT08,     "")
/// @brief 
defasm(BLE_UN_S,        0x0036,     "ble.un.s",     _CIL_INT08,     "")
/// @brief 
defasm(BLT_UN_S,        0x0037,     "blt.un.s",     _CIL_INT08,     "")
/// @brief 
defasm(BR,              0x0038,     "br",           _CIL_INT32,     "")
/// @brief 
defasm(BRFALSE,         0x0039,     "brfalse",      _CIL_INT32,     "")
/// @brief 
defasm(BRNULL,          0x0039,     "brnull",       _CIL_INT32,     "")
/// @brief 
defasm(BRZERO,          0x0039,     "brzero",       _CIL_INT32,     "")
/// @brief 
defasm(BRINST,          0x003A,     "brinst",       _CIL_INT32,     "")
/// @brief 
defasm(BRTRUE,          0x003A,     "brtrue",       _CIL_INT32,     "")
/// @brief 
defasm(BEQ,             0x003B,     "beq",          _CIL_INT32,     "")
/// @brief 
defasm(BGE,             0x003C,     "bge",          _CIL_INT32,     "")
/// @brief 
defasm(BGT,             0x003D,     "bgt",          _CIL_INT32,     "")
/// @brief 
defasm(BLE,             0x003E,     "ble",          _CIL_INT32,     "")
/// @brief 
defasm(BLT,             0x003F,     "blt",          _CIL_INT32,     "")
/// @brief 
defasm(BNE_UN,          0x0000,     "bne.un",       _CIL_INT32,     "")
/// @brief 
defasm(BGE_UN,          0x0000,     "bge.un",       _CIL_INT32,     "")
/// @brief 
defasm(BGT_UN,          0x0000,     "bgt.un",       _CIL_INT32,     "")
/// @brief 
defasm(BLE_UN,          0x0000,     "ble.un", _CIL_INT32,     "")
/// @brief 
defasm(BLT_UN,          0x0000,     "blt.un", _CIL_INT32,     "")
/// @brief 
defasm(SWITCH,          0x0000,     "switch", _CIL_SWTCH,     "")
/// @brief 
defasm(LDIND_I1,        0x0000,     "ldind.i1", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_U1,        0x0000,     "ldind.u1", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_I2,        0x0000,     "ldind.i2", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_U2,        0x0000,     "ldind.u2", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_I4,        0x0000,     "ldind.i4", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_U4,        0x0000,     "ldind.u4", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_I8,        0x0000,     "ldind.i8", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_U8,        0x0000,     "ldind.u8", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_I,         0x0000,     "ldind.i", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_R4,        0x0000,     "ldind.r4", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_R8,        0x0000,     "ldind.r8", _CIL_NOARG,     "")
/// @brief 
defasm(LDIND_REF,       0x0000,     "ldind.ref", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_REF,       0x0000,     "stind.ref", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_I1,        0x0000,     "stind.i1", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_I2,        0x0000,     "stind.i2", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_I4,        0x0000,     "stind.i4", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_I8,        0x0000,     "stind.i8", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_R4,        0x0000,     "stind.r4", _CIL_NOARG,     "")
/// @brief 
defasm(STIND_R8,        0x0000,     "stind.r8", _CIL_NOARG,     "")
/// @brief 
defasm(ADD,             0x0000,     "add", _CIL_NOARG,     "")
/// @brief 
defasm(SUB,             0x0000,     "sub", _CIL_NOARG,     "")
/// @brief 
defasm(MUL,             0x0000,     "mul", _CIL_NOARG,     "")
/// @brief 
defasm(DIV,             0x0000,     "div", _CIL_NOARG,     "")
/// @brief 
defasm(DIV_UN,          0x0000,     "div.un", _CIL_NOARG,     "")
/// @brief 
defasm(REM,             0x0000,     "rem", _CIL_NOARG,     "")
/// @brief 
defasm(REM_UN,          0x0000,     "rem.un", _CIL_NOARG,     "")
/// @brief 
defasm(AND,             0x0000,     "and", _CIL_NOARG,     "")
/// @brief 
defasm(OR,              0x0000,     "or", _CIL_NOARG,     "")
/// @brief 
defasm(XOR,             0x0000,     "xor", _CIL_NOARG,     "")
/// @brief 
defasm(SHL,             0x0000,     "shl", _CIL_NOARG,     "")
/// @brief 
defasm(SHR,             0x0000,     "shr", _CIL_NOARG,     "")
/// @brief 
defasm(SHR_UN,          0x0000,     "shr.un", _CIL_NOARG,     "")
/// @brief 
defasm(NEG,             0x0000,     "neg", _CIL_NOARG,     "")
/// @brief 
defasm(NOT,             0x0000,     "not", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_I1,         0x0000,     "conv.i1", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_I2,         0x0000,     "conv.i2", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_I4,         0x0000,     "conv.i4", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_I8,         0x0000,     "conv.i8", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_R4,         0x0000,     "conv.r4", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_R8,         0x0000,     "conv.r8", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_U4,         0x0000,     "conv.u4", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_U8,         0x0000,     "conv.u8", _CIL_NOARG,     "")
/// @brief 
defasm(CALLVIRT,        0x0000,     "callvirt", _CIL_METHD,     "")
/// @brief 
defasm(CPOBJ,           0x0000,     "cpobj", _CIL_TYTOK,     "")
/// @brief 
defasm(LDOBJ,           0x0000,     "ldobj", _CIL_TYTOK,     "")
/// @brief 
defasm(LDSTR,           0x0000,     "ldstr", _CIL_STRNG,     "")
/// @brief 
defasm(NEWOBJ,          0x0000,     "newobj", _CIL_CCTOR,     "")
/// @brief 
defasm(CASTCLASS,       0x0000,     "castclass", _CIL_CLASS,     "")
/// @brief 
defasm(ISINST,          0x0000,     "isinst", _CIL_CLASS,     "")
/// @brief 
defasm(CONV_R_UN,       0x0000,     "conv.r.un", _CIL_NOARG,     "")
/// @brief 
defasm(UNBOX,           0x0000,     "unbox", _CIL_VALTY,     "")
/// @brief 
defasm(THROW,           0x0000,     "throw", _CIL_NOARG,     "")
/// @brief 
defasm(LDFLD,           0x0000,     "ldfld", _CIL_FIELD,     "")
/// @brief 
defasm(LDFLDA,          0x0000,     "ldflda", _CIL_FIELD,     "")
/// @brief 
defasm(STFLD,           0x0000,     "stfld", _CIL_FIELD,     "")
/// @brief 
defasm(LDSFLD,          0x0000,     "ldsfld", _CIL_FIELD,     "")
/// @brief 
defasm(LDSFLDA,         0x0000,     "ldsflda", _CIL_FIELD,     "")
/// @brief 
defasm(STSFLD,          0x0000,     "stsfld", _CIL_FIELD,     "")
/// @brief 
defasm(STOBJ,           0x0000,     "stobj", _CIL_TYTOK,     "")
/// @brief 
defasm(CONV_OVF_I1_UN,  0x0000,     "conv.ovf.i1.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I2_UN,  0x0000,     "conv.ovf.i2.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I4_UN,  0x0000,     "conv.ovf.i4.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I8_UN,  0x0000,     "conv.ovf.i8.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U1_UN,  0x0000,     "conv.ovf.u1.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U2_UN,  0x0000,     "conv.ovf.u2.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U4_UN,  0x0000,     "conv.ovf.u4.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U8_UN,  0x0000,     "conv.ovf.u8.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I_UN,   0x0000,     "conv.ovf.i.un", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U_UN,   0x0000,     "conv.ovf.u.un", _CIL_NOARG,     "")
/// @brief 
defasm(BOX,             0x0000,     "box", _CIL_TYTOK,     "")
/// @brief 
defasm(NEWARR,          0x0000,     "newarr", _CIL_TTYPE,     "")
/// @brief 
defasm(LDLEN,           0x0000,     "ldlen", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEMA,         0x0000,     "ldelema", _CIL_CLASS,     "")
/// @brief 
defasm(LDELEM_I1,       0x0000,     "ldelem.i1", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_U1,       0x0000,     "ldelem.u1", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_I2,       0x0000,     "ldelem.i2", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_U2,       0x0000,     "ldelem.u2", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_I4,       0x0000,     "ldelem.i4", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_U4,       0x0000,     "ldelem.u4", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_I8,       0x0000,     "ldelem.i8", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_U8,       0x0000,     "ldelem.u8", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_I,        0x0000,     "ldelem.i", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_R4,       0x0000,     "ldelem.r4", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_R8,       0x0000,     "ldelem.r8", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM_REF,      0x0000,     "ldelem.ref", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_I,        0x0000,     "stelem.i", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_I1,       0x0000,     "stelem.i1", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_I2,       0x0000,     "stelem.i2", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_I4,       0x0000,     "stelem.i4", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_I8,       0x0000,     "stelem.i8", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_R4,       0x0000,     "stelem.r4", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_R8,       0x0000,     "stelem.r8", _CIL_NOARG,     "")
/// @brief 
defasm(STELEM_REF,      0x0000,     "stelem.ref", _CIL_NOARG,     "")
/// @brief 
defasm(LDELEM,          0x0000,     "ldelem", _CIL_TYTOK,     "")
/// @brief 
defasm(STELEM,          0x0000,     "stelem", _CIL_TYTOK,     "")
/// @brief 
defasm(UNBOX_ANY,       0x0000,     "unbox.any", _CIL_TYTOK,     "")
/// @brief 
defasm(CONV_OVF_I1,     0x0000,     "conv.ovf.i1", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U1,     0x0000,     "conv.ovf.u1", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I2,     0x0000,     "conv.ovf.i2", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U2,     0x0000,     "conv.ovf.u2", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I4,     0x0000,     "conv.ovf.i4", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U4,     0x0000,     "conv.ovf.u4", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I8,     0x0000,     "conv.ovf.i8", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U8,     0x0000,     "conv.ovf.u8", _CIL_NOARG,     "")
/// @brief 
defasm(REFANYVAL,       0x0000,     "refanyval", _CIL_TTYPE,     "")
/// @brief 
defasm(CKFINITE,        0x0000,     "ckfinite", _CIL_NOARG,     "")
/// @brief 
defasm(MKREFANY,        0x0000,     "mkrefany", _CIL_CLASS,     "")
/// @brief 
defasm(LDTOKEN,         0x0000,     "ldtoken", _CIL_TOKEN,     "")
/// @brief 
defasm(CONV_U2,         0x0000,     "conv.u2", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_U1,         0x0000,     "conv.u1", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_I,          0x0000,     "conv.i", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_I,      0x0000,     "conv.ovf.i", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_OVF_U,      0x0000,     "conv.ovf.u", _CIL_NOARG,     "")
/// @brief 
defasm(ADD_OVF,         0x0000,     "add.ovf", _CIL_NOARG,     "")
/// @brief 
defasm(ADD_OVF_UN,      0x0000,     "add.ovf.un", _CIL_NOARG,     "")
/// @brief 
defasm(MUL_OVF,         0x0000,     "mul.ovf", _CIL_NOARG,     "")
/// @brief 
defasm(MUL_OVF_UN,      0x0000,     "mul.ovf.un", _CIL_NOARG,     "")
/// @brief 
defasm(SUB_OVF,         0x0000,     "sub.ovf", _CIL_NOARG,     "")
/// @brief 
defasm(SUB_OVF_UN,      0x0000,     "sub.ovf.un", _CIL_NOARG,     "")
/// @brief 
defasm(ENDFAULT,        0x0000,     "endfault", _CIL_NOARG,     "")
/// @brief 
defasm(ENDFINALLY,      0x0000,     "endfinally", _CIL_NOARG,     "")
/// @brief 
defasm(LEAVE,           0x0000,     "leave", _CIL_INT32,     "")
/// @brief 
defasm(LEAVE_S,         0x0000,     "leave.s", _CIL_INT08,     "")
/// @brief 
defasm(STIND_I,         0x0000,     "stind.i", _CIL_NOARG,     "")
/// @brief 
defasm(CONV_U,          0x0000,     "conv.u", _CIL_NOARG,     "")
/// @brief 
defasm(ARGLIST,         0xFE00,     "arglist", _CIL_NOARG,     "")
/// @brief 
defasm(CEQ,             0xFE00,     "ceq", _CIL_NOARG,     "")
/// @brief 
defasm(CGT,             0xFE00,     "cgt", _CIL_NOARG,     "")
/// @brief 
defasm(CGT_UN,          0xFE00,     "cgt.un", _CIL_NOARG,     "")
/// @brief 
defasm(CLT,             0xFE00,     "clt", _CIL_NOARG,     "")
/// @brief 
defasm(CLT_UN,          0xFE00,     "clt.un", _CIL_NOARG,     "")
/// @brief 
defasm(LDFTN,           0xFE00,     "ldftn", _CIL_METHD,     "")
/// @brief 
defasm(LDVIRTFTN,       0xFE00,     "ldvirtftn", _CIL_METHD,     "")
/// @brief 
defasm(LDARG,           0xFE00,     "ldarg", _CIL_INT16,     "")
/// @brief 
defasm(LDARGA,          0xFE00,     "ldarga", _CIL_INT16,     "")
/// @brief 
defasm(STARG,           0xFE00,     "starg", _CIL_INT16,     "")
/// @brief 
defasm(LDLOC,           0xFE00,     "ldloc", _CIL_INT16,     "")
/// @brief 
defasm(LDLOCA,          0xFE00,     "ldloca", _CIL_INT16,     "")
/// @brief 
defasm(STLOC,           0xFE00,     "stloc", _CIL_INT16,     "")
/// @brief 
defasm(LOCALLOC,        0xFE00,     "localloc", _CIL_NOARG,     "")
/// @brief 
defasm(ENDFILTER,       0xFE00,     "endfilter", _CIL_NOARG,     "")
/// @brief 
defasm(UNALIGNED,       0xFE00,     "unaligned.", _CIL_INSTR,     "")
/// @brief 
defasm(VOLATILE,        0xFE00,     "volatile.", _CIL_INSTR,     "")
/// @brief 
defasm(TAIL,            0xFE00,     "tail.", _CIL_INSTR,     "")
/// @brief 
defasm(INITOBJ,         0xFE00,     "initobj", _CIL_TYTOK,     "")
/// @brief 
defasm(CONSTRAINED,     0xFE00,     "constrained.", _CIL_TYTOK,     "")
/// @brief 
defasm(CPBLK,           0xFE00,     "cpblk", _CIL_NOARG,     "")
/// @brief 
defasm(INITBLK,         0xFE00,     "initblk", _CIL_NOARG,     "")
/// @brief 
defasm(NO,              0xFE00,     "no.", _CIL_INSTR,     "")
/// @brief 
defasm(RETHROW,         0xFE00,     "rethrow", _CIL_NOARG,     "")
/// @brief 
defasm(SIZEOF,          0xFE00,     "sizeof", _CIL_TYTOK,     "")
/// @brief 
defasm(REFANYTYPE,      0xFE00,     "refanytype", _CIL_NOARG,     "")
/// @brief 
defasm(READONLY,        0xFE00,     "readonly.", _CIL_INSTR,     "")

/* =------------------------------------------------------------= */

#ifdef defasm
#   undef defasm
#endif // UNDEF defasm
