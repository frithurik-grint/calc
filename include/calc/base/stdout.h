#pragma once

/* stdout.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDOUT_H_
#define CALC_BASE_STDOUT_H_

#include "calc/base/stddef.h"

#include <stdio.h>

CALC_C_HEADER_BEGIN

/* =---- Output Functions --------------------------------------= */

#pragma region System Outputting Functions

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

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDOUT_H_
