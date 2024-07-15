#pragma once

/* calc-virtm.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_VIRTM_H_
#define CALC_VIRTM_H_

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

#   include <cstdlib>
#   include <cstdarg>

#   ifdef CALC_DEBUG
#       include <cstdio>
#   endif // CALC_DEBUG

namespace calc::vm
{
    using namespace std;
#else
#   include <assert.h>
#   include <signal.h>

#   include <stdlib.h>
#   include <stdarg.h>

#   ifdef CALC_DEBUG
#       include <stdio.h>
#   endif // CALC_DEBUG
#endif

CALC_C_HEADER_BEGIN

/* =---- Internal Memory Management ----------------------------= */

#pragma region Internal Memory Management

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

/* =---- Environment -------------------------------------------= */

#pragma region Environment

// +---- Datatypes

#pragma region Datatypes

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

#pragma endregion

// +---- Symbol Table

#pragma region Symbol Table

/// @brief Record for primitive types infos.
typedef struct _calc_symbol_datatype  symb_dtype_t;
/// @brief Record for structure types infos.
typedef struct _calc_symbol_structure symb_stype_t;
/// @brief Record for constants infos.
typedef struct _calc_symbol_constant  symb_const_t;
/// @brief Record for variables infos.
typedef struct _calc_symbol_variable  symb_local_t;
/// @brief Record for functions infos.
typedef struct _calc_symbol_function  symb_funct_t;
/// @brief Record for parameters infos.
typedef struct _calc_symbol_parameter symb_param_t;

// Generic Symbols

/// @brief Union type of all possible symbols.
typedef union _calc_symbol_addr
{
    /// @brief Datatype symbol infos.
    symb_dtype_t *datatype;
    /// @brief Structure symbol infos.
    symb_stype_t *structure;
    /// @brief Constant symbol infos.
    symb_const_t *constant;
    /// @brief Variable symbol infos.
    symb_local_t *variable;
    /// @brief Function symbol infos.
    symb_funct_t *function;
    /// @brief Parameter symbol infos.
    symb_param_t *parameter;
} symbaddr_t;

/// @brief Enumerate symbol kinds.
typedef enum _calc_symbol_kind
{
    /// @brief Undefined symbol kind.
    SYMB_UNDEF,
    /// @brief Datatype symbol kind.
    SYMB_DTYPE,
    /// @brief Structure symbol kind.
    SYMB_STYPE,
    /// @brief Constant symbol kind.
    SYMB_CONST,
    /// @brief Variable symbol kind.
    SYMB_LOCAL,
    /// @brief Function symbol kind.
    SYMB_FUNCT,
    /// @brief Parameter symbol kind.
    SYMB_PARAM,
} symbkind_t;

/// @brief Check if kind is of a symbol that
///        can be used as type.
/// @param kind Kind to test.
/// @return TRUE in case of success, else FALSE.
bool_t is_type_kind(const symbkind_t kind);

/// @brief Record of symbol infos.
typedef struct _calc_symbol
{
    /// @brief Symbol name.
    char        *name;
    /// @brief Union of pointers to symbol
    ///        specific infos.
    symbaddr_t   addr;
    /// @brief Kind of the symbol, useful
    ///        to choose which field of
    ///        addr use.
    symbkind_t   kind;
    /// @brief References to the symbol,
    ///        useful for garbage collection.
    unsigned int refs;
} symb_t;

/// @brief Create a new specialized symbol.
/// @param name Name of the symbol.
/// @param kind Kind of the symbol.
/// @return A pointer to new symbol.
symb_t *create_symb(const char *const name, const symbkind_t kind);
byte_t *malloc_symb(const symb_t *const type);

/// @brief Get size of the symbol.
/// @param symb Symbol.
/// @return Size of symb.
unsigned int sizeof_symb(const symb_t *const symb);
/// @brief 
/// @param symb 
/// @return 
unsigned int alignof_symb(const symb_t *const symb);

/// @brief Record for data layout.
typedef struct _calc_symbol_data
{
    /// @brief Name of the member.
    char *name;
    /// @brief Type of the member.
    symb_t *type;
} symbdata_t;

/// @brief 
/// @param name 
/// @param type 
/// @return 
symbdata_t *create_symbdata(const char *const name, const symb_t *const type);
/// @brief 
/// @param count 
/// @param  
/// @return 
symbdata_t *create_symbdata_v(unsigned int count, ...);

/// @brief 
/// @param data 
/// @return 
unsigned int sizeof_symbdata(const symbdata_t *const data);
/// @brief 
/// @param count 
/// @param data 
/// @return 
unsigned int sizeof_symbdata_v(unsigned int count, const symbdata_t *const data);

/// @brief 
/// @param data 
/// @return 
unsigned int alignof_symbdata(const symbdata_t *const data);
/// @brief 
/// @param count 
/// @param data 
/// @return 
unsigned int alignof_symbdata_v(unsigned int count, const symbdata_t *const data);

// Specialized Symbols

struct _calc_symbol_datatype
{
    /// @brief Size of the type.
    unsigned int width;
    /// @brief Alignment of the type.
    unsigned int align;
};

/// @brief 
/// @param name 
/// @param width 
/// @param align 
/// @return 
symb_t *create_symb_dtype(const char *const name, unsigned int width, unsigned int align);

struct _calc_symbol_structure
{
    /// @brief Size of the type.
    unsigned int width;
    /// @brief Alignment of the type.
    unsigned int align;
    /// @brief Count of members.
    unsigned int membc;
    /// @brief Data layouts of members.
    symbdata_t  *membv;
};

/// @brief 
/// @param name 
/// @param layout 
/// @return 
symb_t *create_symb_stype(const char *const name, unsigned int membc, unsigned int width, unsigned int align, const symbdata_t *const membv);

struct _calc_symbol_constant
{
    /// @brief Type symbol pointer.
    symb_t *type;
    /// @brief Raw data value.
    byte_t *data;
};

/// @brief 
/// @param name 
/// @param type 
/// @return 
symb_t *create_symb_const(const char *const name, const symb_t *const type);

struct _calc_symbol_variable
{
    void *_;
};

/// @brief 
/// @param name 
/// @param type 
/// @return 
symb_t *create_symb_local(const char *const name, ...);

struct _calc_symbol_function
{
    void *_;
};

/// @brief 
/// @param name 
/// @param type 
/// @return 
symb_t *create_symb_funct(const char *const name, ...);

struct _calc_symbol_parameter
{
    void *_;
};

/// @brief 
/// @param name 
/// @param type 
/// @return 
symb_t *create_symb_param(const char *const name, ...);

// Symbol Table

#ifndef CALC_SYMBOLTAB_MAX
/// @brief Maximum number of symbols.
#   define CALC_SYMBOLTAB_MAX 0xFFFF
#endif // CALC_SYMBOLTAB_MAX

#ifndef CALC_SYMBOLTAB_SIZ
/// @brief Symbol table growth factor.
#   define CALC_SYMBOLTAB_SIZ 0x0100
#endif // CALC_SYMBOLTAB_MAX

#pragma endregion

// +---- Call Stack

#pragma region Call Stack

#pragma endregion

// +---- Evaluation Stack

#pragma region Evaluation Stack

#ifndef CALC_EVALSTACK_MAX
#   define CALC_EVALSTACK_MAX 0x00FF
#endif // CALC_EVALSTACK_MAX

#pragma endregion

// +---- Environment

#pragma endregion

/* =---- Bytecode ----------------------------------------------= */

#pragma region Bytecode

// +---- Source Stream

#pragma region Source Stream

#pragma endregion

#pragma endregion

/* =---- Hybrid Virtual Machine --------------------------------= */

#pragma region Hybrid Virtual Machine

// +---- Register Record

#pragma region Register Record

#pragma endregion

// +---- Context

#pragma region Context

#pragma endregion

// +---- Control Unit

#pragma region Control Unit

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif

#ifdef _CALC_BUILD_AS_ONE
#   include "calc-virtm.c"
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_VIRTM_H_