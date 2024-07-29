#ifndef NULL
/// @brief NULL pointer. (why it wasn't alredy defined??)
#   define NULL ((void *)0x00)
#endif // NULL - non si sa mai...

/* calc-parse.inc - Copyright (c) 2024 Frithurik Grint */

#ifndef deftok
/// @brief Placeholder declaration of deftok(...).
#   define deftok(name, lexeme)

#   ifndef defstr
/// @brief Placeholder declaration of defstr(...).
#       define defstr(name, text)
#   endif // defstr
#else
#   ifdef defstr
/// Cannot define external defstr if is alredy
/// defined an external deftok. Undefine this
/// macro avoids errors.
#       undef defstr
#   endif // UNDEF defstr

/// @brief Cannot define external defstr if is alredy
///        defined an external deftok.
#   define defstr(name, text)
#endif // deftok

/* =---- Trivia Tokens -----------------------------------------= */

#ifndef deftrv
/// @brief Internal macro for keyword tokens defintion.
#   define deftrv(name, text) deftok(name, NULL) defstr(name, text)
#endif // deftrv

/// @brief EOF trivia token. (end of file)
deftrv(ENDOF,                                       "<eof>")
/// @brief EOL trivia token. (end of line)
deftrv(ENDOL,                                       "<eol>")
/// @brief NUL trivia token. (end of string)
deftrv(NULCH,                                       "<nul>")
/// @brief WSP trivia token. (white spaces)
deftrv(SPACE,                                       "<wsp>")

/// @brief Identifier trivia token.
deftrv(IDENT,                                       "ident")
/// @brief Template identifier trivia token.
deftrv(TEMPL,                                       "templ")
/// @brief Reserved word trivia token.
deftrv(RWORD,                                       "rword")

#ifdef deftrv
#   undef deftrv
#endif // UNDEF deftrv

/* =---- Keyword Tokens ----------------------------------------= */

#ifndef CALC_PARSE_INC_
/// @brief Global keywords context. Keywords with this
///        context are valid only if they're in global scope.
#   define CTX_GLOBL 0x0010
/// @brief Generic keywords context. Keywords with this
///        context are valid everywhere in the code.
///
///        Note: Generic context is 'everywhere' but it
///              doesn't mean that keywords have an use
///              in every other context. This contexts
///              represent only lexical contexts.
#   define CTX_GNRIC 0x0020
/// @brief Switch keywords context. Keywords with this
///        context are valid only in the switch statement.
#   define CTX_SWTCH 0x0040
/// @brief Cycle keywords context. Keywords with this
///        context are valid only in loop statements.
#   define CTX_CYCLE 0x0080
/// @brief Datatype keywords context. Keywords with this
///        context are valid only in type expressions.
#   define CTX_DTYPE 0x0100
/// @brief Object keywords context. Keywords with this
///        context are valid only in object scope.
#   define CTX_OBJCT 0x0200
/// @brief Using keywords context. Keywords with this
///        context are valid only in use statements.
#   define CTX_USING 0x0400
/// @brief Template keywords contrext. Keywords with
///        this context are valid only in tempated
///        declarations.
#   define CTX_TEMPL 0x0800
#endif // CALC_PARSE_INC_

#ifndef defkey
/// @brief Internal macro for keyword tokens defintion.
#   define defkey(name, lexeme, ctx) deftok(name, lexeme) defstr(name, lexeme)
#endif // defkey

/// @brief 'abstract' keyword token.
defkey(KWORD_ABSTRACT,                              "abstract",             CTX_OBJCT | CTX_DTYPE | CTX_TEMPL)
/// @brief 'auto' keyword token.
defkey(KWORD_AUTO,                                  "auto",                 CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'bool' keyword token.
defkey(KWORD_BOOL,                                  "bool",                 CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'break' keyword token.
defkey(KWORD_BREAK,                                 "break",                CTX_SWTCH | CTX_CYCLE)
/// @brief 'call' keyword token.
defkey(KWORD_CALL,                                  "call",                 CTX_GNRIC)
/// @brief 'case' keyword token.
defkey(KWORD_CASE,                                  "case",                 CTX_SWTCH)
/// @brief 'char' keyword token.
defkey(KWORD_CHAR,                                  "char",                 CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'checked' keyword token.
defkey(KWORD_CHECKED,                               "checked",              CTX_DTYPE | CTX_TEMPL)
/// @brief 'const' keyword token.
defkey(KWORD_CONST,                                 "const",                CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'continue' keyword token.
defkey(KWORD_CONTINUE,                              "continue",             CTX_CYCLE)
/// @brief 'default' keyword token.
defkey(KWORD_DEFAULT,                               "default",              CTX_SWTCH | CTX_USING)
/// @brief 'do' keyword token.
defkey(KWORD_DO,                                    "do",                   CTX_GNRIC)
/// @brief 'else' keyword token.
defkey(KWORD_ELSE,                                  "else",                 CTX_GNRIC)
/// @brief 'end' keyword token.
defkey(KWORD_END,                                   "end",                  CTX_GNRIC | CTX_CYCLE)
/// @brief 'enum' keyword token.
defkey(KWORD_ENUM,                                  "enum",                 CTX_GNRIC | CTX_DTYPE | CTX_USING | CTX_TEMPL)
/// @brief 'extern' keyword token.
defkey(KWORD_EXTERN,                                "extern",               CTX_GNRIC | CTX_USING)
/// @brief 'final' keyword token.
defkey(KWORD_FINAL,                                 "final",                CTX_OBJCT | CTX_DTYPE)
/// @brief 'float' keyword token.
defkey(KWORD_FLOAT,                                 "float",                CTX_DTYPE | CTX_TEMPL)
/// @brief 'for' keyword token.
defkey(KWORD_FOR,                                   "for",                  CTX_GNRIC)
/// @brief 'function' keyword token.
defkey(KWORD_FUNC,                                  "function",             CTX_TEMPL)
/// @brief 'goto' keyword token.
defkey(KWORD_GOTO,                                  "goto",                 CTX_GNRIC)
/// @brief 'if' keyword token.
defkey(KWORD_IF,                                    "if",                   CTX_GNRIC)
/// @brief 'int' keyword token.
defkey(KWORD_INT,                                   "int",                  CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'let' keyword token. (constant declaration)
defkey(KWORD_LET,                                   "let",                  CTX_GNRIC)
/// @brief 'match' keyword token.
defkey(KWORD_MATCH,                                 "match",                CTX_GNRIC)
/// @brief 'module' keyword token.
defkey(KWORD_MODULE,                                "module",               CTX_GNRIC | CTX_USING | CTX_TEMPL)
/// @brief 'nameof' keyword token.
defkey(KWORD_NAMEOF,                                "nameof",               CTX_GNRIC)
/// @brief 'object' keyword token.
defkey(KWORD_OBJECT,                                "object",               CTX_GNRIC | CTX_DTYPE | CTX_USING | CTX_TEMPL)
/// @brief 'override' keyword token.
defkey(KWORD_OVERRIDE,                              "override",             CTX_OBJCT)
/// @brief 'pragma' keyword token.
defkey(KWORD_PRAGMA,                                "pragma",               CTX_GLOBL)
/// @brief 'private' keyword token.
defkey(KWORD_PRIVATE,                               "private",              CTX_OBJCT | CTX_USING)
/// @brief 'protected' keyword token.
defkey(KWORD_PROTECTED,                             "protected",            CTX_OBJCT | CTX_USING)
/// @brief 'public' keyword token.
defkey(KWORD_PUBLIC,                                "public",               CTX_OBJCT | CTX_USING)
/// @brief 'real' keyword token.
defkey(KWORD_REAL,                                  "real",                 CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'return' keyword token.
defkey(KWORD_RETURN,                                "return",               CTX_GNRIC)
/// @brief 'signed' keyword token.
defkey(KWORD_SIGNED,                                "signed",               CTX_DTYPE | CTX_TEMPL)
/// @brief 'sizeof' keyword token.
defkey(KWORD_SIZEOF,                                "sizeof",               CTX_GNRIC)
/// @brief 'static' keyword token.
defkey(KWORD_STATIC,                                "static",               CTX_GNRIC | CTX_USING)
/// @brief 'struct' keyword token.
defkey(KWORD_STRUCT,                                "struct",               CTX_GNRIC | CTX_DTYPE | CTX_USING | CTX_TEMPL)
/// @brief 'switch' keyword token.
defkey(KWORD_SWITCH,                                "switch",               CTX_GNRIC)
/// @brief 'type' keyword token.
defkey(KWORD_TYPE,                                  "type",                 CTX_TEMPL)
/// @brief 'typeof' keyword token.
defkey(KWORD_TYPEOF,                                "typeof",               CTX_GNRIC)
/// @brief 'undef' keyword token.
defkey(KWORD_UNDEF,                                 "undef",                CTX_GNRIC | CTX_DTYPE | CTX_TEMPL)
/// @brief 'union' keyword token.
defkey(KWORD_UNION,                                 "union",                CTX_GNRIC | CTX_DTYPE | CTX_USING | CTX_TEMPL)
/// @brief 'unsigned' keyword token.
defkey(KWORD_UNSIGNED,                              "unsigned",             CTX_DTYPE | CTX_TEMPL)
/// @brief 'use' keyword token.
defkey(KWORD_USE,                                   "use",                  CTX_GLOBL)
/// @brief 'void' keyword token.
defkey(KWORD_VOID,                                  "void",                 CTX_GNRIC | CTX_TEMPL)
/// @brief 'while' keyword token.
defkey(KWORD_WHILE,                                 "while",                CTX_GNRIC)
/// @brief 'with' keyword token.
defkey(KWORD_WITH,                                  "with",                 CTX_GNRIC | CTX_TEMPL)

#ifndef CALC_CONTXT_KEYWORDS
/// @brief If its value is 1, keywords have a specific context
///        where they're keywords, so in other contexts some
///        keywords can be used as identifiers. To disable
///        keywords contextualization, set this macro to zero.
#   define CALC_CONTXT_KEYWORDS 1
#endif // CALC_CONTXT_KEYWORDS

#ifndef CALC_SORTED_KEYWORDS
/// @brief If its value is 1, keywords are in alphabetical
///        order. If keywords are not sorted, set this macro
///        to zero.
#   define CALC_SORTED_KEYWORDS 1
#endif // CALC_SORTED_KEYWORDS

#ifdef defkey
#   undef defkey
#endif // UNDEF defkey

/* =---- Pragmatic Tokens --------------------------------------= */

#ifndef defprg
/// @brief Internal macro for pragmatic tokens defintion.
#   define defprg(name, lexeme) deftok(name, lexeme) defstr(name, lexeme)
#endif // defprg

/// @brief 'def' pragmatic token.
defprg(PRAGM_DEF,                                   "def")
/// @brief 'set' pragmatic token.
defprg(PRAGM_SET,                                   "set")
/// @brief 'undef' pragmatic token.
defprg(PRAGM_UNDEF,                                 "undef")

#ifndef CALC_SORTED_PRAGMATICS
/// @brief If its value is 1, pragmatics are in alphabetical
///        order. If pragmatics are not sorted, set this macro
///        to zero.
#   define CALC_SORTED_PRAGMATICS 1
#endif // CALC_SORTED_PRAGMATICS

#ifdef defprg
#   undef defprg
#endif // UNDEF defprg

/* =---- Punctuation Tokens ------------------------------------= */

#ifndef defpct
/// @brief Internal macro for keyword tokens defintion.
#   define defpct(name, text) deftok(PUNCT_ ## name, NULL) defstr(PUNCT_ ## name, text)
#endif // defpct

// Brackets

/// @brief '(' stray bracket token. (left round bracket)
defpct(LROUN,                                       "(")
/// @brief ')' stray bracket token. (rigth round bracket)
defpct(RROUN,                                       ")")
/// @brief '[' stray bracket token. (left square bracket)
defpct(LSQRD,                                       "[")
/// @brief ']' stray bracket token. (right square bracket)
defpct(RSQRD,                                       "]")
/// @brief '(' stray bracket token. (left curly bracket)
defpct(LCURL,                                       "{")
/// @brief '(' stray bracket token. (rigth curly bracket)
defpct(RCURL,                                       "}")

// Composed Brackets

/// @brief '()' composed bracket token. (round brackets)
defpct(ROUND,                                       "()")
/// @brief '[]' composed bracket token. (square brackets)
defpct(SQRED,                                       "[]")
/// @brief '{}' composed bracket token. (curly brackets)
defpct(CURLY,                                       "{}")

// Stray Punctuators

/// @brief '~' stray punctuator token. (tilde)
defpct(TILDE,                                       "~")

/// @brief '?' stray punctuator token. (question mark)
defpct(QUEST,                                       "?")
/// @brief '!' stray punctuator token. (exclamation mark)
defpct(EXCLM,                                       "!")
/// @brief '&' stray punctuator token. (ampersand)
defpct(AMPER,                                       "&")
/// @brief '|' stray punctuator token. (pipe)
defpct(PIPEE,                                       "|")
/// @brief '^' stray punctuator token. (caret)
defpct(CARET,                                       "^")

/// @brief '<' stray punctuator token. (less than)
defpct(LESST,                                       "<")
/// @brief '>' stray punctuator token. (greater than)
defpct(GREAT,                                       ">")
/// @brief '=' stray punctuator token. (equal)
defpct(EQUAL,                                       "=")

/// @brief '+' stray punctuator token. (plus)
defpct(PLUSS,                                       "+")
/// @brief '-' stray punctuator token. (minus)
defpct(MINUS,                                       "-")
/// @brief '*' stray punctuator token. (asterisk)
defpct(STARR,                                       "*")
/// @brief '/' stray punctuator token. (slash)
defpct(SLASH,                                       "/")
/// @brief '%' stray punctuator token. (percent)
defpct(PERCN,                                       "%")

/// @brief '#' stray punctuator token. (hash)
defpct(SHARP,                                       "#")
/// @brief '@' stray punctuator token. (at)
defpct(ATSIG,                                       "@")

/// @brief ',' stray punctuator token. (comma)
defpct(COMMA,                                       ",")
/// @brief '.' stray punctuator token. (dot)
defpct(POINT,                                       ".")
/// @brief ':' stray punctuator token. (colon)
defpct(COLON,                                       ":")
/// @brief ';' stray punctuator token. (semicolon)
defpct(SEMIC,                                       ";")

/// @brief '\' stray punctuator token. (backslash)
defpct(BACKS,                                       "\\")
/// @brief '`' stray punctuator token. (backtick)
defpct(BACKT,                                       "`")

// Composed Tokens - Doubled

/// @brief '??' composed punctuator token.
defpct(QUEST_QUEST,                                 "??")
/// @brief '!!' composed punctuator token.
defpct(EXCLM_EXCLM,                                 "!!")
/// @brief '&&' composed punctuator token.
defpct(AMPER_AMPER,                                 "&&")
/// @brief '||' composed punctuator token.
defpct(PIPEE_PIPEE,                                 "||")

/// @brief '<<' composed punctuator token.
defpct(LESST_LESST,                                 "<<")
/// @brief '>>' composed punctuator token.
defpct(GREAT_GREAT,                                 ">>")
/// @brief '==' composed punctuator token.
defpct(EQUAL_EQUAL,                                 "==")

/// @brief '++' composed punctuator token.
defpct(PLUSS_PLUSS,                                 "++")
/// @brief '--' composed punctuator token.
defpct(MINUS_MINUS,                                 "--")

/// @brief '..' composed punctuator token.
defpct(POINT_POINT,                                 "..")
/// @brief '::' composed punctuator token.
defpct(COLON_COLON,                                 "::")

// Composed Tokens - Equal

/// @brief '?=' composed punctuator token.
defpct(QUEST_EQUAL,                                 "?=")
/// @brief '!=' composed punctuator token.
defpct(EXCLM_EQUAL,                                 "!=")
/// @brief '&=' composed punctuator token.
defpct(AMPER_EQUAL,                                 "&=")
/// @brief '|=' composed punctuator token.
defpct(PIPEE_EQUAL,                                 "|=")
/// @brief '^=' composed punctuator token.
defpct(CARET_EQUAL,                                 "^=")

/// @brief '<=' composed punctuator token.
defpct(LESST_EQUAL,                                 "<=")
/// @brief '>=' composed punctuator token.
defpct(GREAT_EQUAL,                                 ">=")

/// @brief '<<=' composed punctuator token.
defpct(LESST_LESST_EQUAL,                           "<<=")
/// @brief '>>=' composed punctuator token.
defpct(GREAT_GREAT_EQUAL,                           ">>=")

/// @brief '+=' composed punctuator token.
defpct(PLUSS_EQUAL,                                 "+=")
/// @brief '-=' composed punctuator token.
defpct(MINUS_EQUAL,                                 "-=")
/// @brief '*=' composed punctuator token.
defpct(STARR_EQUAL,                                 "*=")
/// @brief '/=' composed punctuator token.
defpct(SLASH_EQUAL,                                 "/=")
/// @brief '%=' composed punctuator token.
defpct(PERCN_EQUAL,                                 "%=")

// Composed Tokens

/// @brief '?.' composed punctuator token.
defpct(QUEST_POINT,                                 "?.")

/// @brief '->' composed punctuator token. (arrow)
defpct(ARROW,                                       "->")
/// @brief '=>' composed punctuator token. (then arrow)
defpct(THENN,                                       "=>")
/// @brief '<>' composed punctuator token. (equivalence arrow)
defpct(EQVLN,                                       "<>")

/// @brief '...' composed punctuator token. (ellipsis)
defpct(ELLIP,                                       "...")

#ifdef defpct
#   undef defpct
#endif // UNDEF defpct

/* =---- Constant Literal Tokens -------------------------------= */

#ifndef deflit
/// @brief Internal macro for keyword tokens defintion.
#   define deflit(name, text) deftok(LITER_ ## name, NULL) defstr(LITER_ ## name, text)
#endif // deflit

// Numeric Literals

/// @brief 'bin' numeric constant literal. (binary integers)
deflit(INTGR_BIN,                                   "bin")
/// @brief 'oct' numeric constant literal. (octal integers)
deflit(INTGR_OCT,                                   "oct")
/// @brief 'dec' numeric constant literal. (decimal integers)
deflit(INTGR_DEC,                                   "dec")
/// @brief 'hex' numeric constant literal. (hexadecimal integers)
deflit(INTGR_HEX,                                   "hex")

/// @brief 'float' numeric constant literal. (floating-point numbers)
deflit(FLOAT,                                       "float")

// Textual Literals

/// @brief 'cchar' textual constant literal. (character)
deflit(CCHAR,                                       "cchar")
/// @brief 'uchar' textual constant literal. (unicode character)
deflit(UCHAR,                                       "uchar")

/// @brief 'cstr' textual constant literal. (string)
deflit(CSTRN,                                       "cstr")
/// @brief 'ustr' textual constant literal. (unicode string)
deflit(USTRN,                                       "ustr")

#ifdef deflit
#   undef deflit
#endif // UNDEF deflit

/* =------------------------------------------------------------= */

#ifdef defstr
#   undef defstr
#endif // UNDEF defstr

#ifdef deftok
#   undef deftok
#endif // UNDEF deftok

#ifndef CALC_PARSE_INC_
/// @brief This header is inlcuded at least one time.
#   define CALC_PARSE_INC_
#endif // CALC_PARSE_INC_
