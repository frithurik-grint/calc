#pragma once

#ifndef CALC_H_
#define CALC_H_

#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS 1
#endif // _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

// +---- Common Macro Definitions ------------------------------+

#pragma region Common Macro Definitions

#ifndef EXIT_SUCCESS
#   define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#   define EXIT_FAILURE 1
#endif

#ifndef EXIT_ABORTED
#   define EXIT_ABORTED 3
#endif

#if (defined _WIN32 || defined __CYGWIN__) && !defined CALC_WINDOWS
#   define CALC_WINDOWS
#endif

#ifdef CALC_WINDOWS
#   ifndef CALC_EXPORT
#       ifdef __GNUC__
#           define CALC_EXPORT __attribute__((dllexport))
#       else
#           define CALC_EXPORT __declspec(dllexport)
#       endif
#   endif

#   ifndef CALC_IMPORT
#       ifdef __GNUC__
#           define CALC_IMPORT __attribute__((dllimport))
#       else
#           define CALC_IMPORT __declspec(dllimport)
#       endif
#   endif

#   ifndef CALC_INTERN
#       define CALC_INTERN
#   endif
#else
#   if __GNUC__ >= 4
#       ifndef CALC_EXPORT
#           define CALC_EXPORT __attribute__((visibility("default")))
#       endif

#       ifndef CALC_IMPORT
#           define CALC_IMPORT __attribute__((visibility("default")))
#       endif

#       ifndef CALC_INTERN
#           define CALC_INTERN __attribute__((visibility("hidden")))
#       endif
#   else
#       ifndef CALC_EXPORT
#           define CALC_EXPORT
#       endif

#       ifndef CALC_IMPORT
#           define CALC_IMPORT
#       endif

#       ifndef CALC_INTERN
#           define CALC_INTERN
#       endif
#   endif
#endif

#ifdef CALC_DLL
#   ifdef CALC_BUILD
#       ifndef CALC_API
#           define CALC_API CALC_EXPORT
#       endif
#   else
#       ifndef CALC_API
#           define CALC_API CALC_IMPORT
#       endif
#   endif

#       ifndef CALC_LOC
#           define CALC_LOC CALC_INTERN
#       endif

#   ifndef CALC_EXT
#       define CALC_EXT CALC_IMPORT
#   endif
#else
#   ifndef CALC_API
#       define CALC_API
#   endif

#   ifndef CALC_LOC
#       define CALC_LOC
#   endif

#   ifndef CALC_EXT
#       define CALC_EXT extern
#   endif
#endif

#ifndef CALC_INL
#   ifdef __GNUC__
#       define CALC_INL CALC_LOC static inline
#   else
#       define CALC_INL CALC_LOC inline
#   endif
#endif

#ifndef CALC_CDECL
#   ifdef CALC_WINDOWS
#       define CALC_CDECL __cdecl
#   else
#       define CALC_CDECL
#   endif
#endif

#pragma endregion

// +---- Common Macro Definitions --------------------- END ----+

#include <limits.h>
#include <float.h>

// +---- Common Datatype Definitions ---------------------------+

#pragma region Common Datatype Definitions

// Booleans

typedef unsigned char bool_t;

#ifndef TRUE
#   define TRUE ((bool_t)0x01U)
#endif

#ifndef FALSE 
#   define FALSE ((bool_t)0x00U)
#endif

// Bytes

typedef unsigned char byte_t;

#ifndef BYTE_MIN
#   define BYTE_MIN ((byte_t)0x00U)
#endif

#ifndef BYTE_MAX
#   define BYTE_MAX ((byte_t)0xFFU)
#endif

#pragma endregion

// +---- Common Datatype Definitions ------------------ END ----+

#include <stdarg.h>
#include <stdio.h>

// +---- Common Output Functions -------------------------------+

#pragma region Common Output Functions

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

CALC_INL int CALC_CDECL fputln(FILE *const stream)
    _Return(_stdout_fputln(stream))

CALC_INL int CALC_CDECL putln()
    _Return(_stdout_putln())

CALC_INL int CALC_CDECL fprint(FILE *const stream, const char *const message)
    _Return(_stdout_fprint(stream, message))

CALC_INL int CALC_CDECL fprintln(FILE *const stream, const char *const message)
    _Return(_stdout_fprintln(stream, message))

CALC_INL int CALC_CDECL print(const char *const message)
    _Return(_stdout_print(message))

CALC_INL int CALC_CDECL println(const char *const message)
    _Return(_stdout_println(message))

CALC_INL int CALC_CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist)
    _Return(_stdout_vfprintfn(stream, format, arglist))

CALC_INL int CALC_CDECL vprintfn(const char *const format, va_list arglist)
    _Return(_stdout_vprintfn(format, arglist))

CALC_INL int CALC_CDECL fprintfn(FILE *const stream, const char *const format, ...)
    _VarArgReturn(format, _stdout_vfprintfn(stream, format, _arglist))

CALC_INL int CALC_CDECL printfn(const char *const format, ...)
    _VarArgReturn(format, _stdout_vprintfn(format, _arglist))

#pragma endregion

// +---- Common Output Functions ---------------------- END ----+

#include <errno.h>

// +---- Common Errors Output Functions ------------------------+

#pragma region Common Errors Output Functions

// +---- Internal Macros ---------------------------------------+

#pragma region Internal Macros

#ifndef _stderr_error
#   define _stderr_error(message) fprint(stderr, (message))
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

CALC_INL int CALC_CDECL error(const char *const message)
    _Return(_stderr_error(message))

CALC_INL int CALC_CDECL errorln(const char *const message)
    _Return(_stderr_errorln(message))

CALC_INL int CALC_CDECL verrorf(const char *const format, va_list arglist)
    _Return(_stderr_verrorf(format, arglist))

CALC_INL int CALC_CDECL verrorfn(const char *const format, va_list arglist)
    _Return(_stderr_verrorfn(format, arglist))

CALC_INL int CALC_CDECL errorf(const char *const format, ...)
    _VarArgReturn(format, _stderr_verrorf(format, _arglist))

CALC_INL int CALC_CDECL errorfn(const char *const format, ...)
    _VarArgReturn(format, _stderr_verrorfn(format, _arglist))

// Termination Functions

CALC_INL int CALC_CDECL fail(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    result = _stderr_verrorfn(format, arglist);
    va_end(arglist);

#ifdef _DEBUG
    exit(EXIT_ABORTED);
#else
    abort();
#endif

    return result;
}

#pragma endregion

// +---- Common Errors Output Functions --------------- END ----+

#ifdef CALC_WINDOWS
#   include <malloc.h>
#endif

#include <string.h>
#include <ctype.h>

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

CALC_API void *CALC_CDECL calc_malloc(size_t size);
CALC_API void *CALC_CDECL calc_calloc(size_t count, size_t size);

#ifndef alloc
#   define alloc(type) (type*)calc_malloc(sizeof(type))
#endif

#ifndef dim
#   define dim(type, count) (type*)calc_calloc((count), sizeof(type))
#endif

CALC_API void *CALC_CDECL calc_malloc_z(size_t size);
CALC_API void *CALC_CDECL calc_calloc_z(size_t count, size_t size);

#ifndef allocz
#   define allocz(type) (type*)calc_malloc_z(sizeof(type))
#endif

#ifndef dimz
#   define dimz(type, count) (type*)calc_calloc_z((count), sizeof(type))
#endif

// Alignment

#ifndef alignto
#   define alignto(size, alignment) (((size) + (alignment) - 1) & ~((alignment) - 1))
#endif

#pragma endregion

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

#ifndef NUL
#   define NUL '\0'
#endif

// +---- String Operations -------------------------------------+

#pragma region String Operations

CALC_API char *CALC_CDECL strnget(char *const dst, const char *const src, size_t count);
CALC_API char *CALC_CDECL strnlow(char *const dst, const char *const src, size_t count);
CALC_API char *CALC_CDECL strnupp(char *const dst, const char *const src, size_t count);

CALC_API char *CALC_CDECL strget(char *const dst, const char *const src);
CALC_API char *CALC_CDECL strlow(char *const dst, const char *const src);
CALC_API char *CALC_CDECL strupp(char *const dst, const char *const src);

CALC_API bool_t CALC_CDECL streq(const char *const str1, const char *const str2);
CALC_API bool_t CALC_CDECL striq(const char *const str1, const char *const str2);

#pragma endregion

// +---- String Operations ---------------------------- END ----+

#pragma endregion

/* =---- Lexical Analyser --------------------------------------= */

#pragma region Lexical Anlyser

// +---- Source Stream -----------------------------------------+

#pragma region Source Stream

// Source Buffer

#pragma endregion

// +---- Source Stream -------------------------------- END ----+

#pragma endregion

/* =---- Syntactic Analyser ------------------------------------= */

#pragma region Syntactic Anlyser

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_C_
#   include "calc.c" // if is built only the header...
#endif // CALC_C_

#endif // CALC_H_
