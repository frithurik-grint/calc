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
///               set to NULL.
/// @return Code of the next scanned token.
tokcode_t gettoken(srcbuf_t *const sb, char **const lexeme);

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

/// @brief Tokenize from input stream. This function
///        can be used as a lexical analyzer to find
///        basic lixical errors.
void tokenize();

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

/// @brief Adds a bucket in the reserved table of the
///        lexer (ptab).
/// @param lex Lexer status on which add the bucket.
/// @param name Name of the bucket to add.
/// @return A pointer to the added bucket.
hashbuc_t *ladd(lexer_t *const lex, char *const name);
/// @brief Gets a bucket from the reserved table or
///        the identifiers table.
/// @param lex Lexer status on which search the bucket.
/// @param name Name of the bucket to search.
/// @return A pointer to the found bucket.
hashbuc_t *lget(lexer_t *const lex, char *const name);
/// @brief Sets bucket value in the reserved table or
///        the identifiers table.
/// @param lex Lexer status on which edit the bucket.
/// @param name Name of the bucket to search.
/// @param data Data value to set in the bucket.
/// @return A pointer to the found bucket.
hashbuc_t *lset(lexer_t *const lex, char *const name, unsigned int data);

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
tokcode_t lnext(lexer_t *const lex);

/// @brief Looks the next tokens and advance
///        only when it matches with match.
/// @param lex Lexer reference.
/// @param match Match token.
/// @param lexeme The corresponding lexeme
///               (only if token need it).
/// @return TRUE if matches, else FALSE.
bool_t lmatch(lexer_t *const lex, tokcode_t match);
/// @brief Looks the next tokens and advance
///        only when it matches with match.
/// @param lex Lexer reference.
/// @param others Match tokens.
/// @param lexeme The corresponding lexeme
///               (only if token need it).
/// @return TRUE if matches, else FALSE.
bool_t vlmatch(lexer_t *const lex, unsigned int count, ...);

#pragma endregion

// +---- Lexer -- End

#pragma endregion

/* =---- Syntactic Analyser ------------------------------------= */

#pragma region Syntactic Analyser

// +---- Abstract Syntax Tree

#pragma region Abstract Syntax Tree

/// @brief Type data record.
typedef struct _calc_ast_type type_t;
/// @brief Symbol data record.
typedef struct _calc_ast_symb symb_t;

// +---- Types

#pragma region Types

// Data Types

/// @brief Record for integer data types info.
typedef struct _calc_ast_type_dt_intgr type_intgr_t;
/// @brief Record for floating-poit data types info.
typedef struct _calc_ast_type_dt_float type_float_t;

/// @brief Record for sub-range (limited) data types info.
typedef struct _calc_ast_type_dt_range type_range_t;
/// @brief Record for pointer data types info.
typedef struct _calc_ast_type_dt_pnter type_pnter_t;
/// @brief Record for enumeration data types info.
typedef struct _calc_ast_type_dt_enumr type_enumr_t;
/// @brief Record for union data types info.
typedef struct _calc_ast_type_dt_union type_union_t;

// Structured Types

/// @brief Record for array types info.
typedef struct _calc_ast_type_st_array type_array_t;
/// @brief Record for multi-dimensional array types info.
typedef struct _calc_ast_type_st_multi type_multi_t;
/// @brief Record for tuple types info.
typedef struct _calc_ast_type_st_tuple type_tuple_t;
/// @brief Record for structure types info.
typedef struct _calc_ast_type_st_strct type_strct_t;

// Referenced Types

/// @brief Record for reference types info.
typedef struct _calc_ast_type_rt_refrn type_refrn_t;
/// @brief Record for function types info.
typedef struct _calc_ast_type_rt_funct type_funct_t;
/// @brief Record for functional types info.
typedef struct _calc_ast_type_rt_funcl type_funcl_t;
/// @brief Record for object types info.
typedef struct _calc_ast_type_rt_objct type_objct_t;

// +---- Data Types

#pragma region Data Types



#pragma endregion

// +---- Data Types -- End

// +---- Structured Types

#pragma region Structured Types



#pragma endregion

// +---- Structured Types -- End

// +---- Referenced Types

#pragma region Referenced Types



#pragma endregion

// +---- Referenced Types -- End

typedef enum _calc_ast_type_kind
{
    TYCOD_DT_INTGR,
    TYCOD_DT_FLOAT,

    TYCOD_DT_RANGE,
    TYCOD_DT_PNTER,
    TYCOD_DT_ENUMR,
    TYCOD_DT_UNION,
} tykind_t;

#pragma endregion

// +---- Types -- End

// +---- Symbols

#pragma region Symbols

/// @brief Record for symbol prototypes info.
typedef struct _calc_ast_symb_prototype symb_proto_t;

/// @brief Record for immutable variables info. (global and local)
typedef struct _calc_ast_symb_const symb_const_t;
/// @brief Record for mutable variables info. (global and local)
typedef struct _calc_ast_symb_mutbl symb_mutbl_t;
/// @brief Record for functions info.
typedef struct _calc_ast_symb_funct symb_funct_t;
/// @brief Record for function paramters info.
typedef struct _calc_ast_symb_param symb_param_t;
/// @brief Record for modules info.
typedef struct _calc_ast_symb_modle symb_modle_t;

#pragma endregion

// +---- Symbols -- End

/// @brief AST expression node struct.
typedef struct _calc_ast_expr ast_expr_t;
/// @brief AST declaration node struct.
typedef struct _calc_ast_decl ast_decl_t;
/// @brief AST statement node struct.
typedef struct _calc_ast_stmt ast_stmt_t;
/// @brief AST pragma node struct.
typedef struct _calc_ast_prgm ast_prgm_t;

#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_PARSE_H_
