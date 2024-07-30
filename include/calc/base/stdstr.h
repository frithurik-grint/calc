#pragma once

/* stdstr.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDSTR_H_
#define CALC_BASE_STDSTR_H_

#include "calc/base/stddef.h"
#include "calc/base/stdmem.h"
#include "calc/base/types.h"

#include <string.h>
#include <ctype.h>

CALC_C_HEADER_BEGIN

/* =---- String Operatations Functions -------------------------= */

#pragma region String Operatations Functions

/// @brief Equals two strings.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
_API bool_t _CDECL streq(const char *const str1, const char *const str2);
/// @brief Equals two strings ignoring letters case.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
_API bool_t _CDECL strieq(const char *const str1, const char *const str2);

/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
_API char *_CDECL strntolower(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
_API char *_CDECL strtolower(char *const dest, const char *const source);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
_API char *_CDECL strntoupper(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
_API char *_CDECL strtoupper(char *const dest, const char *const source);

/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of character to copy.
/// @return A pointer to a new string or dest.
_API char *_CDECL strndcpy(char *const dest, const char *const source, size_t length);
/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to a new string or dest.
_API char *_CDECL strdcpy(char *const dest, const char *const source);

/// @brief Unescape a character.
/// @param str Source char.
/// @return The pointer to unescaped str.
_API char *_CDECL unesc(char *const dest, int c);

/// @brief Formats a format string with a list of args.
/// @param format String containing result format.
/// @param arglist List of arguments to use to format
///                the final value.
/// @return The formatted string.
_API char *_CDECL vformat(char *const dest, const char *const format, va_list arglist);
/// @brief Formats a format string with some arguments.
/// @param format String containing result format.
/// @param others List of arguments to use to format
///               the final value.
/// @return The formatted string.
_API char *_CDECL format(char *const dest, const char *const format, ...);

/// @brief Formats a format string with a list of args.
/// @param format String containing result format.
/// @param arglist List of arguments to use to format
///                the final value.
/// @return The formatted string.
_API char *_CDECL vformatn(const char *const format, va_list arglist);
/// @brief Formats a format string with some arguments.
/// @param format String containing result format.
/// @param others List of arguments to use to format
///               the final value.
/// @return The formatted string.
_API char *_CDECL formatn(const char *const format, ...);

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDSTR_H_
