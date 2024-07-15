#pragma once

/* calc-shell.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_SHELL_H_
#define CALC_SHELL_H_ 1

#include "calc-build.h"

#ifdef __cplusplus
namespace calc::shell
{
using namespace calc::parser;
using namespace calc::vm;
using namespace calc::codegen;

using namespace std;
#endif // __cplusplus

CALC_C_HEADER_BEGIN

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _CALC_BUILD_AS_ONE
#   include "calc-shell.c"
#endif // _CALC_BUILD_AS_ONE

#endif