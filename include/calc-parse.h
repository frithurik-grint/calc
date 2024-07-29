#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_

#include "calc-liber.h"

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

/* =---- Lexical Analyser --------------------------------------= */

#pragma region Lexical Analyser

// +---- Source ------------------------------------------------+

#pragma region Source

// Source Buffer

typedef struct _calc_source_buffer
{
    /// @brief Source buffer pointer.
    char *buf;
    /// @brief Current position counter.
    unsigned int pos;
    /// @brief Forward position counter.
    unsigned int fwd;
    /// @brief Maximum number of characters
    ///        in the source buffer.
    unsigned int len;
} srcbuf_t;

/// @brief Create a new source buffer reader
///        from an input buffer and from its
///        maximum length.
/// @param buffer Input buffer of characters
///               from which read. If it's NULL
///               will be allocated a new
///               buffer of as characters as
///               length, or if that's equal
///               to zero, as memory page
///               size (or BUFSIZ, if is defined
///               macro _CALC_USE_BUFSIZ).
/// @param length Maximum number of character that
///               source buffer can contain. If
///               it's zero it will be set to
///               memory page size (or BUFSIZ,
//                if is defined macro _CALC_USE_BUFSIZ).
/// @return A pointer to the new source buffer.
_API srcbuf_t *_CDECL create_srcbuf(char *const buffer, unsigned int length);
/// @brief Creates a new auto size source buffer
///        reader. (the same as create_srcbuf(NULL, 0))
/// @return A pointer to the new source buffer.
_API srcbuf_t *_CDECL create_srcbuf_auto();
/// @brief Deletes a source buffer reader and
///        releases any memory resource used by
///        it.
/// @param sb Source buffer reader to delete.
_API void _CDECL delete_srcbuf(srcbuf_t *const sb);

/// @brief Gets the top character from the source
///        buffer reader. (peeks the char)
/// @param sb Pointer to the source buffer reader.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
_API int _CDECL sbtopc(srcbuf_t *const sb);
/// @brief Gets the top character from the source
///        buffer reader and increases forward
///        position counter. (reads the char)
/// @param sb Pointer to the source buffer reader.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
_API int _CDECL sbgetc(srcbuf_t *const sb);
/// @brief Sets the top character of the source buffer
///        reader. (writes the char)
/// @param sb Pointer to the source buffer reader.
/// @param c The character to set.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
_API int _CDECL sbsetc(srcbuf_t *const sb, int c);
/// @brief Sets the top character of the source buffer
///        reader and increases forward position
///        counter.
/// @param sb Pointer to the source buffer reader.
/// @param c The character to put.
/// @return The put character in the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
_API int _CDECL sbputc(srcbuf_t *const sb, int c);

/// @brief Reads a string from the source buffer in the
///        destination buffer. It reads a line or until it
///        reaches the max number of characters to read.
///        If destination buffer is NULL will be allocated
///        a new buffer of as characters as count, but if
///        count is zero this function does nothing.
/// @param dst Destination bffer 
/// @param sb Pointer to the source buffer reader.
/// @param count Maximum number of characters to read.
/// @return A pointer to the destination buffer.
_API char *_CDECL sbtops(char *const dst, srcbuf_t *const sb, unsigned int count);
/// @brief Reads a string from the source buffer in the
///        destination buffer and increases forward postion
///        counter. It reads a line or it reads until it
///        reaches the max number of characters to read.
///        If destination buffer is NULL will be allocated
///        a new buffer of as characters as count, but if
///        count is zero this function does nothing.
/// @param dst Destination bffer 
/// @param sb Pointer to the source buffer reader.
/// @param count Maximum number of characters to read.
/// @return A pointer to the destination buffer.
_API char *_CDECL sbgets(char *const dst, srcbuf_t *const sb, unsigned int count);
/// @brief Writes a string from the source buffer in the
///        destination buffer. It writes a line or until it
///        reaches the max number of characters to write.
///        If destination buffer is NULL or if count is zero,
///        this function does nothing.
/// @param sb Pointer to the source buffer reader.
/// @param src Source string to write.
/// @param count Maximum number of characters to write.
/// @return A pointer to the source string or NULL in case
///         of error.
_API char *_CDECL sbsets(srcbuf_t *const sb, char *const src, unsigned int count);
/// @brief Writes a string from the source buffer in the
///        destination buffer and increases forward position
///        counter. It writes a line or until it
///        reaches the max number of characters to write.
///        If destination buffer is NULL or if count is zero,
///        this function does nothing.
/// @param sb Pointer to the source buffer reader.
/// @param src Source string to write.
/// @param count Maximum number of characters to write.
/// @return A pointer to the source string or NULL in case
///         of error.
_API char *_CDECL sbputs(srcbuf_t *const sb, char *const src, unsigned int count);

/// @brief Chops buffer content from begin postion to
///        forward position.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to a new string containing the
///         chopped content.
_API char *_CDECL sbchop(srcbuf_t *const sb);
/// @brief Chops buffer content from begin postion to
///        forward position writing chopped content into
///        destination buffer, or if it's NULL into a new
///        string.
/// @param dst Pointer to the destination buffer.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to the destination buffer.
_API char *_CDECL sbchto(char *const dst, srcbuf_t *const sb);
/// @brief Chops buffer content from begin postion of a
///        specified number of characters writing chopped
///        content into destination buffer, or if it's
///        NULL into a new string.
/// @param dst Pointer to the destination buffer.
/// @param sb Pointer to the source buffer reader.
/// @param count Number of characters to chop.
/// @return A pointer to the destination buffer.
_API char *_CDECL sbchof(char *const dst, srcbuf_t *const sb, unsigned int count);

/// @brief Gets the beginning of the internal source
///        buffer.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to the source buffer.
_API char *_CDECL sbgetbuf(srcbuf_t *const sb);

/// @brief Advances position counter to reach the
///        forward position. While CALC_DEBUG is defined
///        it advances also the buffer pointer.
/// @param sb Pointer to the source buffer reader.
_API void _CDECL sbadvnc(srcbuf_t *const sb);
/// @brief Retreats position counter to zero.
/// @param sb Pointer to the source buffer reader.
_API void _CDECL sbretrt(srcbuf_t *const sb);
/// @brief Resets position counters to zero and
///        resets buffer to the beginning.
/// @param sb Pointer to the source buffer reader.
_API void _CDECL sbrewnd(srcbuf_t *const sb);

#pragma endregion

// +---- Source ------------------------ End -------------------+

// +---- Tokenizer ---------------------------------------------+

#pragma region Tokenizer

typedef enum _calc_token_code
{
    /// @brief Not recognized or erroneus
    ///        token code.
    TOK_INVAL,

#pragma push_macro("deftok")

#ifndef deftok
    /// @brief Define a token and its lexeme.
#   define deftok(tok_name, tok_lexeme) TOK_ ## tok_name,
#endif // deftok

#include "calc-parse.inc"

#ifdef deftok
#   undef deftok
#endif // UNDEF deftok

#pragma pop_macro("deftok")
} tokcode_t;

/// @brief Gets the corresponding token code to the
///        specified lexeme, searching into the keywords
///        table.
/// @param lexeme Lexeme to search.
/// @return Keyword code, if lexeme is a valid keyword,
///         else it returns identifier code.
_API tokcode_t _CDECL getkword(char *const lexeme, unsigned int context);
/// @brief Gets the corresponding token code to the
///        specified lexeme, searching into the pragmatic
///        words table.
/// @param lexeme Lexeme to search.
/// @return Pragmatic code, if lexeme is a valid pragmatic
///         word, else it returns identifier code.
_API tokcode_t _CDECL getpragm(char *const lexeme);

/// @brief Gets the next token in the source stream
///        without advancing position.
/// @param src Pointer to the source stream.
/// @param lexeme If the current token had a scanned
///               lexeme is set there, else this is
///               set to NULL.
/// @return Code of the next scanned token.
_API tokcode_t _CDECL gettoken(srcbuf_t *const sb, char **const lexeme);

#ifndef _CALC_MINIMAL_BUILD

/// @brief Gets the corresponding lexeme string of
///        a token code.
/// @param tok Code of the token.
/// @return A constant string containing the lexeme
///         or the lexeme format, or an error message.
_API const char *const _CDECL toktostr(const tokcode_t tok);
/// @brief Gets the name of the token.
/// @param tok Code of the token.
/// @return A constant string containing the token
///         name, or an error message.
_API const char *const _CDECL tokname(const tokcode_t tok);

/// @brief Tokenize from input stream. This function
///        can be used as a lexical analyzer to find
///        basic lixical errors.
_API void _CDECL tokenize();

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Tokenizer --------------------- End -------------------+

// +---- Lexer -------------------------------------------------+

#pragma region Lexer

#pragma endregion

// +---- Lexer ------------------------- End -------------------+

#pragma endregion

/* =---- Syntactic Analyser ------------------------------------= */

#pragma region Syntactic Analyser

// +---- Abstract Syntax Tree ----------------------------------+

#pragma region Abstract Syntax Tree

// Terminals

// +---- Types -------------------------------------------------+

#pragma region Types

// +---- Value Types -------------------------------------------+

#pragma region Value Types

#pragma endregion

// +---- Value Types ------------------- End -------------------+

// +---- Structured Types --------------------------------------+

#pragma region Structured Types

#pragma endregion

// +---- Structured Types -------------- End -------------------+

// +---- Referenced Types --------------------------------------+

#pragma region Referenced Types

#pragma endregion

// +---- Referenced Types -------------- End -------------------+

#pragma endregion

// +---- Types ------------------------- End -------------------+

// +---- Symbols -----------------------------------------------+

#pragma region Symbols

#pragma endregion

// +---- Symbols ----------------------- End -------------------+

// Non-Terminals

// +---- Expressions -------------------------------------------+

#pragma region Expressions

// +---- Type Expressions --------------------------------------+

// +---- Type Expressions -------------- End

#pragma endregion

// +---- Expressions ------------------- End -------------------+

// +---- Declarations ------------------------------------------+

#pragma region Declarations

#pragma endregion

// +---- Declarations ------------------ End -------------------+

// +---- Statements --------------------------------------------+

#pragma region Statements

#pragma endregion

// +---- Statements -------------------- End -------------------+

// +---- Pragmatics --------------------------------------------+

#pragma region Pragmatics

#pragma endregion

// +---- Pragmatics -------------------- End -------------------+

#pragma endregion

// +---- Abstraxt Syntax Tree ---------- End -------------------+

// +---- Parser ------------------------------------------------+

#pragma region Parser

#pragma endregion

// +---- Parser ------------------------ End -------------------+

#pragma endregion

/* =---- Semantic Analyser -------------------------------------= */

#pragma region Semantic Analyser

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_PARSE_H_
