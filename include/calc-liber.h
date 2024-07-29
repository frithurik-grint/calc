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
#   define _EXPORT
/// @brief Local API function specifier. (export)
#   define _LOC _INTERN
#else
/// @brief Public API function specifier. (import)
#   define _IMPORT
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

#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <ctype.h>
#include <stdio.h>

// +---- System Dependent Definitions --------------------------+

#pragma region System Dependent Definitions

#ifdef _WIN32

/* unistd.h useful ports on windows */

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
#   define malloca _alloca
#endif // alloca

#ifdef _WIN64
/// @brief Used for a count of bytes or an error indication.
#   define ssize_t  __int64
#else
/// @brief Used for a count of bytes or an error indication.
#   define ssize_t  long
#endif // _WIN64

/// @brief stdin fileno.
#define STDIN_FILENO  0
/// @brief stdout fileno.
#define STDOUT_FILENO 1
/// @brief stderr fileno.
#define STDERR_FILENO 2

CALC_C_HEADER_BEGIN

/// @brief Gets the system memory page size.
/// @return The system meory page size in bytes.
unsigned int _CDECL _getpagesiz();

CALC_C_HEADER_END

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

#include <unistd.h>

#ifndef pagesiz
/// @brief Gets the system memory page size.
#   define pagesiz sysconf(PAGESIZE)
#endif // pagesiz

#ifndef wordsiz
/// @brief Gets the system word size.
#   define wordsiz sizeof(void *)
#endif // wordsiz

#ifndef _OS_NAME
#   if   defined __linux__
#       define _OS_NAME "Linux"
#   elif defined __unix__
#       define _OS_NAME "Unix"
#   else
#       define _OS_NAME "uknown_os"
#   endif
#endif // _OS_NAME

#endif // _WIN32

#pragma endregion

// +---- System Dependent Definitions -- End -------------------+

// +---- System Outputting Functions ---------------------------+

#pragma region System Outputting Functions

CALC_C_HEADER_BEGIN

/// @brief Puts endline character on stream.
/// @param stream Stream on which print.
/// @return The number of characters written.
int _CDECL fputln(FILE *const stream);
/// @brief Puts endline character.
/// @return The number of characters written.
int _CDECL putln();

// Print Functions

/// @brief Prints a message on a stream.
/// @param stream Stream on which print.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL fprint(FILE *const stream, const char *const message);
/// @brief Prints a message on a stream on a
///        single line.
/// @param stream Stream on which print.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL fprintln(FILE *const stream, const char *const message);
/// @brief Prints a message on stdout.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL print(const char *const message);
/// @brief Prints a message on stdout on a line.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL println(const char *const message);

/// @brief Prints a formatted line on a stream.
/// @param stream Stream on which print.
/// @param format Format of the message to print.
/// @param arglist List of format arguments.
/// @return The number of characters written.
int _CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist);
/// @brief Prints a formatted line on a stream.
/// @param stream Stream on which print.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return The number of characters written.
int _CDECL fprintfn(FILE *const stream, const char *const format, ...);
/// @brief Prints a formatted line.
/// @param format Format of the message to print.
/// @param arglist List of format arguments.
/// @return The number of characters written.
int _CDECL vprintfn(const char *const format, va_list arglist);
/// @brief Prints a formatted line.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return The number of characters written.
int _CDECL printfn(const char *const format, ...);

// Error Print Functions

/// @brief Prints an error message on error stream.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL error(const char *const message);
/// @brief Prints an error line on error stream.
/// @param message Message to print.
/// @return The number of characters written.
int _CDECL errorln(const char *const message);

/// @brief Prints a formatted error message on error stream.
/// @param format Format of the error message to print.
/// @param arglist List of format arguments.
/// @return The number of characters written.
int _CDECL verrorf(const char *const format, va_list arglist);
/// @brief Prints a formatted error line on error stream.
/// @param format Format of the error message to print.
/// @param arglist List of format arguments.
/// @return The number of written characters.
int _CDECL verrorfn(const char *const format, va_list arglist);
/// @brief Prints a formatted error message on error stream.
/// @param format Format of the message to print.
/// @param others Fromat arguments.
/// @return The number of characters written.
int _CDECL errorf(const char *const format, ...);
/// @brief Prints a formatted error line on error stream.
/// @param format Format of the message to print.
/// @param others Format arguments.
/// @return The numbers of characters written.
int _CDECL errorfn(const char *const format, ...);

CALC_C_HEADER_END

#pragma endregion

// +---- System Outputting Functions --- End -------------------+ 

CALC_C_HEADER_BEGIN

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

/// @brief Allocates a block of memory in the heap.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the allocated
///			region of memory.
void *_CDECL malloc_s(size_t size);
/// @brief Allocates a series of contiguous blocks
///		   of memory in the heap.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return Pointer to the beginning of the first
///			allocated block
void *_CDECL calloc_s(size_t count, size_t size);

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

/// @brief Allocates a block of bytes, all set to
///		   zero.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the
///			allocated region of memory.
void *_CDECL mallocz_s(size_t size);
/// @brief Allocates a series of coniguous block of
///		   bytes, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block
///		   of memory.
/// @return Pointer to the first allocated block.
void *_CDECL callocz_s(size_t count, size_t size);

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

#ifndef alignto
/// @brief Align two sizes. Used in *alloca_s and *allocaz_s
///        functions.
#   define alignto(size, alignment) ((size + alignment - 1) & ~(alignment - 1))
#endif // alignto

/// @brief Allocates a block of memory in the heap,
///		   aligned to a specific width.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *_CDECL malloca_s(size_t size, size_t alignment);
/// @brief Allocates a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *_CDECL calloca_s(size_t count, size_t size, size_t alignment);

#ifndef alloca
/// @brief Allocate a new instance of the specified
///        type, aligned to another type.
#	define alloca(type1, type2) (type1 *)malloca_s(sizeof(type1), sizeof(type2))
#endif // alloca

#ifndef dima
/// @brief Allocates a new array of the specified type
///        with members aligned to another type.
#	define dima(type1, type2, count) (type1 *)calloca_s((count), sizeof(type1), sizeof(type2))
#endif // dima

/// @brief Allocates a block of memory in the heap,
///		   aligned to a specific width, all set to
///		   zero.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *_CDECL mallocaz_s(size_t size, size_t alignment);
/// @brief Allocates a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *_CDECL callocaz_s(size_t count, size_t size, size_t alignment);

#ifndef allocaz
/// @brief Allocates a new zero instance of the specified
///        type, aligned to another type.
#	define allocaz(type1, type2) (type1 *)mallocaz_s(sizeof(type1), sizeof(type2))
#endif // allocaz

#ifndef dimaz
/// @brief Allocates a new array of the specified type
///        with members aligned to another type all
///        set to zero.
#	define dimaz(type1, type2, count) (type1 *)callocaz_s((count), sizeof(type1), sizeof(type2))
#endif // dimaz

// Strings

/// @brief Allocates a new NUL terminated empty string.
/// @param length Number of characters of the string.
/// @return A pointer to the new allocated string.
char *_CDECL stralloc(size_t length);

#pragma endregion

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

#ifndef EXIT_SUCCESS
#   define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#   define EXIT_FAILURE 1
#endif

#ifndef EXIT_ABORTED
#   define EXIT_ABORTED 3
#endif

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

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

#ifndef numcmp
/// @brief Compares two generic numbers.
#   define numcmp(num1, num2)   ((num1) > (num2)) ? +1 : \
                                ((num1) < (num2)) ? -1 : 0
#endif // numcmp

// +---- Datatypes ---------------------------------------------+

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

// String

/// @brief Equals two strings.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t _CDECL streq(const char *const str1, const char *const str2);
/// @brief Equals two strings ignoring letters case.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t _CDECL strieq(const char *const str1, const char *const str2);

/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
char *_CDECL strntolower(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
char *_CDECL strtolower(char *const dest, const char *const source);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
char *_CDECL strntoupper(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
char *_CDECL strtoupper(char *const dest, const char *const source);

/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of character to copy.
/// @return A pointer to a new string or dest.
char *_CDECL strndcpy(char *const dest, const char *const source, size_t length);
/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to a new string or dest.
char *_CDECL strdcpy(char *const dest, const char *const source);

/// @brief Unescape a character.
/// @param str Source char.
/// @return The pointer to unescaped str.
char *_CDECL unesc(char *const dest, int c);

/// @brief Formats a format string with a list of args.
/// @param format String containing result format.
/// @param arglist List of arguments to use to format
///                the final value.
/// @return The formatted string.
char *_CDECL vformat(char *const dest, const char *const format, va_list arglist);
/// @brief Formats a format string with some arguments.
/// @param format String containing result format.
/// @param others List of arguments to use to format
///               the final value.
/// @return The formatted string.
char *_CDECL format(char *const dest, const char *const format, ...);

/// @brief Formats a format string with a list of args.
/// @param format String containing result format.
/// @param arglist List of arguments to use to format
///                the final value.
/// @return The formatted string.
char *_CDECL vformatn(const char *const format, va_list arglist);
/// @brief Formats a format string with some arguments.
/// @param format String containing result format.
/// @param others List of arguments to use to format
///               the final value.
/// @return The formatted string.
char *_CDECL formatn(const char *const format, ...);

#pragma endregion

// +---- Datatypes --------------------- End -------------------+

// +---- Datastructs (DSA) -------------------------------------+

#pragma region Datastructs

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

/// @brief Hashing function data type.
typedef hash_t(*hashfnc_t)(char *const);

// Hash Bucket

/// @brief Hash table bucket structure.
typedef struct _calc_hash_table_bucket
{
    /// @brief Key string.
    char *name;
    /// @brief Hash code of the bucket in
    ///        the table.
    hash_t hash;
    /// @brief Index of the datas in the
    ///        data record.
    unsigned int data;
    /// @brief Pointer to the colliding
    ///        bucket with the same hash.
    struct _calc_hash_table_bucket *next;
} hashbuc_t;

/// @brief Creates a new hash table bucket
///        with data address and colliding
///        bucket.
/// @param name Key string.
/// @param hash Computed hash code.
/// @param data Data address.
/// @param prev In case of collision this points
///             to the colliding bucket.
/// @return A pointer to the new hash table
///         bucket.
hashbuc_t *_CDECL create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev);
/// @brief Deletes the specified hash table
///        bucket, releasing its memory. (this
///        frees name pointer)
/// @param bucket Bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *_CDECL delete_hashbuc(hashbuc_t *const bucket);

// Hash Table

#ifndef CALC_HASHTAB_BUCKSNUM
/// @brief Default number of buckets in a chunk
///        of the hash table.
#   define CALC_HASHTAB_BUCKSNUM 0xFF
#endif // CALC_HASHTAB_BUCKSNUM

/// @brief Hash table data structure.
typedef struct _calc_hash_table
{
    /// @brief Array of pointers to the buckets.
    hashbuc_t **buck;
    /// @brief Hashing function for this table.
    hashfnc_t func;
    /// @brief Maximum number of buckets.
    unsigned int size;
    /// @brief Number of used buckets.
    unsigned int used;
    /// @brief Pointer to previous hash table
    ///        chunk.
    struct _calc_hash_table *prev;
} hashtab_t;

/// @brief Creates a new hash table with the
///        specified number of buckets.
/// @param size Numeber of buckets to allocate
///             in the chunk. (if size is 0 will
///             be allocated a chunk of
///             CALC_HASHTAB_BUCKSNUM buckets)
/// @param prev Pointer to the previous chunk.
/// @return A pointer to the new hash table chunk.
hashtab_t *_CDECL create_hashtab(unsigned int size, hashfnc_t func, hashtab_t *const prev);
/// @brief Deletes the specified hash table with
///        its buckets, releasing all used memory.
/// @param tab Pointer to table to delete.
/// @return A pointer to the previous hash table chunk.
hashtab_t *_CDECL delete_hashtab(hashtab_t *const tab);

/// @brief If specified key is not inserted in the hash table
///        chunk, inserts it, else gets it.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to add or get.
/// @return A pointer to the new (or old) hash bucket.
hashbuc_t *_CDECL hashtab_add(hashtab_t *const tab, char *const key);
/// @brief Gets bucket with the specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *_CDECL hashtab_get(hashtab_t *const tab, char *const key);
/// @brief Sets the data address of the bucket at the specified
///        key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @param data Data address to set to the found bucket.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *_CDECL hashtab_set(hashtab_t *const tab, char *const key, unsigned int data);

/// @brief Checks if hash table contains a specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to check.
/// @return TRUE if the hash table contains the value, else FALSE.
bool_t _CDECL hashtab_contains(hashtab_t *const tab, char *const key);

/// @brief Removes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to remove.
/// @return A pointer to the removed hash bucket.
hashbuc_t *_CDECL hashtab_remove(hashtab_t *const tab, char *const key);
/// @brief Deletes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *_CDECL hashtab_delete(hashtab_t *const tab, char *const key);

#ifndef _CALC_MINIMAL_BUILD

/// @brief Dumps hash table content on a stream.
/// @param stream Stream in which dump content of the hash
///               table.
/// @param tab Pointer to the last hash table chunk of the
///            hash table to dump on the selected stream.
void _CDECL hashtab_dump(FILE *const stream, hashtab_t *const tab);

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Datastructs ------------------- End -------------------+

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_LIBER_C_
#       include "calc-liber.c"
#   endif // CALC_LIBER_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_LIBER_H_
