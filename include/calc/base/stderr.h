#pragma once

/* stderr.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDERR_H_
#define CALC_BASE_STDERR_H_

#include "calc/base/stddef.h"
#include "calc/base/stdout.h"
#include "calc/base/stdmem.h"

CALC_C_HEADER_BEGIN

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

/// @brief Exception kind enumeration.
typedef enum _calc_exception_kind
{
    /// @brief Internal exception. (from C code)
    EX_KIND_INTRNL,
    /// @brief Lexical exception.
    EX_KIND_LEXICL,
    /// @brief Syntactic exception.
    EX_KIND_SYNTCT,
} exkind_t;

/// @brief Common exception codes enumeration.
typedef enum _calc_exception_code
{
    /// @brief Unhandled exception code.
    EX_UNHDL = 0x1000,
    /// @brief Unreachable exception code.
    EX_UNRCH = 0x1001,
} excode_t;

/// @brief Exception informations record.
typedef struct _calc_exception
{
    /// @brief Name of the exception.
    char *name;
    /// @brief Error message of the exception.
    char *mssg;
    /// @brief Exception kind.
    exkind_t kind;
    /// @brief Unique identifier of the exception.
    excode_t code;
    /// @brief Function from which is thrown the
    ///        exception.
    char *func;
    /// @brief File (if known) from which is thrown
    ///        the exception.
    char *file;
    /// @brief Line (if known) from which is thrown
    ///        the exception.
    unsigned int line;
    /// @brief Previous exception in the exceptions
    ///        stack.
    struct _calc_exception *prev;
} ex_t;

/// @brief Creates a new exception informations record
///        to store informations on thrown exception.
/// @param message Error message.
/// @param code Unique code of the exception.
/// @return A pinter to the new allocated exception.
ex_t *_CDECL create_exception(char *const message, excode_t code);
/// @brief Creates a new exception informations record
///        to store informations on thrown exception,
///        included the location from where is thrown.
/// @param message Error message.
/// @param code Unique code of the exception.
/// @param func Function from where is thrown the exception.
/// @param file File from where is thrown the exception.
/// @param line Line from where is thrown the exception.
/// @return A pointer to the new allocated exception.
ex_t *_CDECL create_exception_located(char *const message, excode_t code, char *const func, char *const file, unsigned int line);
/// @brief Deletes an exception informations record.
/// @param exception Exception to delete.
ex_t *_CDECL delete_exception(ex_t *const exception);

// Exception Stack

/// @brief Thrown exceptions stack data structure.
typedef struct _calc_exception_stack
{
    /// @brief Exit code to return in main.
    unsigned int extcd;
    /// @brief Number of thrown exceptions.
    unsigned int count;
    /// @brief Pointer to the last thrown exception.
    ex_t *excpt;
} ex_stack_t;

/// @brief Creates a new exception stack.
/// @return A pointer to the new allocated exception stack.
ex_stack_t *_CDECL create_ex_stack();
/// @brief Deletes an exception stack.
/// @param stack Exceptions stack to delete.
void _CDECL delete_ex_stack(ex_stack_t *const stack);

#ifndef _CALC_MINIMAL_BUILD

/// @brief Dumps the exception stack trace on the
///        specified stream.
/// @param stream Stream on which dump the exception stack.
/// @param stack Thrown exceptions stack.
void _CDECL ex_stack_dump(FILE *const stream, ex_stack_t *const stack);

#endif // _CALC_MINIMAL_BUILD

/// @brief Pushes an exception on the thrown exceptions
///        stack.
/// @param stack Thrown exceptions stack.
/// @param exception Exception to push.
void _CDECL pushex(ex_stack_t *const stack, ex_t *const exception);
/// @brief Pops the last exception on the stack.
/// @param stack Thrown exceptions stack.
/// @return Pointer to the popped exception.
ex_t *_CDECL popex(ex_stack_t *const stack);
/// @brief Peeks the last exception on the stack.
/// @param stack Thrown exceptions stack.
/// @return Pointer to the last exception.
ex_t *_CDECL peekex(ex_stack_t *const stack);

/// @brief Pushes an exception on the exceptions stack.
/// @param stack Stack on which push the exception.
/// @param file File from which the exception has been throwed.
/// @param func Function from which the exception has been throwed.
/// @param line Line from which the exception has been throwed.
/// @param code Code of the exception.
/// @param message Error message.
void _CDECL _ex_throw(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const message);
/// @brief Pushes an exception on the exceptions stack.
/// @param stack Stack on which push the exception.
/// @param file File from which the exception has been throwed.
/// @param func Function from which the exception has been throwed.
/// @param line Line from which the exception has been throwed.
/// @param code Code of the exception.
/// @param format Format of the exception error message.
/// @param others Format messages.
void _CDECL _ex_throwf(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const format, ...);

#ifndef ex_throw
/// @brief Throws an exception on the specified exceptions stack.
/// @param stack Thrown exceptions stack.
/// @param code Exception code. (this defines name, kind and other exception infos)
/// @param message Error message to display.
#   define ex_throw(stack, code, message) _ex_throw(stack, __FILE__, __func__, __LINE__, code, message)
#endif // ex_throw

#ifndef ex_throwf
/// @brief Throws an exception on the specified exceptions stack,
///        with a formatted message.
/// @param stack Thrown exceptions stack.
/// @param code Exception code. (this defines name, kind and other exception infos)
/// @param format Format of the error message.
/// @param others Format arguments.
#   define ex_throwf(stack, code, format, ...) _ex_throwf(stack, __FILE__, __func__, __LINE__, code, format, __VA_ARGS__)
#endif // ex_throwf

/// @brief Exception catch callback function.
typedef int (*_calc_exception_callback)(ex_t *const), (*ex_callback_t)(ex_t *const);

/// @brief Catches the last thrown exception passing it to the
///        callback function.
/// @param stack Thrown exceptions stack.
/// @param code Code of the exception to catch.
/// @param callback Callback function.
/// @return The returned code from the callback function.
int _CDECL except(ex_stack_t *const stack, excode_t code, ex_callback_t callback);

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDERR_H_
