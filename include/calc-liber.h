#pragma once

/* calc-liber.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_LIBER_H_
#define CALC_LIBER_H_

#ifndef _CRT_SECURE_NO_WARNINGS
/// @brief Avoid deprecation warnings.
#   define _CRT_SECURE_NO_WARNINGS 1
#endif // _CRT_SECURE_NO_WARNINGS

#ifdef __cplusplus
#   ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#       define CALC_C_HEADER_BEGIN extern "C" {
#   endif // CALC_C_HEADER_BEGIN

#   ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#       define CALC_C_HEADER_END }
#   endif // CALC_C_HEADER_END
#else
#   ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#       define CALC_C_HEADER_BEGIN
#   endif // CALC_C_HEADER_BEGIN

#   ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#       define CALC_C_HEADER_END
#   endif // CALC_C_HEADER_END
#endif // __cplusplus

#ifdef _DEBUG
#	ifndef CALC_DEBUG
/// @brief Internal debug flag.
#		define CALC_DEBUG 1
#	endif // CALC_DEBUG
#endif // _DEBUG

#ifdef _UNICODE
#	ifndef CALC_UNICODE
/// @biref Internal unicode charset flag.
#		define CALC_UNICODE 1
#	endif // CALC_UNICODE
#endif // _UNICODE

#ifdef _CALC_USE_DYNAMIC_LIBRARY
#   if defined _WIN32 || defined __CYGWIN__
#       ifdef __GNUC__
/// @brief Dynamic library function exportation. (__attribute__)
#           define _EXPORT __attribute__((dllexport))
/// @brief Dynamic library function importation. (__attribute__)
#           define _IMPORT __attribute__((dllimport))
/// @brief Dynamic library function hiding.
#           define _INTERN
#       else
/// @brief Dynamic library function exportation. (__declspec)
#           define _EXPORT __declspec(dllexport)
/// @brief Dynamic library function importation. (__declspec)
#           define _IMPORT __declspec(dllimport)
/// @brief Dynamic library function hiding.
#           define _INTERN
#       endif // __GNUC__
#   else
#       if __GNUC__ >= 4
/// @brief Dynamic library function exportation. (__attribute__)
#           define _EXPORT __attribute__((visibility("default")))
/// @brief Dynamic library function importation. (__attribute__)
#           define _IMPORT __attribute__((visibility("default")))
/// @brief Dynamic library function hiding. (__attribute__)
#           define _INTERN __attribute__((visibility("hidden")))
#       else
/// @brief Dynamic library function exportation.
#           define _EXPORT
/// @brief Dynamic library function importation.
#           define _IMPORT
/// @brief Dynamic library function hiding.
#           define _INTERN
#       endif // __GNUC__
#   endif // _WIN32 or __CYGWIN__
#else
/// Dynamic library function exportation. (not enabled)
#   define _EXPORT
/// Dynamic library function importation. (not enabled)
#   define _IMPORT
/// Dynamic library function hiding. (not enabled)
#   define _INTERN
#endif // _CALC_USE_DYNAMIC_LIBRARY

#ifdef _CALC_BUILD
/// @brief Public API function specifier. (export)
#   define _API _EXPORT
/// @brief Local API function specifier. (export)
#   define _LOC _INTERN
#else
/// @brief Public API function specifier. (import)
#   define _API _IMPORT
/// @brief Local API function specifier. (import)
#   define _LOC _INTERN
#endif // _CALC_BUILD

#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <ctype.h>
#include <stdio.h>

// +---- System Dependent Definitions -------------------------+

#pragma region System Dependent Definitions



#pragma endregion

// +---- System Dependent Definitions -- End ------------------+

// +---- System Outputting Functions --------------------------+

#pragma region System Outputting Functions

CALC_C_HEADER_BEGIN

_API int fputln(FILE *const stream);

// 

#pragma endregion

CALC_C_HEADER_END

// +---- System Outputting Functions --- End ------------------+ 

#endif // CALC_LIBER_H_