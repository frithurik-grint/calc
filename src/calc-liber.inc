/* calc-liber.inc - Copyright (c) 2024 Frithurik Grint */

/* =---- Exceptions --------------------------------------------= */

#ifndef CALC_EX_FORMAT_ERROR
#   define CALC_EX_PREFX_ERROR "error: "
#endif // CALC_EX_PREFX_ERROR

#ifndef CALC_EX_FORMAT_SYNTX
#   define CALC_EX_PREFX_SYNTX "syntax error: "
#endif // CALC_EX_PREFX_SYNTX

#ifndef defexc
/// @brief Placeholder declaration of defexc(...).
#   define defexc(name, handler, excode, format, argc)
#endif // defexc

/// @brief Invalid token exception.
defexc(SYNTX_INVAL, "InvalidTokenException",        0x0100, CALC_EX_PREFX_SYNTX "'%s' is not a valid token",    1)
/// @brief Expected token exception.
defexc(SYNTX_EXPCT, "ExpectedTokenException"        0x0110, CALC_EX_PREFX_SYNTX "expected %s (got '%s')",       2)
/// @brief Unexpected token exception.
defexc(SYNTX_UNEXP, "UnexpectedTokenException",     0x0120, CALC_EX_PREFX_SYNTX "unexpected '%s'",              1)

#ifdef defexc
#   undef defexc
#endif // UNDEF defexc

/* =------------------------------------------------------------= */