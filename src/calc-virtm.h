#pragma once

/* calc-virtm.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_VIRTM_H_
#define CALC_VIRTM_H_ 1

#include "calc-liber.h"

#ifdef __cplusplus
namespace calc
{
using namespace std;
#endif

CALC_C_HEADER_BEGIN

/* =---- Environment -------------------------------------------= */

#pragma region Environment

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
/// @brief Allocate a new instance of a
///        symbol type.
/// @param type Type to allocate.
/// @return A pointer to the new allocated value.
byte_t *malloc_symb(const symb_t *const type);
/// @brief Allocate a packed instance ot
///        a symbol type. (allocate unaligned)
/// @param type 
/// @return 
byte_t *packlc_symb(const symb_t *const type);

/// @brief Get size of the symbol.
/// @param symb Symbol.
/// @return Size of symb.
unsigned int sizeof_symb(const symb_t *const symb);
/// @brief Get alignment of the symbol.
/// @param symb Symbol.
/// @return Alignment of symb.
unsigned int alignof_symb(const symb_t *const symb);

/// @brief Record for data layout.
typedef struct _calc_symbol_data
{
    /// @brief Name of the member.
    char *name;
    /// @brief Type of the member.
    symb_t *type;
} symbdata_t;

/// @brief Create a new symbol data (prototype).
/// @param name Name of the symbol.
/// @param type Type of the symbol.
/// @return A pointer to the new symbol data.
symbdata_t *create_symbdata(const char *const name, const symb_t *const type);
/// @brief Create a new array of symbol datas
///        (prototypes).
/// @param count Number of prototypes in the
///              array.
/// @param names Names of protorypes.
/// @param types Types of prototypes
/// @return A pointer to the first element of
///         the prototypes array.
symbdata_t *create_symbdata_array(unsigned int count, const char *const *const names, const symb_t *const *const types);
/// @brief Create a new array of symbol datas
///        (prototypes) using varargs. Useful
///        for built-in complex types.
/// @param count Number of 
/// @param others Pairs of name-type. E.g.
///               create_symbdata_va(2, "x", i32, "y", i32)
/// @return A pointer to the first element of
///         the prototypes array.
symbdata_t *create_symbdata_va(unsigned int count, ...);

/// @brief Get the size of prototype. 
/// @param data Prototype.
/// @return Size of prototype.
unsigned int sizeof_symbdata(const symbdata_t *const data);
/// @brief Get the size of prototype array.
/// @param count Number of prototypes.
/// @param data Prototypes.
/// @return Size of prototypes.
unsigned int sizeof_symbdata_array(unsigned int count, const symbdata_t *const data);

/// @brief Get the alignment of prototype. 
/// @param data Prototype.
/// @return Alignment of prototype.
unsigned int alignof_symbdata(const symbdata_t *const data);
/// @brief Get the alignment of prototype array.
/// @param count Number of prototypes.
/// @param data Prototypes.
/// @return Alignment of prototypes.
unsigned int alignof_symbdata_array(unsigned int count, const symbdata_t *const data);

// Specialized Symbols

struct _calc_symbol_datatype
{
    /// @brief Size of the type.
    unsigned int width;
    /// @brief Alignment of the type.
    unsigned int align;
};

/// @brief Create a new primitive data type.
/// @param name Name of the type.
/// @param width Width (not size, in bytes) of the type.
/// @param align Alignment (in bytes) of the type.
/// @return A pointer to the new symbol.
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

/// @brief Create a new structure data type.
/// @param name Name of the type.
/// @param width Width (not size, in bytes) of the type.
///              If it's uknown use create_symb_stype_autosiz.
/// @param align Alignment (in bytes) of the type.
///              If it's uknown use create_symb_stype_autosiz.
/// @param membc Number of members. (can be 0)
/// @param membv Members prototypes. (should be NULL only if
///              membc is 0)
/// @return A pointer to the new symbol.
symb_t *create_symb_stype(const char *const name, unsigned int width, unsigned int align, unsigned int membc, const symbdata_t *const membv);
/// @brief Create a new structure data type.
/// @param name Name of the type.
/// @param membc Number of members. (can be 0)
/// @param membv Members prototypes. (should be NULL only if
///              membc is 0)
/// @return A pointer to the new symbol.
symb_t *create_symb_stype_autosiz(const char *const name, unsigned int membc, const symbdata_t *const membv);

struct _calc_symbol_constant
{
    /// @brief Type symbol pointer.
    symb_t *type;
    /// @brief Raw data value.
    byte_t *data;
};

symb_t *create_symb_const(const char *const name, const symb_t *const type, const byte_t *const data);

struct _calc_symbol_variable
{
    /// @brief Type symbol pointer.
    symb_t *type;
    /// @brief Raw data value.
    byte_t *data;
};

symb_t *create_symb_local(const char *const name, const symb_t *const type);
symb_t *malloc_symb_local(const char *const name, const symb_t *const type);
symb_t *packlc_symb_local(const char *const name, const symb_t *const type);

struct _calc_symbol_function
{
    void *_;
};

symb_t *create_symb_funct(const char *const name, ...);

struct _calc_symbol_parameter
{
    void *_;
};

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

#endif // CALC_VIRTM_H_