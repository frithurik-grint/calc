#pragma once

/*
 * MIT License
 *
 * Copyright (c) 2024 Frithurik Grint
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef CALC_H_
#define CALC_H_

#pragma warning(push)
#pragma warning(disable: 4090 4996)

#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stddef.h>

#ifdef _CRT_BEGIN_C_HEADER
#	ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
///        (based on CRT)
#		define CALC_C_HEADER_BEGIN _CRT_BEGIN_C_HEADER
#	endif // CALC_C_HEADER_BEGIN
#endif // _CRT_BEGIN_C_HEADER

#ifdef _CRT_END_C_HEADER
#	ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
///        (based on CRT)
#		define CALC_C_HEADER_END _CRT_END_C_HEADER
#	endif // CALC_C_HEADER_END
#endif // _CRT_END_C_HEADER

#ifdef _DEBUG
#	ifndef CALC_DEBUG
/// @brief Internal debug flag.
#		define CALC_DEBUG 1
#	endif // CALC_DEBUG
#endif // _DEBUG

#ifdef _UNICODE
#	ifndef CALC_UNICODE
#		define CALC_UNICODE 1
#	endif // CALC_UNICODE
#endif // _UNICODE

#ifdef _CALC_NO_PARSE_ENV
#   define CALC_PARSE_H_ 0
#endif

#ifdef _CALC_NO_VIRTM_ENV
#   define CALC_VIRTM_H_ 0
#endif

#include "calc-build.h"

#ifdef _CALC_NO_SCRPT_ENV
#   define CALC_SCRPT_H_ 0
#endif

#ifdef _CALC_NO_SHELL_ENV
#   define CALC_SHELL_H_ 0
#endif

#include "calc-scrpt.h"
#include "calc-shell.h"

#include <stdio.h>

CALC_C_HEADER_BEGIN

/* =---- Command Line Interface --------------------------------= */



/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#pragma warning(pop)

#endif