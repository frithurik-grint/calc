#include "calc-parse.h"

#pragma warning(push)
#pragma warning(disable: 6385)
#pragma warning(disable: 6386)

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

doub_t *create_doub(char *const buffer, unsigned int length)
{
    doub_t *buf = alloc(doub_t);

    if (!length)
        length = BUFSIZ;

    buf->buf = (!buffer ? stralloc(length) : buffer);
    buf->pos = 0;
    buf->fwd = 0;
    buf->len = length;

    return buf;
}

int dtopc(const doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd];
#else
        return buf->buf[buf->pos + buf->fwd];
#endif
    else
        return EOF;
}

int dgetc(doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd++];
#else
        return buf->buf[buf->pos + buf->fwd++];
#endif
    else
        return EOF;
}

int dsetc(doub_t *const buf, int c)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd] = c;
#else
        return buf->buf[buf->pos + buf->fwd] = c;
#endif
    else
        return EOF;
}

int dputc(doub_t *const buf, int c)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd++] = c;
#else
        return buf->buf[buf->pos + buf->fwd++] = c;
#endif
    else
        return EOF;
}

char *dgets(char *const dest, doub_t *const buf)
{
    int i = 0, c;

    do
    {
        c = dgetc(buf);

        if (c == EOF)
            break;
        else
            dest[i++] = c;
    } while ((c != '\n') && (c != '\0'));

    dest[i] = '\0';

    return dest;
}

char *dputs(doub_t *const buf, char *const str)
{
    int i = 0, c;

    do
        c = dputc(buf, str[i++]);
    while ((c != EOF) && str[i]);

    dputc(buf, '\0');

    return buf->fwd = 0, str;
}

char *dchop(doub_t *const buf)
{
#ifdef CALC_DEBUG
    return strndcpy(NULL, buf->buf, buf->fwd);
#else
    return strndcpy(NULL, buf->buf + buf->pos, buf->fwd);
#endif
}

char *dchopto(doub_t *const buf, char *const dest)
{
#ifdef CALC_DEBUG
    return strndcpy(dest, buf->buf, buf->fwd);
#else
    return strndcpy(dest, buf->buf + buf->pos, buf->fwd);
#endif
}

char *dchopof(doub_t *const buf, size_t of)
{
#ifdef CALC_DEBUG
    return strndcpy(NULL, buf->buf, of);
#else
    return strndcpy(NULL, buf->buf + buf->pos, of);
#endif
}

void dadvance(doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
    {
#ifdef CALC_DEBUG
        buf->buf += buf->fwd;
#endif
        buf->pos += buf->fwd;
        buf->fwd = 0;
    }
    else
    {
#ifdef CALC_DEBUG
        buf->buf += (buf->pos + buf->fwd) - buf->len;
#endif
        buf->pos += (buf->pos + buf->fwd) - buf->len;
        buf->fwd = 0;
    }

    return;
}

void dretreat(doub_t *const buf)
{
    buf->fwd = 0;
    return;
}

void drewind(doub_t *const buf)
{
#ifdef CALC_DEBUG
    buf->buf -= buf->pos;
#endif
    buf->pos = 0;
    buf->fwd = 0;

    return;
}

char *dgetbuf(doub_t *const buf)
{
#ifdef CALC_DEBUG
    return buf->buf;
#else
    return buf->buf + buf->pos;
#endif
}

#pragma endregion

// +---- Source Stream -- End

// +---- Tokens

#pragma region Tokens

// token structure
struct tok_t
{
    const char *lexm;
    tokcode_t code;
};

tokcode_t get_keyword_or_id(const char *const lexeme)
{
    static const struct tok_t keystab[] = {
#pragma push_macro("defkey")

#ifndef defkey
#   define defkey(tok_name, tok_lexeme) { .code = TOK_ ## tok_name, .lexm = tok_lexeme },
#endif

#include "calc-parse.inc"

#ifdef defkey
#   undef defkey
#endif

#pragma pop_macro("defkey")

        NULL
    };

    static const int count = (sizeof(keystab) / sizeof(*keystab));

#if CALC_SORTED_KEYWORDS // if keywords are soreted, it does a binary search
    int hig = 0, mid, low = count - 1, k = 0;

    do
    {
        mid = (hig + low) >> 1;

        switch (numcmp(*(lexeme + k), *(keystab[mid].lexm + k)))
        {
        case +1:
            hig = mid + 1, k = 0;
            break;

        case -1:
            low = mid - 1, k = 0;
            break;

        default:
            if (streq(lexeme, keystab[mid].lexm))
                return keystab[mid].code;
            else
                ++k;
            
            break;
        }
    } while (hig <= low);

    return TOK_IDENT;
#else // if keywords aren't sorted, it does a linear search
    int i;

    for (i = 0; i < count; i++)
    {
        if (streq(lexeme, keystab[i].lexm))
            return keystab[i].code;
        else
            continue;
    }

    return TOK_IDENT;
#endif
}

// +---- Internal

#pragma region Internal (Lexical Errors)

// TODO: better exception management...

static inline int expected(const char *const what, char *const got)
{
    return errorfn("syntax error: expected %s (got '%s')", what, got);
}

static inline int unexpected(const char *const what, char *const expected)
{
    return errorfn("syntax error: unexpected %s (expected '%s')", what, expected);
}

static inline int notvalid(const char *const what)
{
    return errorfn("syntax error: '%s' is not a valid token", what);
}

#pragma endregion

// +---- Internal -- End

// +---- Scanner

#pragma region Internal (Lexical Scanner)

static tokcode_t _gettok(doub_t *const src, char **const lexeme)
{
    int l, n = 0, o = 0; // lookahead character

    while (isspace(l = dtopc(src)))
        src->buf++, src->pos++;

    static char t[BUFSIZ] = { 0 };

    switch (l)
    {
        // Trivia

    case EOF:
        return TOK_ENDOF;

    case NUL:
        return TOK_NULCH;

        // Brackets

    case '(':
        src->fwd++;

        while (isspace(l = dtopc(src)))
            src->fwd++;

        if (l == ')')
            return src->fwd++, TOK_PUNCT_ROUND;
        else
            return TOK_PUNCT_LROUN;

    case ')':
        return src->fwd++, TOK_PUNCT_RROUN;

    case '[':
        src->fwd++;

        while (isspace(l = dtopc(src)))
            src->fwd++;

        if (l == ']')
            return src->fwd++, TOK_PUNCT_SQRED;
        else
            return TOK_PUNCT_LSQRD;

    case ']':
        return src->fwd++, TOK_PUNCT_RSQRD;

    case '{':
        src->fwd++;

        while (isspace(l = dtopc(src)))
            src->fwd++;

        if (l == '}')
            return src->fwd++, TOK_PUNCT_CURLY;
        else
            return TOK_PUNCT_LCURL;

    case '}':
        return src->fwd++, TOK_PUNCT_RCURL;

        // Punctuators

    case '~':
        return src->fwd++, TOK_PUNCT_TILDE;

    case '?':
        src->fwd++;

        if ((l = dtopc(src)) == '?')
            return src->fwd++, TOK_PUNCT_QUEST_QUEST;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_QUEST_EQUAL;
        else
            return TOK_PUNCT_QUEST;

    case '!':
        src->fwd++;

        if ((l = dtopc(src)) == '!')
            return src->fwd++, TOK_PUNCT_EXCLM_EXCLM;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_EXCLM_EQUAL;
        else
            return TOK_PUNCT_EXCLM;

    case '&':
        src->fwd++;

        if ((l = dtopc(src)) == '&')
            return src->fwd++, TOK_PUNCT_AMPER_AMPER;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_AMPER_EQUAL;
        else
            return TOK_PUNCT_AMPER;

    case '|':
        src->fwd++;

        if ((l = dtopc(src)) == '|')
            return src->fwd++, TOK_PUNCT_PIPEE_PIPEE;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_PIPEE_EQUAL;
        else
            return TOK_PUNCT_PIPEE;

    case '^':
        src->fwd++;

        if (dtopc(src) == '=')
            return src->fwd++, TOK_PUNCT_CARET_EQUAL;
        else
            return TOK_PUNCT_CARET;

    case '<':
        src->fwd++;

        if ((l = dtopc(src)) == '<')
        {
            src->fwd++;

            if (dtopc(src) == '=')
                return src->fwd++, TOK_PUNCT_LESST_LESST_EQUAL;
            else
                return TOK_PUNCT_LESST_LESST;
        }
        else if (l == '=')
        {
            return src->fwd++, TOK_PUNCT_LESST_EQUAL;
        }
        else if (l == '>')
        {
            return src->fwd++, TOK_PUNCT_EQVLN;
        }
        else
        {
            return TOK_PUNCT_LESST;
        }

    case '>':
        src->fwd++;

        if (dtopc(src) == '>')
        {
            src->fwd++;

            if (dtopc(src) == '=')
                return src->fwd++, TOK_PUNCT_GREAT_GREAT_EQUAL;
            else
                return TOK_PUNCT_GREAT_GREAT;
        }
        else if (l == '=')
        {
            return src->fwd++, TOK_PUNCT_GREAT_EQUAL;
        }
        else
        {
            return TOK_PUNCT_GREAT;
        }

    case '=':
        src->fwd++;

        if ((l = dtopc(src)) == '=')
            return src->fwd++, TOK_PUNCT_EQUAL_EQUAL;
        else if (l == '>')
            return src->fwd++, TOK_PUNCT_THENN;
        else
            return TOK_PUNCT_EQUAL;

    case '+':
        src->fwd++;

        if ((l = dtopc(src)) == '+')
            return src->fwd++, TOK_PUNCT_PLUSS_PLUSS;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_PLUSS_EQUAL;
        else
            return TOK_PUNCT_PLUSS;

    case '-':
        src->fwd++;

        if ((l = dtopc(src)) == '-')
            return src->fwd++, TOK_PUNCT_MINUS_MINUS;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_MINUS_EQUAL;
        else if (l == '>')
            return src->fwd++, TOK_PUNCT_ARROW;
        else
            return TOK_PUNCT_MINUS;

    case '*':
        src->fwd++;

        if (dtopc(src) == '=')
            return src->fwd++, TOK_PUNCT_STARR_EQUAL;
        else
            return TOK_PUNCT_STARR;

    case '/':
        src->fwd++;

        if (dtopc(src) == '=')
            return src->fwd++, TOK_PUNCT_SLASH_EQUAL;
        else
            return TOK_PUNCT_SLASH;

    case '%':
        src->fwd++;

        if (dtopc(src) == '=')
            return src->fwd++, TOK_PUNCT_PERCN_EQUAL;
        else
            return TOK_PUNCT_PERCN;

    case '#':
        return src->fwd++, TOK_PUNCT_SHARP;

    case '@':
        src->fwd++;

        if (isalnum(l = dtopc(src)) || (l == '_') || (l == '$') || (l == '.'))
        {
            dadvance(src);

            do
                src->fwd++;
            while (isalnum(l = dtopc(src)) || (l == '_') || (l == '$') || (l == '.'));

            dchopto(src, t);

            if (lexeme)
                *lexeme = t;

            return TOK_IDENT;
        }
        else
        {
            return TOK_PUNCT_ATSIG;
        }

    case ',':
        return src->fwd++, TOK_PUNCT_COMMA;

    case '.':
        src->fwd++;

        if ((l = dtopc(src)) == '.')
        {
            src->fwd++;

            if (dtopc(src) == '.')
                return src->fwd++, TOK_PUNCT_ELLIP;
            else
                return TOK_PUNCT_POINT_POINT;
        }
        else if (isdigit(l))
        {
            goto real_part;
        }
        else
        {
            return TOK_PUNCT_POINT;
        }

    case ':':
        src->fwd++;

        if (dtopc(src) == ':')
            return src->fwd++, TOK_PUNCT_COLON_COLON;
        else
            return TOK_PUNCT_COLON;

    case ';':
        return src->fwd++, TOK_PUNCT_SEMIC;

    case '\\':
        return src->fwd++, TOK_PUNCT_BACKS;

    case '`':
        return src->fwd++, TOK_PUNCT_BACKT;

        // Reserved Words And Identifiers

    case 'a':   case 'b':   case 'c':   case 'd':   case 'e':   case 'f':
    case 'g':   case 'h':   case 'i':   case 'j':   case 'k':   case 'l':
    case 'm':   case 'n':   case 'o':   case 'p':   case 'q':   case 'r':
    case 's':   case 't':   case 'u':   case 'v':   case 'w':   case 'x':
    case 'y':   case 'z':
        src->fwd++;

        if (islower(l = dtopc(src)))
        {
            do
                src->fwd++;
            while (islower(l = dtopc(src)));

            if (!isalnum(l) && (l != '_') && (l != '$'))
            {
                tokcode_t c;

                dchopto(src, t);

                if (lexeme && ((c = get_keyword_or_id(t)) == TOK_IDENT))
                    *lexeme = t;

                return c;
            }
            else
            {
                goto id;
            }
        }
        else
        {
            src->fwd--;
        }

        if (isalnum(l = dtopc(src)) || (l == '_') || (l == '$'))
        {
    case 'A':   case 'B':   case 'C':   case 'D':   case 'E':   case 'F':
    case 'G':   case 'H':   case 'I':   case 'J':   case 'K':   case 'L':
    case 'M':   case 'N':   case 'O':   case 'P':   case 'Q':   case 'R':
    case 'S':   case 'T':   case 'U':   case 'V':   case 'W':   case 'X':
    case 'Y':   case 'Z':   case '_':   case '$':   id:
            do
                src->fwd++;
            while (isalnum(l = dtopc(src)) || (l == '_') || (l == '$'));

            dchopto(src, t);

            if (lexeme)
                *lexeme = t;

            return TOK_IDENT;
        }

        // Numerics

    case '0':
        do
            src->fwd++;
        while ((l = dtopc(src)) == '0');

        if ((l == 'B') || (l == 'b'))
        {
            src->fwd++;

            if ((((l = dtopc(src)) == '0') || (l == '1')))
            {
                do
                {
                    src->fwd++;

                    if (!n && (l == '0'))
                        continue;
                    else
                        t[n++] = l;
                } while ((((l = dtopc(src)) == '0') || (l == '1')));

                if (!n)
                    t[n++] = '0';
            }
            else
            {
                return expected("binary digit", unesc(t, l)), TOK_INVAL;
            }

            if (lexeme)
                t[n] = NUL, *lexeme = t;

            return TOK_LITER_INTGR_BIN;
        }
        else if ((l == 'C') || (l == 'c'))
        {
            src->fwd++;

            if ((((l = dtopc(src)) >= '0') && (l <= '7')))
            {
                do
                {
                    src->fwd++;

                    if (!n && (l == '0'))
                        continue;
                    else
                        t[n++] = l;
                } while ((((l = dtopc(src)) >= '0') && (l <= '7')));

                if (!n)
                    t[n++] = '0';
            }
            else
            {
                return expected("octal digit", unesc(t, l)), TOK_INVAL;
            }

            if (lexeme)
                t[n] = NUL, *lexeme = t;

            return TOK_LITER_INTGR_OCT;
        }
        else if ((l == 'D') || (l == 'd') || isdigit(l))
        {
            if (l >= 'D')
                src->fwd++;

            if ((((l = dtopc(src)) >= '0') && (l <= '9')))
            {
    case '1': case '2': case '3':
    case '4': case '5': case '6':
    case '7': case '8': case '9':
                do
                {
                    src->fwd++;

                    if (!n && (l == '0'))
                        continue;
                    else
                        t[n++] = l;
                } while ((((l = dtopc(src)) >= '0') && (l <= '9')));

                if (!n)
                    t[n++] = '0';
            }
            else
            {
                return expected("decimal digit", unesc(t, l)), TOK_INVAL;
            }

            if (l == DECSEP)
            {
            real:
                src->fwd++, t[n++] = l;

                if (isdigit(l = dtopc(src)))
                {
                real_part:
                    do
                        src->fwd++, t[n++] = l;
                    while (((l = dtopc(src)) >= '0') && (l <= '9'));

                    while ((t[n - 1] == '0') && (t[n - 2] != DECSEP))
                        --n;

                expo_part:
                    if ((l == 'E') || (l == 'e'))
                    {
                        src->fwd++, t[n++] = l;
                        
                        if (((l = dtopc(src)) == '+') || (l == '-'))
                            src->fwd++, t[n++] = l;
                        
                        if (isdigit(l = dtopc(src)))
                        {
                            o = n;

                            do
                            {
                                src->fwd++;

                                if ((n == o) && (l == '0'))
                                    continue;
                                else
                                    t[n++] = l;
                            } while (isdigit(l = dtopc(src)));

                            if (n == o)
                                t[n++] = '0';
                        }
                        else
                        {
                            return unexpected(unesc(t, l), "numeric exponent"), TOK_INVAL;
                        }
                    }

                    if (lexeme)
                        t[n] = NUL, *lexeme = t;

                    return TOK_LITER_FLOAT;
                }
                else
                {
                    return unexpected(unesc(t, l), "real part"), TOK_INVAL;
                }
            }
            else
            {
                if (lexeme)
                    *lexeme = dchop(src);

                return TOK_LITER_INTGR_DEC;
            }
        }
        else if ((l == 'X') || (l == 'x'))
        {
            src->fwd++;

            if (((((l = dtopc(src)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F')) || ((l >= 'a') && (l <= 'f'))))
            {
                do
                {
                    src->fwd++;

                    if (!n && (l == '0'))
                        continue;
                    else
                        t[n++] = l;
                } while (((((l = dtopc(src)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F')) || ((l >= 'a') && (l <= 'f'))));

                if (!n)
                    t[n++] = '0';
            }
            else
            {
                return expected("hexadecimal digit", unesc(t, l)), TOK_INVAL;
            }

            if (lexeme)
                t[n] = NUL, *lexeme = t;

            return TOK_LITER_INTGR_HEX;
        }
        else if (l == DECSEP)
        {
            goto real;
        }
        else
        {
            if (lexeme)
                *lexeme = "0";

            return TOK_LITER_INTGR_DEC;
        }

        // Others

    default:
        src->fwd++;
        return notvalid(unesc(t, l)), TOK_INVAL;
    }
}

#pragma endregion

// +---- Scanner -- End

tokcode_t gettok(doub_t *const src, char **lexeme)
{
    if (lexeme)
        *lexeme = NULL;

    return _gettok(src, lexeme);
}

#ifdef CALC_DEBUG

const char *const tokcode_to_str(const tokcode_t code)
{
    switch (code)
    {
    case TOK_INVAL:
        return "<inv>";

#pragma push_macro("defstr")

#ifndef defstr
#   define defstr(name, lexeme) case TOK_ ## name: \
                                    return lexeme;
#endif

#include "calc-parse.inc"

#ifdef defstr
#   undef defstr
#endif

#pragma pop_macro("defstr")

    default:
        return "<???>";
    }
}

const char *const tokname_to_str(const tokcode_t code)
{
    switch (code)
    {
    case TOK_INVAL:
        return "INVALID";

#pragma push_macro("defstr")

#ifndef defstr
#   define defstr(name, lexeme) case TOK_ ## name: \
                                    return # name;
#endif

#include "calc-parse.inc"

#ifdef defstr
#   undef defstr
#endif

#pragma pop_macro("defstr")

    default:
        return "UKNWOWN";
    }
}

void tokenize()
{
    char line[BUFSIZ];
    bool_t end = FALSE;
    lexer_t *lex = create_lexer(BUFSIZ, 32);

    do
    {
        printf("TOK > ");
        fgets(line, BUFSIZ, stdin);
        dputs(lex->doub, line);

        tokcode_t c;

        do
        {
            c = lnext(lex);

            if (lex->lexm)
                printfn("(%02d) %-24s -> '%s'", (int)c, tokname_to_str(c), lex->lexm);
            else
                printfn("(%02d) %-24s -> '%s'", (int)c, tokname_to_str(c), tokcode_to_str(c));

            if (c == TOK_KWORD_END)
                end = TRUE;
        } while (!end && (c > TOK_NULCH));

        putln();
    } while (!end);

    hashtab_print(lex->htab);

    return;
}

#endif // CALC_DEBUG

#pragma endregion

// +---- Tokens -- End

// +---- Lexer

#pragma region Lexer

lexer_t *create_lexer(unsigned int bufsiz, unsigned int tabsiz)
{
    lexer_t *lex = alloc(lexer_t);

    lex->doub = create_doub(NULL, bufsiz);
    lex->htab = create_hashtab(tabsiz, NULL);
    lex->hkey = NULL;
    lex->look = 0;
    lex->last = 0;

    return lex;
}

tokcode_t llook(lexer_t *const lex) // lookahead
{
    tokcode_t code = _gettok(lex->doub, NULL);

    if (code != TOK_INVAL)
        dretreat(lex->doub);
    else
        dadvance(lex->doub);

    return lex->look = code;
}

tokcode_t lnext(lexer_t *const lex) // lex
{
    lex->lexm = NULL;

    tokcode_t code = _gettok(lex->doub, &lex->lexm);

    if (lex->lexm && (code == TOK_IDENT))
        lex->hkey = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->doub->fwd), (lex->htab->used > 0 ? lex->hkey->data + 1 : 0));

    dadvance(lex->doub);

    return lex->last = code;
}

bool_t lmatch(lexer_t *const lex, tokcode_t match) // match
{
    lex->lexm = NULL;

    tokcode_t code = _gettok(lex->doub, &lex->lexm);

    if (code == match)
    {
        if (lex->lexm && (code == TOK_IDENT))
            lex->hkey = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->doub->fwd), (lex->htab->used > 0 ? lex->hkey->data + 1 : 0));

        dadvance(lex->doub);

        return lex->last = code, TRUE;
    }
    else
    {
        return lex->doub->fwd = 0, FALSE;
    }
}

bool_t vlmatch(lexer_t *const lex, unsigned int count, ...)
{
    lex->lexm = NULL;

    unsigned int i;
    bool_t result = FALSE;
    tokcode_t code = _gettok(lex->doub, &lex->lexm);
    va_list list;

    va_start(list, count);

    for (i = 0; !result && (i < count); i++)
    {
        if (code == va_arg(list, tokcode_t))
        {
            if (lex->lexm && (code == TOK_IDENT))
                lex->hkey = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->doub->fwd), (lex->htab->used > 0 ? lex->hkey->data + 1 : 0));

            dadvance(lex->doub);

            lex->last = code;
            result = TRUE;
        }
        else
        {
            continue;
        }
    }

    va_end(list);

    if (!result)
        lex->doub->fwd = 0;

    return result;
}

#pragma endregion

// +---- Lexer -- End

#pragma endregion

/* =---- Syntax Analyzer ---------------------------------------= */

#pragma region Semantic Analyzer

// +---- Abstract Syntax Tree

#pragma region Abstract Syntax Tree

// +---- Internal

#pragma region Internal (Expression Parsers)

// Value Expressions

static inline ast_expr_t *parse_expr_lval(tokcode_t tok, char *const lexm)
{
    ast_expr_t *lval;

    switch (tok)
    {
    case TOK_LITER_INTGR_BIN:
        lval = create_ast_expr(AST_EXPR_UNSIG);
        lval->node.uint = strtoull(lexm, NULL, 0x02);

        free(lexm);

        break;

    case TOK_LITER_INTGR_OCT:
        lval = create_ast_expr(AST_EXPR_UNSIG);
        lval->node.uint = strtoull(lexm, NULL, 0x08);

        free(lexm);

        break;

    case TOK_LITER_INTGR_DEC:
        lval = create_ast_expr(AST_EXPR_UNSIG);
        lval->node.uint = strtoull(lexm, NULL, 0x0A);

        free(lexm);

        break;

    case TOK_LITER_INTGR_HEX:
        lval = create_ast_expr(AST_EXPR_UNSIG);
        lval->node.uint = strtoull(lexm, NULL, 0x10);

        free(lexm);

        break;

    default:
        expected("a valid operator", lexm);
        return NULL;
    }

    return lval;
}

static inline ast_expr_t *parse_expr_rval(lexer_t *const lex)
{
    return parse_expr_lval(lnext(lex), lex->lexm);
}

// Unary Expressions

static ast_expr_t *_parse_ast_expr_un_sig(lexer_t *const lex, tokcode_t op)
{
    bool_t flag;
    ast_expr_t *expr, *val;

    do
    {
        val = parse_expr_rval(lex);
        expr = create_ast_expr_un(val, op);

        if (flag = vlmatch(lex, 2, TOK_PUNCT_PLUSS, TOK_PUNCT_MINUS))
            val = expr, op = lex->last;
    } while (flag);

    return expr;
}

// Binary Expressions

static ast_expr_t *_parse_ast_expr_bin_prd(lexer_t *const lex, ast_expr_t *lhs, tokcode_t op)
{
    bool_t flag;
    ast_expr_t *expr, *rhs;

    do
    {
        if (vlmatch(lex, 2, TOK_PUNCT_PLUSS, TOK_PUNCT_MINUS))
            rhs = _parse_ast_expr_un_sig(lex, lex->last);
        else
            rhs = parse_expr_rval(lex);

        expr = create_ast_expr_bin(lhs, rhs, op);

        if (flag = vlmatch(lex, 3, TOK_PUNCT_STARR, TOK_PUNCT_SLASH, TOK_PUNCT_PERCN))
            lhs = expr, op = lex->last;
    } while (flag);

    return expr;
}

static ast_expr_t *_parse_ast_expr_bin_sum(lexer_t *const lex, ast_expr_t *lhs, tokcode_t op)
{
    bool_t flag;
    ast_expr_t *expr, *rhs;

    do
    {
        rhs = parse_expr_rval(lex);

        if (vlmatch(lex, 3, TOK_PUNCT_STARR, TOK_PUNCT_SLASH, TOK_PUNCT_PERCN))
            rhs = _parse_ast_expr_bin_prd(lex, rhs, lex->last);

        expr = create_ast_expr_bin(lhs, rhs, op);

        if (flag = vlmatch(lex, 2, TOK_PUNCT_PLUSS, TOK_PUNCT_MINUS))
            lhs = expr, op = lex->last;
    } while (flag);

    return expr;
}

static ast_expr_t *_parse_ast_expr_bin(lexer_t *const lex)
{
    ast_expr_t *expr = NULL, *lhs;

    lhs = parse_expr_lval(lex->last, lex->lexm);

    switch (lnext(lex))
    {
    case TOK_PUNCT_PLUSS:
    case TOK_PUNCT_MINUS:
        expr = _parse_ast_expr_bin_sum(lex, lhs, lex->last);
        break;

    case TOK_PUNCT_STARR:
    case TOK_PUNCT_SLASH:
    case TOK_PUNCT_PERCN:
        expr = _parse_ast_expr_bin_prd(lex, lhs, lex->last);
        break;

    default:
        unexpected("operator", "binary operator");
        break;
    }

    return expr;
}

#pragma endregion

// +---- Internal -- End

ast_expr_t *parse_expr(lexer_t *const lex)
{
    tokcode_t tok = lnext(lex);

    switch (tok)
    {
    case TOK_LITER_INTGR_BIN:
    case TOK_LITER_INTGR_OCT:
    case TOK_LITER_INTGR_DEC:
    case TOK_LITER_INTGR_HEX:
        return _parse_ast_expr_bin(lex);

    default:
        break;
    }
}

// Expression Node

ast_expr_t *create_ast_expr(ast_expr_kind_t kind)
{
    ast_expr_t *expr = alloc(ast_expr_t);

    expr->kind = kind;
    
    switch (kind)
    {
    case AST_EXPR_UNARY:
        expr->node.unexpr = alloc(ast_unexpr_t);

    case AST_EXPR_BINRY:
        expr->node.binexpr = alloc(ast_binexpr_t);
        break;
    }

    return expr;
}

// Unary Expressions

ast_expr_t *create_ast_expr_un(ast_expr_t *const val, tokcode_t op)
{
    ast_expr_t *expr = create_ast_expr(AST_EXPR_UNARY);

    expr->node.unexpr->val = val;
    expr->node.unexpr->op = op;

    return expr;
}

// Binary Expressions

ast_expr_t *create_ast_expr_bin(ast_expr_t *const lhs, ast_expr_t *const rhs, tokcode_t op)
{
    ast_expr_t *expr = create_ast_expr(AST_EXPR_BINRY);

    expr->node.binexpr->lhs = lhs;
    expr->node.binexpr->rhs = rhs;
    expr->node.binexpr->op = op;

    return expr;
}

#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_PARSE_C_
#   define CALC_PARSE_C_
#endif // CALC_PARSE_C_

#pragma warning(pop)