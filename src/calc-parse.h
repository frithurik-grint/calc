#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_ 1

#include "calc-liber.h"

#include <stdio.h>

#ifndef EOL
/// @brief End of line character.
#   define EOL '\n'
#endif // EOL

#ifndef NUL
/// @brief NUL character.
#   define NUL '\0'
#endif // NUL

CALC_C_HEADER_BEGIN

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

/// @brief Double buffer structure.
typedef struct _calc_double_buffer
{
    /// @brief Buffer pointer.
    char *buf;
    /// @brief Current position.
    unsigned int pos;
    /// @brief Forward position.
    unsigned int fwd;
    /// @brief Length of the buffer.
    unsigned int len;
} doub_t;

/// @brief Create a new double buffered
///        stream from a buffer, if buffer is
///        void and length is zero will be
///        allocated a new buffer of BUFSIZ
///        characters.
/// @param buffer Source buffer. If it is null
///               will be allocated a new buffer
///               of length character.
/// @param length Max length of source buffer.
///               If length is zero, length
///               becomes BUFSIZ.
/// @return A pointer to the new 
doub_t *create_doub(char *const buffer, unsigned int length);

/// @brief Get top character.
/// @param buf Source buffer.
/// @return Top character of the buffer or
///         EOF if is at end.
int doub_topc(const doub_t *const buf);
/// @brief Get next character.
/// @param buf Source buffer.
/// @return Next character of the buffer or
///         EOF if is at end.
int doub_getc(doub_t *const buf);
/// @brief Set top character of the buffer.
/// @param buf Source buffer.
/// @param c Character to set.
/// @return c or EOF it's at end.
int doub_setc(doub_t *const buf, int c);
/// @brief Set next character of the buffer.
/// @param buf Source buffer.
/// @param c Character to set.
/// @return c or EOF it's at end.
int doub_putc(doub_t *const buf, int c);

/// @brief Get next line.
/// @param buf Source buffer.
/// @return Pointer to a new string
///         containing the next line.
char *doub_gets(char *const dest, doub_t *const buf);
/// @brief Set next line.
/// @param buf Source buffer.
/// @param str String to set.
/// @return str.
char *doub_puts(doub_t *const buf, char *const str);
/// @brief Chop buffer content from begin
///        to forward.
/// @param buf Source buffer.
/// @return A pointer to the chopped string.
char *doub_chop(doub_t *const buf);

/// @brief Advance position of buf.
/// @param buf Source buffer.
void doub_advance(doub_t *const buf);
/// @brief Retreat position of buf.
/// @param buf Source buffer.
void doub_retreat(doub_t *const buf);
/// @brief Reset position to the beginning
///        and restore initial buffer.
/// @param buf Source buffer.
void doub_rewind(doub_t *const buf);

/// @brief Get begin buffer.
/// @param buf Source buffer.
/// @return A pointer to the current begin
///         of the buffer.
char *doub_getbuf(doub_t *const buf);

#pragma endregion

// +---- Tokens

#pragma region Tokens

#ifndef DECSEP
/// @brief Decimal separator constant.
#   define DECSEP '.'
#endif // DECSEP

/// @brief Enumerate token codes. (prefixed
///        with TOK_)
typedef enum _calc_token_code
{
    /// @brief Not recognized or erroneus
    ///        token code.
    TOK_INVAL,

#pragma push_macro("deftok")

#ifndef deftok
#   define deftok(tok_name, tok_lexeme) TOK_ ## tok_name,
#endif

#include "calc-parse.inc"

#ifdef deftok
#   undef deftok
#endif

#pragma pop_macro("deftok")
} tokcode_t;

/// @brief Select keyword or identifier code.
/// @param lexeme Lexeme of the token.
/// @return Keyword code, or identifier code.
tokcode_t gettok_kword_or_id(const char *const lexeme);
/// @brief Scan next token in the source.
/// @param src Source buffer.
/// @param outlex Ouput lexeme.
/// @return Code of the scanned token.
tokcode_t gettok(doub_t *const src, char **const outlex);

/// @brief Record for toke infos.
typedef struct _calc_token
{
    /// @brief Lexeme value of the token,
    ///        is not NULL when token is
    ///        an identifier or a literal.
    char     *lexm;
    /// @brief Code of the token.
    tokcode_t code;
} tok_t;

/// @brief Create a new token.
/// @param code Token code.
/// @param lexeme Corresponding lexeme for
///        token parsing.
/// @return A pointer to the new token.
tok_t *create_token(tokcode_t const code, const char *const lexeme);

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_PARSE_H_