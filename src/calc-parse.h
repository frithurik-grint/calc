#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_ 1

#include "calc-liber.h"

#include <stdio.h>

#ifndef EOF
/// @brief End of file character.
#   define EOF (-1)
#endif // EOF

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
int dtopc(const doub_t *const buf);
/// @brief Get next character.
/// @param buf Source buffer.
/// @return Next character of the buffer or
///         EOF if is at end.
int dgetc(doub_t *const buf);
/// @brief Set top character of the buffer.
/// @param buf Source buffer.
/// @param c Character to set.
/// @return c or EOF it's at end.
int dsetc(doub_t *const buf, int c);
/// @brief Set next character of the buffer.
/// @param buf Source buffer.
/// @param c Character to set.
/// @return c or EOF it's at end.
int dputc(doub_t *const buf, int c);

/// @brief Get next line.
/// @param buf Source buffer.
/// @return Pointer to a new string
///         containing the next line.
char *dgets(char *const dest, doub_t *const buf);
/// @brief Set next line.
/// @param buf Source buffer.
/// @param str String to set.
/// @return str.
char *dputs(doub_t *const buf, char *const str);
/// @brief Chops buffer content from begin
///        to forward.
/// @param buf Source buffer.
/// @return A pointer to the chopped string.
char *dchop(doub_t *const buf);
/// @brief Chops buffer content from begin
///        to forward and copy its value in
///        dest.
/// @param buf Source buffer.
/// @param dest Destination buffer.
/// @return A pointer to the chopped string (dest)
///         or null if fails.
char *dchopto(doub_t *const buf, char *const dest);

/// @brief Advance position of buf.
/// @param buf Source buffer.
void dadvance(doub_t *const buf);
/// @brief Retreat position of buf.
/// @param buf Source buffer.
void dretreat(doub_t *const buf);
/// @brief Reset position to the beginning
///        and restore initial buffer.
/// @param buf Source buffer.
void drewind(doub_t *const buf);

/// @brief Get begin buffer.
/// @param buf Source buffer.
/// @return A pointer to the current begin
///         of the buffer.
char *dgetbuf(doub_t *const buf);

#pragma endregion

// +---- Source Stream -- End

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
tokcode_t get_keyword_or_id(const char *const lexeme);
/// @brief Scan next token in the source.
/// @param src Source buffer.
/// @param outlex Ouput lexeme.
/// @return Code of the scanned token.
tokcode_t gettok(doub_t *const src, char **const outlex);

#ifdef CALC_DEBUG

/// @brief Gets the corresponding string format
///        for a token.
/// @param code Token code.
/// @return A string containing the format string
///         to re-build the lexeme.
const char *const tokcode_to_str(const tokcode_t code);
/// @brief Gets the name of the token (e.g. KWORD_CONST).
/// @param code Token code.
/// @return A string containing the name of the token.
const char *const tokname_to_str(const tokcode_t code);

/// @brief Tokenize user inputs.
void tokenize();

#endif // CALC_DEBUG

#pragma endregion

// +---- Tokens -- End

// +---- Lexer

#pragma region Lexer

/// @brief Record of lexical analyzer infos.
typedef struct _calc_lexer
{
    /// @brief Double buffered source.
    doub_t      *doub;
    /// @brief Hash table to store identifiers.
    hashtab_t   *htab;
    /// @brief Pointer to the last inserted symbol.
    hashkey_t   *hkey;
    /// @brief Lookahead token.
    tokcode_t    look;
    /// @brief Last lexed token.
    tokcode_t    last;
} lexer_t;

/// @brief Create a new lexer status record.
/// @param bufsiz Size of the source buffer.
/// @param tabsiz Size of the hash table.
/// @return A pointer to the new lexer.
lexer_t *create_lexer(unsigned int bufsiz, unsigned int tabsiz);

/// @brief Gets the next token and puts it into
///        lex->look without increasing input
///        position.
/// @param lex Lexer reference.
/// @return The next token in the stream.
tokcode_t llook(lexer_t *const lex);
/// @brief Gets the next token and puts into
///        lex->last.
/// @param lex Lexer reference.
/// @param lexeme The corresponding lexeme
///               (only if token need it).
/// @return The next token in the stream.
tokcode_t lnext(lexer_t *const lex, char **const lexeme);
/// @brief Looks the next tokens and advance
///        only when it matches with match.
/// @param lex Lexer reference.
/// @param match Match token.
/// @param lexeme The corresponding lexeme
///               (only if token need it).
/// @return TRUE if matches, else FALSE.
bool_t lmatch(lexer_t *const lex, tokcode_t match, char **const lexeme);

#pragma endregion

// +---- Lexer -- End

#pragma endregion

/* =---- Semantic Analyzer -------------------------------------= */

#pragma region Semantic Analyzer

// +---- Abstract Syntax Tree

#pragma region Abstract Syntax Tree

typedef struct _calc_ast_symbol
{
    void *_;
} ast_symb_t;

#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma region

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_PARSE_C_
#       include "calc-parse.c"
#   endif // CALC_PARSE_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_PARSE_H_
