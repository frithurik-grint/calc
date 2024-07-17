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
deftrv(ENDOS,                                       "<nul>")
/// @brief WSP trivia token. (white spaces)
deftrv(SPACE,                                       "<wsp>")

/// @brief Identifier trivia token.
deftrv(IDENT,                                       "<id: '%s'>")

#ifdef deftrv
#   undef deftrv
#endif // UNDEF deftrv

/* =---- Reserved Word Tokens ----------------------------------= */

#ifndef defkey
/// @brief Internal macro for keyword tokens defintion.
#   define defkey(name, lexeme) deftok(KWORD_ ## name, lexeme) defstr(KWORD_ ## name, lexeme)
#endif // defkey

/// @brief 'const' reserved word token.
defkey(CONST,                                       "const")
/// @brief 'let' reserved word token. (constant declaration)
defkey(CDECL,                                       "let")

#ifndef CALC_SORTED_KEYWORDS
/// @brief If its value is 1, keywords are in alphabetical
///        order. If keywords are not sorted, set this macro
///        to zero.
#   define CALC_SORTED_KEYWORDS 1
#endif // CALC_SORTED_KEYWORDS

#ifdef defkey
#   undef defkey
#endif // UNDEF defkey

/* =---- Punctuation Tokens ------------------------------------= */

#ifndef defpct
/// @brief Internal macro for keyword tokens defintion.
#   define defpct(name, text) deftok(PUNCT_ ## name, NULL) defstr(PUNCT_ ## name, text)
#endif // defpct

// Brackets

/// @brief '(' stray punctuator token. (left round bracket)
defpct(LROUN,                                       "(")
/// @brief ')' stray punctuator token. (rigth round bracket)
defpct(RROUN,                                       ")")
/// @brief '[' stray punctuator token. (left square bracket)
defpct(LSQRD,                                       "[")
/// @brief ']' stray punctuator token. (right square bracket)
defpct(RSQRD,                                       "]")
/// @brief '(' stray punctuator token. (left curly bracket)
defpct(LCURL,                                       "{")
/// @brief '(' stray punctuator token. (rigth curly bracket)
defpct(RCURL,                                       "}")

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
deflit(INTGR_BIN,                                   "<bin: '%s'>")
/// @brief 'oct' numeric constant literal. (octal integers)
deflit(INTGR_OCT,                                   "<oct: '%s'>")
/// @brief 'dec' numeric constant literal. (decimal integers)
deflit(INTGR_DEC,                                   "<dec: '%s'>")
/// @brief 'hex' numeric constant literal. (hexadecimal integers)
deflit(INTGR_HEX,                                   "<hex: '%s'>")

/// @brief 'fbin' numeric constant literal. (binary floats)
deflit(FLOAT_BIN,                                   "<fbin: '%s'>")
/// @brief 'foct' numeric constant literal. (octal floats)
deflit(FLOAT_OCT,                                   "<foct: '%s'>")
/// @brief 'fdec' numeric constant literal. (decimal floats)
deflit(FLOAT_DEC,                                   "<fdec: '%s'>")
/// @brief 'fhex' numeric constant literal. (hexadecimal floats)
deflit(FLOAT_HEX,                                   "<fhex: '%s'>")

// Textual Literals

/// @brief 'cchar' textual constant literal. (character)
deflit(CCHAR,                                       "cchar")
/// @brief 'wchar' textual constant literal. (unicode character)
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