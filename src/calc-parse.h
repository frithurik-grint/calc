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
    /// @brief Pointer to the last scanned lexeme.
    char        *lexm;
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

/* =---- Syntactic Analyzer ------------------------------------= */

#pragma region Syntactic Analyzer

// +---- Abstract Syntax Tree

#pragma region Abstract Syntax Tree

/// @brief AST expression node struct.
typedef struct _calc_ast_expr ast_expr_t;
/// @brief AST declaration node struct.
typedef struct _calc_ast_decl ast_decl_t;
/// @brief AST statement node struct.
typedef struct _calc_ast_stmt ast_stmt_t;
/// @brief AST pragma node struct.
typedef struct _calc_ast_prgm ast_prgm_t;

// +---- AST Expressions

#pragma region AST Expressions

// Unary Expressions

/// @brief AST unary expression operators.
typedef enum _calc_ast_expr_unary_op
{
    /// @brief Positive sign operator.
    OP_UNARY_POS = TOK_PUNCT_PLUSS,
    /// @brief Negative sign operator.
    OP_UNARY_NEG = TOK_PUNCT_MINUS,
} ast_expr_unop_t;

/// @brief AST unary expression node struct.
typedef struct _calc_ast_expr_unary
{
    /// @brief Operand value.
    ast_expr_t    *val;
    /// @brief Operator code.
    ast_expr_unop_t op;
} ast_expr_unary_t;

/// @brief Create a new unary expression AST node.
/// @param val Operand of the expression.
/// @param op Operator of the expression.
/// @return A new unary expression node.
ast_expr_t *create_ast_expr_unary(ast_expr_t *const val, ast_expr_unop_t op);

// Binary Expressions

/// @brief AST binary expression operators.
typedef enum _calc_ast_expr_bnary_op
{
    /// @brief Addition operator.
    OP_BNARY_ADD = TOK_PUNCT_PLUSS,
    /// @brief Subtraction operator.
    OP_BNARY_SUB = TOK_PUNCT_MINUS,
    /// @brief Multiplication operator.
    OP_BNARY_MUL = TOK_PUNCT_STARR,
    /// @brief Division operator.
    OP_BNARY_DIV = TOK_PUNCT_SLASH,
    /// @brief Modulus operator.
    OP_BNARY_MOD = TOK_PUNCT_PERCN,
    /// @brief Bitwise and operator.
    OP_BNARY_AND = TOK_PUNCT_AMPER,
    /// @brief Bitwise or operator.
    OP_BNARY_ORR = TOK_PUNCT_PIPEE,
    /// @brief Bitwise exclusive or operator.
    OP_BNARY_XOR = TOK_PUNCT_CARET,
} ast_expr_bnop_t;

/// @brief AST binary expression node struct.
typedef struct _calc_ast_expr_bnary
{
    /// @brief Left-hand side operand. (lvalue)
    ast_expr_t     *lhs;
    /// @brief Right-hand side operand. (rvalue)
    ast_expr_t     *rhs;
    /// @brief Operator code.
    ast_expr_bnop_t op;
} ast_expr_bnary_t;

/// @brief Create a new AST binary expression node.
/// @param lhs Left-hand side operand of the expression.
/// @param rhs Right-hand side operand of the expression.
/// @param op Operator of the expression.
/// @return A pointer to the new node.
ast_expr_t *create_ast_expr_bnary(ast_expr_t *const lhs, ast_expr_t *const rhs, ast_expr_bnop_t op);

// Ternary Expressions

/// @brief AST ternary expression operators.
typedef enum _calc_ast_expr_tnary_op
{
    /// @brief Conditional ternary operator.
    OP_TNARY_IFF = TOK_PUNCT_QUEST,
    /// @brief Switch ternary operator.
    OP_TNARY_SWT = TOK_PUNCT_QUEST_EQUAL,
} ast_expr_tnop_t;

/// @brief AST ternary expression node struct.
typedef struct _calc_ast_expr_tnary
{
    /// @brief Operand 1.
    ast_expr_t     *vl1;
    /// @brief Operand 2.
    ast_expr_t     *vl2;
    /// @brief Operand 3.
    ast_expr_t     *vl3;
    /// @brief Operator code.
    ast_expr_tnop_t op;
} ast_expr_tnary_t;

/// @brief Create a new AST ternary expression node.
/// @param vl1 Operand 1 of the expression.
/// @param vl2 Operand 2 of the expression. 
/// @param vl3 Operand 3 of the expression.
/// @param op Operator of the expression.
/// @return A pointer to the new node.
ast_expr_t *create_ast_expr_tnary(ast_expr_t *const vl1, ast_expr_t *const vl2, ast_expr_t *const vl3, ast_expr_tnop_t op);

// AST Expression node

/// @brief AST expression node kinds.
typedef enum _calc_ast_expr_kind
{
    /// @brief AST constant unsigned integer expression.
    AST_EXPR_UNSIG,
    /// @brief AST constant signed integer expression.
    AST_EXPR_SIGND,
    /// @brief AST constant real expression.
    AST_EXPR_REALL,
    /// @brief AST unary expression.
    AST_EXPR_UNARY,
    /// @brief AST binary expression.
    AST_EXPR_BNARY,
    /// @brief AST ternary expression.
    AST_EXPR_TNARY,
} ast_expr_kind_t;

/// @brief AST expression node data union.
typedef union _calc_ast_expr_data
{
    /// @brief AST signed integer const expression.
    long long          signd;
    /// @brief AST unsigned integer const expression.
    unsigned long long unsig;
    /// @brief AST real const expression.
    long double        reall;
    /// @brief AST unary expression.
    ast_expr_unary_t  *unary;
    /// @brief AST binary expression.
    ast_expr_bnary_t  *bnary;
    /// @brief AST ternary expression.
    ast_expr_tnary_t  *tnary;
} ast_expr_data_t;

struct _calc_ast_expr
{
    /// @brief Expression kind.
    ast_expr_kind_t kind;
    /// @brief Expression data union.
    ast_expr_data_t data;
};

ast_expr_t *create_ast_expr(ast_expr_kind_t kind);

#pragma endregion

// +---- AST Expressions -- End

#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_PARSE_C_
#       include "calc-parse.c"
#   endif // CALC_PARSE_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_PARSE_H_
