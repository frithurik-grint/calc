#pragma once

/* calc-build.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BUILD_H_
#define CALC_BUILD_H_

#include "calc-parse.h"
#include "calc-virtm.h"

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
#   ifndef CALC_BUILD_C_
#       include "calc-build.c"
#   endif // CALC_BUILD_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_BUILD_H_