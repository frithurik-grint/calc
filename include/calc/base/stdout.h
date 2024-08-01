#pragma once

/* stdout.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDOUT_H_
#define CALC_BASE_STDOUT_H_

#include "calc/base/stddef.h"

#include <stdarg.h>
#include <stdio.h>

CALC_C_HEADER_BEGIN

/* =---- Common Output Functions -------------------------------= */

// +---- Internal Macros ---------------------------------------+

#pragma region Internal Macros

// Printing Macros

#ifndef _stdout_fputln
#   define _stdout_fputln(stream) (fputc('\n', (stream)) - ('\n' - 1))
#endif // _stdout_fputln

#ifndef _stdout_putln
#   define _stdout_putln() puts("")
#endif // _stdout_putln

#ifndef _stdout_fprint
#   define _stdout_fprint(stream, message) fprintf((stream), "%s", (message))
#endif // _stdout_fprint

#ifndef _stdout_fprintln
#   define _stdout_fprintln(stream, message) fputs((message), (stream))
#endif // _stdout_fprintln

#ifndef _stdout_print
#   define _stdout_print(message) printf("%s", (message))
#endif // _stdout_print

#ifndef _stdout_println
#   define _stdout_println(message) puts((message))
#endif // _stdout_println

#ifndef _stdout_vfprintfn
#   define _stdout_vfprintfn(stream, format, arglist) (vfprintf((stream), (format), (arglist)) + _stdout_fputln(stream))
#endif // _stdout_fprint

#ifndef _stdout_vprintfn
#   define _stdout_vprintfn(format, arglist) (vprintf((format), (arglist)) + _stdout_putln())
#endif // _stdout_vprintfn

#ifndef _stdout_fprintfn
#   define _stdout_fprintfn(stream, format, ...) (fprintf((stream), (format), __VA_ARGS__) + _stdout_fputln(stream))
#endif // _stdout_fprint

#ifndef _stdout_printfn
#   define _stdout_printfn(format, ...) (printf((format), __VA_ARGS__) + _stdout_putln())
#endif // _stdout_printfn

// Inlining Macros

#ifndef _Return
#   if _CALC_BUILD_INLINES
#       define _Return(result) \
{                              \
    return (result);           \
}
#   else
#       define _Return(result) ;
#   endif // _CALC_BUILD_INLINES
#endif // _Return

#ifndef _VarArgReturn
#   if _CALC_BUILD_INLINES
#       define _VarArgReturn(x, result) \
{                                       \
    int _result;                        \
    va_list _arglist;                   \
                                        \
    va_start(_arglist, x);              \
    _result = (result);                 \
    va_end(_arglist);                   \
                                        \
    return _result;                     \
}
#   else
#       define _VarArgReturn(x, result) ;
#   endif // _CALC_BUILD_INLINES
#endif // _VarArgReturn

#pragma endregion

// +---- Internal Macros ------------------------------ END ----+

/// @brief Puts an empty line in the specified stream.
/// @param stream Stream pointer on which put the empty
///               line.
/// @return Number of characters written.
_INLINE int _CDECL fputln(FILE *const stream)
    _Return(_stdout_fputln(stream))

/// @brief Puts an empty line in standard output stream.
/// @return Number of characters written.
_INLINE int _CDECL putln()
    _Return(_stdout_putln())

/// @brief Prints a message in the specified stream.
/// @param stream Stream pointer on which print the message.
/// @param message Message to print.
/// @return Number of characters written.
_INLINE int _CDECL fprint(FILE *const stream, const char *const message)
    _Return(_stdout_fprint(stream, message))

/// @brief Prints a message on a line in the specified stream.
/// @param stream Stream pointer on which print the message.
/// @param message Message to print.
/// @return Number of characters written.
_INLINE int _CDECL fprintln(FILE *const stream, const char *const message)
    _Return(_stdout_fprintln(stream, message))

/// @brief Prints a message in standard output stream.
/// @param message Message to print.
/// @return Number of characters written.
_INLINE int _CDECL print(const char *const message)
    _Return(_stdout_print(message))

/// @brief Prints a message on a line in standard output stream.
/// @param message Message to print.
/// @return Number of characters written.
_INLINE int _CDECL println(const char *const message)
    _Return(_stdout_println(message))

/// @brief Prints a formatted message on a line in a stream.
/// @param stream Stream pointer on which print the message.
/// @param format Format of the message to print.
/// @param arglist Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist)
    _Return(_stdout_vfprintfn(stream, format, arglist))

/// @brief Prints a formatted message on a line in starndard output stream.
/// @param format Format of the message to print.
/// @param arglist Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL vprintfn(const char *const format, va_list arglist)
    _Return(_stdout_vprintfn(format, arglist))

/// @brief Prints a formatted message on a line in a stream.
/// @param stream Stream pointer on which print the message.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL fprintfn(FILE *const stream, const char *const format, ...)
    _VarArgReturn(format, _stdout_vfprintfn(stream, format, _arglist))

/// @brief Prints a formatted message on a line in starndard output stream.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return Number of characters written.
_INLINE int _CDECL printfn(const char *const format, ...)
    _VarArgReturn(format, _stdout_vprintfn(format, _arglist))

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDOUT_H_
