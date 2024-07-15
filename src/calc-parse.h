#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_ 1

#include "calc-liber.h"

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
#   ifndef CALC_PARSE_C_
#       include "calc-parse.c"
#   endif // CALC_PARSE_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_PARSE_H_