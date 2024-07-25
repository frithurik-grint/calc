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

// +---- Sources

#pragma region Sources

// Source Buffer

typedef struct _calc_source_buffer {
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
srcbuf_t *create_srcbuf(char *const buffer, unsigned int length);
/// @brief Creates a new auto size source buffer
///        reader. (the same as create_srcbuf(NULL, 0))
/// @return A pointer to the new source buffer.
srcbuf_t *create_srcbuf_auto();
/// @brief Deletes a source buffer reader and
///        releases any memory resource used by
///        it.
/// @param sb Source buffer reader to delete.
void delete_srcbuf(srcbuf_t *const sb);

/// @brief Gets the top character from the source
///        buffer reader. (peeks the char)
/// @param sb Pointer to the source buffer reader.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
int sbtopc(srcbuf_t *const sb);
/// @brief Gets the top character from the source
///        buffer reader and increases forward
///        position counter. (reads the char)
/// @param sb Pointer to the source buffer reader.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
int sbgetc(srcbuf_t *const sb);
/// @brief Sets the top character of the source buffer
///        reader. (writes the char)
/// @param sb Pointer to the source buffer reader.
/// @param c The character to set.
/// @return The top character of the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
int sbsetc(srcbuf_t *const sb, int c);
/// @brief Sets the top character of the source buffer
///        reader and increases forward position
///        counter.
/// @param sb Pointer to the source buffer reader.
/// @param c The character to put.
/// @return The put character in the buffer or, in
///         case of failure or if reached the end of
///         the file, EOF.
int sbputc(srcbuf_t *const sb, int c);

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
char *sbtops(char *const dst, srcbuf_t *const sb, unsigned int count);
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
char *sbgets(char *const dst, srcbuf_t *const sb, unsigned int count);
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
char *sbsets(srcbuf_t *const sb, char *const src, unsigned int count);
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
char *sbputs(srcbuf_t *const sb, char *const src, unsigned int count);

/// @brief Chops buffer content from begin postion to
///        forward position.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to a new string containing the
///         chopped content.
char *sbchop(srcbuf_t *const sb);
/// @brief Chops buffer content from begin postion to
///        forward position writing chopped content into
///        destination buffer, or if it's NULL into a new
///        string.
/// @param dst Pointer to the destination buffer.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to the destination buffer.
char *sbchto(char *const dst, srcbuf_t *const sb);
/// @brief Chops buffer content from begin postion of a
///        specified number of characters writing chopped
///        content into destination buffer, or if it's
///        NULL into a new string.
/// @param dst Pointer to the destination buffer.
/// @param sb Pointer to the source buffer reader.
/// @param count Number of characters to chop.
/// @return A pointer to the destination buffer.
char *sbchof(char *const dst, srcbuf_t *const sb, unsigned int count);

/// @brief Gets the beginning of the internal source
///        buffer.
/// @param sb Pointer to the source buffer reader.
/// @return A pointer to the source buffer.
char *sbgetbuf(srcbuf_t *const sb);

/// @brief Advances position counter to reach the
///        forward position. While CALC_DEBUG is defined
///        it advances also the buffer pointer.
/// @param sb Pointer to the source buffer reader.
void sbadvnc(srcbuf_t *const sb);
/// @brief Retreats position counter to zero.
/// @param sb Pointer to the source buffer reader.
void sbretrt(srcbuf_t *const sb);
/// @brief Resets position counters to zero and
///        resets buffer to the beginning.
/// @param sb Pointer to the source buffer reader.
void sbrewnd(srcbuf_t *const sb);

// Source Stream

#pragma endregion

// +---- Sources -- End

// +---- Tokens

#pragma region Tokens

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
tokcode_t getkword(char *const lexeme);
/// @brief Gets the corresponding token code to the
///        specified lexeme, searching into the pragmatic
///        words table.
/// @param lexeme Lexeme to search.
/// @return Pragmatic code, if lexeme is a valid pragmatic
///         word, else it returns identifier code.
tokcode_t getpragm(char *const lexeme);

/// @brief Gets the next token in the source stream
///        without advancing position.
/// @param src Pointer to the source stream.
/// @param lexeme If the current token had a scanned
///               lexeme is set there, else this is
///               NULL.
/// @return Code of the next scanned token.
tokcode_t gettok(srcbuf_t *const sb, char *const *const lexeme);

#ifndef _CALC_MINIMAL_BUILD

/// @brief Gets the corresponding lexeme string of
///        a token code.
/// @param tok Code of the token.
/// @return A constant string containing the lexeme
///         or the lexeme format, or an error message.
const char *const toktostr(const tokcode_t tok);
/// @brief Gets the name of the token.
/// @param tok Code of the token.
/// @return A constant string containing the token
///         name, or an error message.
const char *const tokname(const tokcode_t tok);

/// @brief Tokenize an input stream. This function
///        can be used as a lexical analyzer to find
///        basic syntax errors and 
/// @param stream Input stream.
/// @return FALSE in case of errors, else TRUE.
bool_t tokenize(FILE *const stream);
/// @brief Tokenize an input stream to an output
///        stream.
/// @param stream Input stream.
/// @param out Output stream.
/// @return FALSE in case of errors, else TRUE.
bool_t tokenzto(FILE *const stream, FILE *const out);

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Tokens -- End

// +---- Lexer

#pragma region Lexer

/// @brief Lexer status data structure.
typedef struct _calc_lexer
{
    /// @brief Pointer to the source buffer reader.
    srcbuf_t *sbuf;
    /// @brief Pointer to the pragmatic macros hash
    ///        table. (treated as reserved symbols)
    hashtab_t *ptab;
    /// @brief Pointer to the current scope identifiers
    ///        hash table.
    hashtab_t *htab;
    /// @brief Pointer to the last inserted identifier.
    hashbuc_t *buck;
    /// @brief Pointer to the last scanned lexeme.
    char *lexm;
    /// @brief Lookahead token.
    tokcode_t look;
    /// @brief Last lexed token.
    tokcode_t last;
} lexer_t;

/// @brief Creates a new lexer status record initializing
///        it's tables and buffers.
/// @param bufsiz Maximum number of characters in the source
///               buffer. (if zero is set to default value)
/// @param tabsiz Maximum number of buckets in hash tables.
///               (if zero is set to default value)
/// @return A pointer to the new lexer status record.
lexer_t *create_lexer(unsigned int bufsiz, unsigned int tabsiz);
/// @brief Creates a new lexer status record initializing
///        it's tables using the specified source buffer.
/// @param sb Pointer to source buffer reader.
/// @param tabsiz Maximum number of buckets in hash tables.
///               (if zero is set to default value)
/// @return A pointer to the new lexer status record.
lexer_t *create_lexer_from(srcbuf_t *const sb, unsigned int tabsiz);
/// @brief Deletes the specified lexer status record
///        releasing any used memory resource.
/// @param lex Pointer to the lexer status record to
///            delete.
void delete_lexer(lexer_t *const lex);

hashbuc_t *ladd(lexer_t *const lex, char *const name);
hashbuc_t *lget(lexer_t *const lex, char *const name);
hashbuc_t *lset(lexer_t *const lex, char *const name, unsigned int data);

tokcode_t llook(lexer_t *const lex);
tokcode_t lnext(lexer_t *const lex);

#pragma endregion

// +---- Lexer -- End

#pragma endregion

CALC_C_HEADER_END

#endif // CALC_PARSE_H_
