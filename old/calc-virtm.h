#pragma once

/* calc-virtm.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_VIRTM_H_
#define CALC_VIRTM_H_ 1

#include "calc-liber.h"

CALC_C_HEADER_BEGIN

/* =---- Environment -------------------------------------------= */

#pragma region Environment

// +---- Symbol Table

#pragma region Symbol Table

// Symbol Table

#ifndef CALC_SYMBOLTAB_MAX
/// @brief Maximum number of symbols.
#   define CALC_SYMBOLTAB_MAX 0xFFFF
#endif // CALC_SYMBOLTAB_MAX

#ifndef CALC_SYMBOLTAB_SIZ
/// @brief Symbol table growth factor.
#   define CALC_SYMBOLTAB_SIZ 0x0100
#endif // CALC_SYMBOLTAB_MAX

#pragma endregion

// +---- Evaluation Stack

#pragma region Evaluation Stack

#ifndef CALC_EVALSTACK_MAX
#   define CALC_EVALSTACK_MAX 0x00FF
#endif // CALC_EVALSTACK_MAX



#pragma endregion

// +---- Call Stack

#pragma region Call Stack

#pragma endregion

// +---- Environment

#pragma endregion

/* =---- Bytecode ----------------------------------------------= */

#pragma region Bytecode

// +---- Source Stream

#pragma region Source Stream

#pragma endregion

#pragma endregion

/* =---- Hybrid Virtual Machine --------------------------------= */

#pragma region Hybrid Virtual Machine

// +---- Register Record

#pragma region Register Record

#pragma endregion

// +---- Context

#pragma region Context

#pragma endregion

// +---- Control Unit

#pragma region Control Unit

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_VIRTM_H_