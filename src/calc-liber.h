#pragma once

/* calc-liber.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_LIBER_H_
#define CALC_LIBER_H_

#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS 1
#endif

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

#ifdef __cplusplus
#   include <cassert>
#   include <csignal>
#   include <cstring>

#   include <cstdlib>
#   include <cstdarg>

#   ifdef CALC_DEBUG
#       include <cstdio>
#   endif // CALC_DEBUG

namespace calc
{
using namespace std;
#else
#   include <assert.h>
#   include <signal.h>
#   include <string.h>

#   include <stdlib.h>
#   include <stdarg.h>

#   ifdef CALC_DEBUG
#       include <stdio.h>
#   endif // CALC_DEBUG
#endif

CALC_C_HEADER_BEGIN

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

/// @brief Allocate a block of memory in the heap.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the allocated
///			region of memory.
void *malloc_s(size_t size);
/// @brief Allocate a series of contiguous blocks
///		   of memory in the heap.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return Pointer to the beginning of the first
///			allocated block
void *calloc_s(size_t count, size_t size);

#ifndef alloc
/// @brief Allocate a new instance of the specified
///        type.
#	define alloc(type) (type *)malloc_s(sizeof(type))
#endif // alloc

#ifndef dim
/// @brief Allocate a new array of the specified type
///        the specified number of members
#	define dim(type, count) (type *)calloc_s((count), sizeof(type))
#endif // dim

/// @brief Allocate a block of bytes, all set to
///		   zero.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the
///			allocated region of memory.
void *mallocz_s(size_t size);
/// @brief Allocate a series of coniguous block of
///		   bytes, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block
///		   of memory.
/// @return Pointer to the first allocated block.
void *callocz_s(size_t count, size_t size);

#ifndef allocz
/// @brief Allocate a new zero or default instance
///        of a specified type.
#	define allocz(type) (type *)mallocz_s(sizeof(type))
#endif // allocz

#ifndef dimz
/// @brief Allocate a new array of type with all
///        members set to zero.
#	define dimz(type, count) (type *)callocz_s((count), sizeof(type))
#endif // dimz

/// @brief Allocate a block of memory in the heap,
///		   aligned to a specific width.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *malloca_s(size_t size, size_t alignment);
/// @brief Allocate a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *calloca_s(size_t count, size_t size, size_t alignment);

#ifndef alloca
/// @brief Allocate a new instance of the specified
///        type, aligned to another type.
#	define alloca(type1, type2) (type1 *)malloca_s(sizeof(type1), sizeof(type2))
#endif // alloca

#ifndef dima
/// @brief Allocate a new array of the specified type
///        with members aligned to another type.
#	define dima(type1, type2, count) (type1 *)calloca_s((count), sizeof(type1), sizeof(type2))
#endif // dima

/// @brief Allocate a block of memory in the heap,
///		   aligned to a specific width, all set to
///		   zero.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *mallocaz_s(size_t size, size_t alignment);
/// @brief Allocate a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *callocaz_s(size_t count, size_t size, size_t alignment);

#ifndef allocaz
/// @brief Allocate a new zero instance of the specified
///        type, aligned to another type.
#	define allocaz(type1, type2) (type1 *)mallocaz_s(sizeof(type1), sizeof(type2))
#endif // allocaz

#ifndef dimaz
/// @brief Allocate a new array of the specified type
///        with members aligned to another type all
///        set to zero.
#	define dimaz(type1, type2, count) (type1 *)callocaz_s((count), sizeof(type1), sizeof(type2))
#endif // dimaz

#pragma endregion

/* =---- Datatypes Management ----------------------------------= */

#pragma region Datatypes Management

// Boolean

#ifndef CALC_BOOL_T
/// @brief This macro allow boolean data type modification.
#   define CALC_BOOL_T unsigned char
#endif // CALC_BOOL_T

/// @brief Boolean data type, can be only set to TRUE
///        or to FALSE.
typedef CALC_BOOL_T bool_t;

#ifndef TRUE
/// @brief This macro provides true value (1) for bool_t
///        data type.
#   define TRUE ((bool_t)0x01)
#endif // TRUE

#ifndef FALSE
/// @brief This macro provides false value (0) for bool_t
///        data type.
#   define FALSE ((bool_t)0x00)
#endif // FALSE

// Byte

#ifndef CALC_BYTE_T
/// @brief This macro allow byte data type modification.
#   define CALC_BYTE_T unsigned char
#endif // CALC_BYTE_T

/// @brief Byte data type, its value must be into BYTE_MIN
///        and BYTE_MAX range.
typedef CALC_BYTE_T byte_t;

#ifndef BYTE_MIN
/// @brief This macro provides the minimun value for
///        byte_t data type.
#   define BYTE_MIN ((byte_t)0x00)
#endif // BYTE_MIN

#ifndef BYTE_MAX
/// @brief This macro provides the maximum value for
///        byte_t data type.
#   define BYTE_MAX ((byte_t)0xFF)
#endif // BYTE_MAX

// String

char *strdcpy(char *const dest, const char *const source, size_t length);

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_LIBER_C_
#       include "calc-liber.c"
#   endif // CALC_LIBER_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_LIBER_H_