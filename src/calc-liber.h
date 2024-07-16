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

#include <assert.h>
#include <signal.h>
#include <string.h>

#include <stdlib.h>
#include <stdarg.h>

#include <ctype.h>

#ifdef CALC_DEBUG
#   include <stdio.h>
#endif // CALC_DEBUG

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

/// @brief Allocate a new empty string.
/// @param length Size of the string.
/// @return A pointer to the new string.
char *strloc(size_t length);

/// @brief Equals two strings.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t streq(const char *const str1, const char *const str2);
/// @brief Equals two strings ignoring letters case.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t strieq(const char *const str1, const char *const str2);

/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of character to copy.
/// @return A pointer to a new string or dest.
char *strdcpy(char *const dest, const char *const source, size_t length);

#pragma endregion

/* =---- Symbols Management ------------------------------------= */

#pragma region Symbols Management

/// @brief Hash code data type.
typedef unsigned int hash_t;

#ifndef HASH_MIN
/// @brief Minimum hash code value.
#   define HASH_MIN ((hash_t)0x00)
#endif // HASH_MIN

#ifndef HASH_MAX
/// @brief Maximum hash code value.
#   define HASH_MAX ((hash_t)-2)
#endif // HASH_MAX

#ifndef HASH_INV
/// @brief Erroneus hash code value.
#   define HASH_INV ((hash_t)-1)
#endif // HASH_INV

/// @brief Compute hash code of a string.
/// @param str Input string.
/// @return Computed hash code.
hash_t gethash(const char *const str);

// Symbol Table

/// @brief Access record of a symbol.
typedef struct _calc_symbol_key
{
    /// @brief Name of the symbol.
    char                    *name;
    /// @brief References counter, it can be
    ///        useful for garbage collection.
    unsigned int             refs;
    /// @brief Index of symbol datas in the
    ///        symbols record.
    unsigned int             data;
    /// @brief Pointer to the collided symbol
    ///        with the same hash.
    struct _calc_symbol_key *next;
} symbkey_t;

/// @brief Create a new symbol table key record.
/// @param name Name of the symbol.
/// @param data Index in symbol record.
/// @param prev In case of collision this points
///        to the other item.
/// @return A pointer to the new symbol key.
symbkey_t *create_symbkey(const char *const name, unsigned int data, symbkey_t *const prev);

#ifndef CALC_SYMBTAB_CHUNKSIZ
/// @brief Number of symbols in a chunk.
#   define CALC_SYMBTAB_CHUNKSIZ (BUFSIZ >> 4)
#endif // CALC_SYMBTAB_CHUNKSIZ

/// @brief Symbol table.
typedef struct _calc_symbol_table
{
    /// @brief Pointer to first key.
    symbkey_t                 *keys;
    /// @brief Number of used hashes.
    unsigned int               size;
    /// @brief Last allocated hash.
    hash_t                     last;
    /// @brief Previous symbol table chunk.
    struct _calc_symbol_table *prev;
} symbtab_t;

/// @brief Create a symbol table.
/// @param size Number of symbol key records.
/// @param prev Previous symbol table chunk.
/// @return A pointer to the new symbol table
symbtab_t *create_symbtab(unsigned int size, symbtab_t *const prev);

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_LIBER_C_
#       include "calc-liber.c"
#   endif // CALC_LIBER_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_LIBER_H_