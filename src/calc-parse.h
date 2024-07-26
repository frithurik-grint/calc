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

/// @brief AST type data record.
typedef struct _calc_ast_type type_t;
/// @brief AST symbol data record.
typedef struct _calc_ast_symb symb_t;

/// @brief AST expression node struct.
typedef struct _calc_ast_expr ast_expr_t, expr_t;
/// @brief AST declaration node struct.
typedef struct _calc_ast_decl ast_decl_t, decl_t;
/// @brief AST statement node struct.
typedef struct _calc_ast_stmt ast_stmt_t, stmt_t;
/// @brief AST pragma node struct.
typedef struct _calc_ast_prgm ast_prgm_t, prgm_t;

// Common infos

/// @brief Record for symbol limits info.
typedef struct _calc_ast_limit
{
    /// @brief Minimum value.
    expr_t *min;
    /// @brief Maximum value.
    expr_t *max;
} limit_t;

typedef struct _calc_ast_limit_array
{
    /// @brief Limits array.
    limit_t *limv;
    /// @brief Number of limits in the array.
    unsigned int limc;
} limits_t;

/// @brief Gets a limits array.
/// @param count Number of limits in the array.
/// @param limits Limits array.
/// @return A record containing the limits info.
limits_t get_limits(unsigned int count, limit_t *const limits);
/// @brief Gets a limits array.
/// @param count Number of limits in the array.
/// @param others Limits array. (min1, max1, ..., minN, maxN)
/// @return A record containing the limits info.
limits_t vget_limits(unsigned int count, ...);

/// @brief Record for symbol prototypes info.
typedef struct _calc_ast_proto
{
    /// @brief Name of the prototyped symbol. (can be NULL)
    char *name;
    /// @brief Type of the prototyped symbol.
    type_t *type;
} proto_t, param_t;

typedef struct _calc_ast_proto_array
{
    /// @brief Prototypes array.
    proto_t *array;
    /// @brief Number of prototypes in the array.
    unsigned int count;
} protos_t, params_t;

/// @brief Gets a params/prototypes array.
/// @param count Number of params/prototypes in the array.
/// @param params Params array.
/// @return A record containing the params info.
params_t get_params(unsigned int count, param_t *const params);
/// @brief Gets a params array.
/// @param count Number of params in the array.
/// @param others Params array. (name1, type1, ..., nameN, typeN)
/// @return A record containing the params info.
params_t vget_params(unsigned int count, ...);

/// @brief Record for symbol fields info.
typedef struct _calc_ast_field
{
    /// @brief Name of the field.
    char *name;
    /// @brief Type of the field.
    type_t *type;
    /// @brief Default value of the field.
    expr_t *dval;
} field_t;

typedef struct _calc_ast_filed_array
{
    /// @brief Fields array.
    field_t *fldv;
    /// @brief Number of fields.
    unsigned int fldc;
} fields_t;

/// @brief Gets a fields array.
/// @param count Number of fields in the array.
/// @param fields Fileds array.
/// @return A record containing the fields info.
fields_t get_fields(unsigned int count, field_t *const fields);
/// @brief Gets a fields array.
/// @param count Number of fields in the array.
/// @param others Fileds array. (name1, type1, dval1, ..., nameN, typeN, dvalN)
/// @return A record containing the fields info.
fields_t vget_fields(unsigned int count, ...);

// +---- Types

#pragma region Types

// +---- Data Types

#pragma region Data Types

/*
 *  type_intgr
 *    : ('signed'|'unsigned')? 'int' '::' liter_intgr_dec
 *    | ('signed'|'unsigned')? 'int'
 *    ;
*/

/// @brief Record for integer data types info.
typedef struct _calc_ast_type_dt_intgr
{
    /// @brief Width (in bits) of the integer.
    unsigned int width;
    /// @brief Sign extension flag.
    bool_t sign;
    /// @brief Overflow checks flag.
    bool_t ovfl;
} type_intgr_t;

/// @brief Create a new integer data type record.
/// @param width Width (in bits) of the integer.
/// @param sign Sign extension flag.
/// @param ovfl Overflow checks flag.
/// @return A pointer to the new integer type.
type_t *create_type_intgr(unsigned int width, bool_t sign, bool_t ovfl);
/// @brief Deletes an integer data type.
/// @param ty Type to delete.
void delete_type_intgr(type_t *const ty);

/*
 *  type_float_prec
 *    : 'half'
 *    | 'single'
 *    | 'double'
 *    | 'extended'
 *    | 'quadruple'
 *    | 'octuple'
 *    ;
 *
 *  type_float
 *    : 'real' '::' type_float_prec
 *    | 'real'
 *    ;
*/

/// @brief Record for floating-poit data types info.
typedef struct _calc_ast_type_dt_float
{
    void *plcaeholder;
} type_float_t;

/*
 *  type_range_limit
 *    : ('['|'(') expr_const '..' expr_const (']'|')')
 *    | ('['|'(') expr_const '...' (']'|')')
 *    | ('['|'(') '...' expr_const (']'|')')
 *
 *  type_range
 *    : type_range_limit ':' type
 *    | type_range_limit
 *    ;
*/

/// @brief Record for sub-range (limited) data types info.
typedef struct _calc_ast_type_dt_range // 
{
    /// @brief Base data type.
    type_t *base;
    /// @brief Limits of the range.
    limit_t lim;
} type_range_t;

/// @brief Creates a new sub-range type.
/// @param ty Base data type.
/// @param min Inferior limit of the set.
/// @param max Superior limit of the set.
/// @return A pointer to the new sub-range type.
type_t *create_type_range(type_t *const ty, expr_t *const min, expr_t *const max);
/// @brief Creates a new sub-range type.
/// @param ty Base data type.
/// @param limits Limits of the set.
/// @return A pointer to the new sub-range type.
type_t *create_type_range_limited(type_t *const ty, limit_t *const limits);
/// @brief Deletes a sub-range type.
/// @param ty Sub-range type to delete.
void delete_type_range(type_t *const ty);

/*
 *  type_pnter
 *    : type '*'
 *    | '*' type
 *    ;
*/

/// @brief Record for pointer data types info.
typedef struct _calc_ast_type_dt_pnter
{
    /// @brief Base data type.
    type_t *base;
}type_pnter_t;

/*
 *  type_enumr_field
 *    : ident '=' expr_const
 *    | ident
 *    ;
 * 
 *  type_enumr_fields
 *    : type_enumr_field ',' type_enumr_fields
 *    | type_enumr_field
 *    ;
 *
 *  type_enumr_block
 *    : '{' type_enumr_fields '}'
 *    | '{' '}'
 *    ;
 *
 *  type_enumr
 *    : 'enum' ident? ':' type type_enumr_block
 *    | 'enum' type_enumr_block
 *    ;
*/

/// @brief Record for enumeration data types info.
typedef struct _calc_ast_type_dt_enumr
{
    /// @brief Base data type.
    type_t *base;
    /// @brief Body of the enumeration.
    fields_t body;
} type_enumr_t;

/*
 *  type_union_fields
 *    : decl ',' type_union_fields
 *    | decl
 *    ;
 *
 *  type_union_block
 *    : '{' type_union_fields '}'
 *    | '{' '}'
 *    ;
 *
 *  type_union
 *    : 'union' ident? type_union_block
 *    | 'union' type_union_block
 *    ;
*/

/// @brief Record for union data types info.
typedef struct _calc_ast_type_dt_union
{
    /// @brief Body of the union.
    fields_t body;
} type_union_t;

#pragma endregion

// +---- Data Types -- End

// +---- Structured Types

#pragma region Structured Types

/*
 *  type_array_bound
 *    : expr_const '..' expr_const
 *    | expr_const
 *    ;
 *
 *  type_array
 *    : type '[' type_array_bound ']'
 *    | type '[' ']'
 *    ;
*/

/// @brief Record for array types info.
typedef struct _calc_ast_type_st_array
{
    /// @brief Base data type.
    type_t *base;
    /// @brief Limits of the array index.
    limit_t lim;
} type_array_t;

/*
 *  type_multi_bound
 *    : type_array_bound ',' type_multi_bound
 *    | type_array_bound
 *    ;
 *
 *  type_multi_bounds
 *    : type_array_bound ',' type_multi_bound
 *    | ','+
 *    ;
 * 
 *  type_multi
 *    : type '[' type_multi_bounds ']'
 *    ;
*/

/// @brief Record for multi-dimensional array types info.
typedef struct _calc_ast_type_st_multi
{
    /// @brief Base data type.
    type_t *base;
    /// @brief Array of dimensions limits.
    limit_t *limv;
    /// @brief Number of array dimensions. (rank)
    unsigned int limc;
} type_multi_t;

/*
 *  type_tuple_items
 *    : decl ',' type_tuple_items
 *    | type ',' type_tuple_items
 *    | decl
 *    | type
 *    ;
 * 
 *  type_tuple
 *    : '(' type_tuple_items ')'
 *    | '(' ')'
 *    ;
*/

/// @brief Record for tuple types info.
typedef struct _calc_ast_type_st_tuple
{
    /// @brief Member prototypes.
    proto_t *mbrv;
    /// @brief Number of members.
    unsigned int mbrc;
} type_tuple_t;

/*
 *  type_strct_fields
 *    : decl ',' type_strct_fields
 *    | decl
 *    ;
 *
 *  type_strct_block
 *    : '{' type_strct_fields '}'
 *    | '{' '}'
 *    ;
 *
 *  type_strct
 *    : 'struct' ident? ':' type type_strct_block
 *    | 'struct' type_strct_block
 *    ;
*/

/// @brief Record for structure types info.
typedef struct _calc_ast_type_st_strct type_strct_t;

#pragma endregion

// +---- Structured Types -- End

// +---- Referenced Types

#pragma region Referenced Types

/*
 *  type_refrn
 *    : '&' type
 *    | type '&'
 *    ;
*/

/// @brief Record for reference types info.
typedef struct _calc_ast_type_rt_refrn
{
    /// @brief Base type.
    type_t *base;
} type_refrn_t;

/*
 *  type_funct_params
 *    : decl ',' type_funct_params
 *    | type ',' type_funct_params
 *    | '...' decl
 *    | '...' type
 *    | '...'
 *    | decl
 *    | type
 *    ;
 * 
 *  type_funct
 *    : '(' type_funct_params ')' '->' type
 *    | '(' ')' '->' type
 *    | type '->' type
 *    ;
*/

/// @brief Record for function types info.
typedef struct _calc_ast_type_rt_funct
{
    /// @brief Return type.
    type_t *ret;
    /// @brief Param prototypes.
    param_t *params;
    /// @brief Number of paramters.
    unsigned int paramc;
    /// @brief Variadic function flag. (last is variadic)
    bool_t vararg;
} type_funct_t;

/// @brief Record for Symbolic types info.
typedef struct _calc_ast_type_rt_symbl type_symbl_t;

/// @brief Record for object types info.
typedef struct _calc_ast_type_rt_objct type_objct_t;

#pragma endregion

// +---- Referenced Types -- End

/// @brief Type codes enumeration.
typedef enum _calc_ast_type_code
{
    /// @brief Integer data type code.
    TYCOD_DT_INTGR,
    /// @brief Floating-pont data type code.
    TYCOD_DT_FLOAT,

    /// @brief Sub-range data type code.
    TYCOD_DT_RANGE,
    /// @brief Pointer data type code.
    TYCOD_DT_PNTER,
    /// @brief Enumeration data type code.
    TYCOD_DT_ENUMR,
    /// @brief Union data type code.
    TYCOD_DT_UNION,

    /// @brief Array structured type code.
    TYCOD_ST_ARRAY,
    /// @brief Multi-dimensional array type code.
    TYCOD_ST_MULTI,
    /// @brief Tuple structured type code.
    TYCOD_ST_TUPLE,
    /// @brief Structure structured type code.
    TYCOD_RT_STRCT,

    /// @brief Reference referenced type code.
    TYCOD_RT_REFRN,
    /// @brief Function referenced type code.
    TYCOD_RT_FUNCT,
    /// @brief Functional referenced type code.
    TYCOD_RT_FUNCC,
    /// @brief Object referenced type code.
    TYCOD_RT_OBJCT,
} tycode_t;

#ifndef isdattype
/// @brief Checks if a type code is a data type.
#   define isdattype(value) ((bool_t)((value >= TYCOD_DT_INTGR) && (value <= TYCOD_DT_UNION)))
#endif // isdattype

#ifndef isstrtype
/// @brief Checks if a type code is a structured type.
#   define isstrtype(value) ((bool_t)((value >= TYCOD_ST_ARRAY) && (value <= TYCODE_ST_STRCT)))
#endif // isstrtype

#ifndef isreftype
/// @brief Checks if a type code is a referenced type.
#   define isreftype(value) ((bool_t)((value >= TYCOD_RT_REFRN) && (value <= TYCOD_RT_OBJCT)))
#endif // isreftype

/// @brief Type data pointers union.
typedef union _calc_ast_type_data
{
    /// @brief Interger type record pointer.
    type_intgr_t *intgr;
    /// @brief Floating-point type record pointer.
    type_float_t *fltty;
    /// @brief Sub-range type recrod pointer.
    type_range_t *range;
    /// @brief Pointer type record pointer.
    type_pnter_t *pnter;
    /// @brief Enumeration type record pointer.
    type_enumr_t *enumr;
    /// @brief Union type record pointer.
    type_union_t *unnty;
    /// @brief Array type record pointer.
    type_array_t *array;
    /// @brief Multi-dimensional array type record pointer.
    type_multi_t *multi;
    /// @brief Tuple type record pointer.
    type_tuple_t *tuple;
    /// @brief Structure type record pointer.
    type_strct_t *strct;
    /// @brief Reference type record pointer.
    type_refrn_t *refrn;
    /// @brief Function type record pointer.
    type_funct_t *funct;
    /// @brief Symbolic type record pointer.
    type_symbl_t *symbl;
    /// @brief Object type record pointer.
    type_objct_t *objct;
} tydata_t;

/// @brief Allocates the specific record for the
///        type kind specified by the code.
/// @param code Code of the type.
/// @return A the type record pointers union.
tydata_t alloc_tydata(tycode_t code);

struct _calc_ast_type
{
    /// @brief Name of the type.
    char *name;
    /// @brief Constant type flag.
    bool_t cnst;
    /// @brief Type code.
    tycode_t code;
    /// @brief Type data pointer.
    tydata_t data;
};

/// @brief Creates a new type record.
/// @param name Name of the type.
/// @param code Code of the type.
/// @return A pointer to the new allocated type
///         record.
type_t *create_type(char *name, tycode_t code);

#pragma endregion

// +---- Types -- End

// +---- Symbols

#pragma region Symbols

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



#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_PARSE_H_
