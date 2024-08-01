#pragma once

/* stderr.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDERR_H_
#define CALC_BASE_STDERR_H_

#include "calc/base/stdout.h"

CALC_C_HEADER_BEGIN

/* =---- Common Error Output Functions -------------------------= */

// +---- Internal Macros ---------------------------------------+

#pragma region Internal Macros

#ifndef _stderr_error
#   define _stderr_error(message) fprint((message), stderr)
#endif // _stderr_error

#ifndef _stderr_errorln
#   define _stderr_errorln(message) fputs((message), stderr)
#endif // _stderr_errorln

#ifndef _stderr_verrorf
#   define _stderr_verrorf(format, arglist) vfprintf(stderr, (format), (arglist))
#endif // _stderr_verrorf

#ifndef _stderr_verrorfn
#   define _stderr_verrorfn(format, arglist) vfprintfn(stderr, (format), (arglist))
#endif // _stderr_verrorfn

#ifndef _stderr_errorf
#   define _stderr_errorf(format, ...) fprintf(stderr, (format), __VA_ARGS__)
#endif // _stderr_errorf

#ifndef _stderr_errorfn
#   define _stderr_errorfn(format, ...) fprintfn(stderr, (format), __VA_ARGS__)
#endif // _stderr_errorfn

#pragma endregion

// +---- Internal Macros ------------------------------ END ----+

/// @brief Prints a message on the standard error stream.
/// @param The message to print.
/// @return Number of characters written.
_INLINE int _CDECL error(const char *const message)
    _Return(_stderr_error(message))

/// @brief Prints a message on a line of the standard error stream.
/// @param The message to print.
/// @return Number of characters written.
_INLINE int _CDECL errorln(const char *const message)
    _Return(_stderr_errorln(message))

/// @brief Prints a formatted message on the standard error stream.
/// @param format Format of the message to print.
/// @param arglist Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL verrorf(const char *const format, va_list arglist)
    _Return(_stderr_verrorf(format, arglist))

/// @brief Prints a formatted message on a line of the standard error
///        stream.
/// @param format Format of the message to print.
/// @param arglist Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL verrorfn(const char *const format, va_list arglist)
    _Return(_stderr_verrorfn(format, arglist))

/// @brief Prints a formatted message on the standard error stream.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL errorf(const char *const format, ...)
    _VarArgReturn(format, _stderr_verrorf(format, _arglist))

/// @brief Prints a formatted message on a line of the standard error
///        stream.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL errorfn(const char *const format, ...)
    _VarArgReturn(format, _stderr_verrorfn(format, _arglist))

// Termination Functions

/// @brief Aborts execution with an error exiting formatted message.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return If this functions succeed doesn't return anything (calls
///         exit functions), else is returned the number of written
///         characters. (it that case there is a very strange problem)
int _CDECL fail(const char *const format, ...);

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDERR_H_
