#pragma once

/* calc-scrpt.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_SCRPT_H_
#define CALC_SCRPT_H_ 1

#include "calc-build.h"

#ifdef __cplusplus
namespace calc
{
#endif // __cplusplus

CALC_C_HEADER_BEGIN

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_SCRPT_C_
#       include "calc-scrpt.c"
#   endif // CALC_SCRPT_C_
#endif // _CALC_BUILD_AS_ONE

#endif