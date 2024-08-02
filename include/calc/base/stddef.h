#pragma once

/* stddef.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDDEF_H_
#define CALC_BASE_STDDEF_H_

#ifndef _CRT_SECURE_NO_WARNINGS
/// @brief Disable CRT deprecations. (on windows)
#   define _CRT_SECURE_NO_WARNINGS 1
#endif // _CRT_SECURE_NO_WARNINGS

#include <stddef.h>

/* =---- Common Definitions ------------------------------------= */

#ifndef EXIT_SUCCESS
/// @brief Succeful exit code.
#   define EXIT_SUCCESS 0
#endif // EXIT_SUCCESS

#ifndef EXIT_FAILURE
/// @brief Failure exit code.
#   define EXIT_FAILURE 1
#endif // EXIT_FAILURE

#ifndef EXIT_ABORTED
/// @brief Aborted exit code.
#   define EXIT_ABORTED 3
#endif // EXIT_ABORTED

#ifdef __cplusplus
#   ifndef CALC_C_EXTERN
/// @brief This macro marks a C function definition for C++ compilers.
#       define CALC_C_EXTERN
#   endif // CALC_C_EXTERN

#   ifndef _CRT_BEGIN_C_HEADER
#       ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#           define CALC_C_HEADER_BEGIN extern "C" {
#       endif // CALC_C_HEADER_BEGIN

#       ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#           define CALC_C_HEADER_END }
#       endif // CALC_C_HEADER_END
#   else
#       ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers. (CRT base)
#           define CALC_C_HEADER_BEGIN _CRT_BEGIN_C_HEADER
#       endif // CALC_C_HEADER_BEGIN

#       ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers. (CRT base)
#           define CALC_C_HEADER_END _CRT_END_C_HEADER
#       endif // CALC_C_HEADER_END
#   endif // _CRT_BEGIN_C_HEADER
#else
#   ifndef CALC_C_EXTERN
/// @brief This macro marks a C function definition for C++ compilers.
#       define CALC_C_EXTERN
#   endif // CALC_C_EXTERN

#   ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#       define CALC_C_HEADER_BEGIN
#   endif // CALC_C_HEADER_BEGIN

#   ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#       define CALC_C_HEADER_END
#   endif // CALC_C_HEADER_END
#endif // __cplusplus

#ifdef _CALC_BUILD_API
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
#   define _API
/// @brief Local API function specifier. (import)
#   define _LOC _INTERN
#endif // _CALC_BUILD

#ifdef _WIN32
/// @brief cdecl calling convention specifier. (enabled)
#   define _CDECL __cdecl
#else
/// @brief cdecl calling convention specifier. (disabled)
#   define _CDECL
#endif // _CALC_USE_CDECL

#if _CALC_BUILD_INLINES
#   ifdef __GNUC__
/// @brief Marks a possible inline function. (enabled)
#       define _INLINE static inline
#   else
/// @brief Marks a possible inline function. (enabled)
#       define _INLINE inline
#   endif // __GNUC__
#else
/// @brief Marks a possible inline function. (disabled)
#   define _INLINE
#endif // _INLINE

// +---- System Dependent Definitions --------------------------+

#pragma region System Dependent Definitions

#ifdef _WIN32

/* unistd.h useful ports on windows */

#include <malloc.h>
#include <process.h>
#include <direct.h>
#include <io.h>

/// @brief Access function modes.
typedef enum _access_mode
{
    /// @brief Existance only mode.
    F_OK = 0,
    /// @brief Write access mode.
    W_OK = 2,
    /// @brief Read access mode.
    R_OK = 4,
    /// @brief Both write and read access modes.
    X_OK = W_OK | R_OK
} amode_t;

#ifndef access
/// @brief When used with files, the _access function determines
///        whether the specified file or directory exists and has
///        the attributes specified by the value of mode. When
///        used with directories, _access determines only whether
///        the specified directory exists; in Windows 2000 and
///        later operating systems, all directories have read and
///        write access.
/// @param path File or directory path.
/// @param mode Read/write attribute.
/// @return 0 if the file has the given mode. The function returns
///         -1 if the named file doesn't exist or doesn't have the
///         given mode.
#   define access _access
#endif // access

#ifndef malloca
/// @brief Allocates a block of memory on the stack.
/// @param size Number of bytes to allocate.
/// @return A pointer to the allocated block of memory.
#   define malloca _malloca
#endif // malloca

#ifdef _WIN64
/// @brief Used for a count of bytes or an error indication.
typedef __int64 ssize_t;
#else
/// @brief Used for a count of bytes or an error indication.
typedef long ssize_t;
#endif // _WIN64

/// @brief stdin fileno.
#define STDIN_FILENO  0
/// @brief stdout fileno.
#define STDOUT_FILENO 1
/// @brief stderr fileno.
#define STDERR_FILENO 2

/// @brief Gets the system memory page size.
/// @return The system meory page size in bytes.
_API CALC_C_EXTERN unsigned int _CDECL _getpagesiz();

#ifndef pagesiz
/// @brief Gets the system memory page size.
#   define pagesiz _getpagesiz()
#endif // pagesiz

#ifndef wordsiz
/// @brief Gets the system word size.
#   define wordsiz sizeof(void *)
#endif // wordsiz

#ifndef _OS_NAME
/// @brief Operating system name.
#   define _OS_NAME "Windows"
#endif // _OS_NAME

#else

#include <stdlib.h>
#include <unistd.h>

#ifndef pagesiz
/// @brief Gets the system memory page size.
#   define pagesiz sysconf(PAGESIZE)
#endif // pagesiz

#ifndef wordsiz
/// @brief Gets the system word size.
#   define wordsiz sizeof(void *)
#endif // wordsiz

#endif // _WIN32

#pragma endregion

// +---- System Dependent Definitions ----------------- END ----+

/* =------------------------------------------------------------= */

#endif // CALC_BASE_STDDEF_H_
