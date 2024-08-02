#pragma once

/* stdty.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDTY_H_
#define CALC_BASE_STDTY_H_

#include "calc/base/stddef.h"

#include <limits.h>
#include <stdint.h>
#include <float.h>

CALC_C_HEADER_BEGIN

/* =---- Standard Datatypes Header -----------------------------= */

#ifndef numcmp
/// @brief Compares two generic numbers.
#   define numcmp(num1, num2)   ((num1) > (num2)) ? +1 : \
                                ((num1) < (num2)) ? -1 : 0
#endif // numcmp

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

#ifndef hasflag
/// @brief Checks if an enumeration filed has a flag toggled.
#   define hasflag(value, bitflag) (((value) & (bitflag)) == (bitflag))
#endif // hasflag

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

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDTY_H_
