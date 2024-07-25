#include "calc-parse.h"

/* =---- Lexical Analyser --------------------------------------= */

#pragma region Lexical Analyser

// +---- Sources

#pragma region Sources

// Source Buffer

srcbuf_t *create_srcbuf(char *const buffer, unsigned int length)
{
    srcbuf_t *sb = alloc(srcbuf_t);

    if (!length)
#ifdef _CALC_USE_BUFSIZ
        length = BUFSIZ;
#else
        length = pagesiz;
#endif // _CALC_USE_BUFSIZ

    sb->buf = !buffer ? stralloc(length) : buffer;
    sb->pos = 0;
    sb->fwd = 0;
    sb->len = length;

    return sb;
}

srcbuf_t *create_srcbuf_auto()
{
    return create_srcbuf(NULL, 0);
}

void delete_srcbuf(srcbuf_t *const sb)
{
#if CALC_DEBUG
    sbrewnd(sb);
#else
    sb->pos = 0;
    sb->fwd = 0;
    sb->len = 0;
#endif // CALC_DEBUG

    free(sb->buf);
    free(sb);

    return;
}

int sbtopc(srcbuf_t *const sb)
{
    if ((sb->pos + sb->fwd) < sb->len)
#ifdef CALC_DEBUG
        return sb->buf[sb->fwd];
#else
        return sb->buf[sb->pos + sb->fwd];
#endif // CALC_DEBUG
    else
        return EOF;
}

int sbgetc(srcbuf_t *const sb)
{
    if ((sb->pos + sb->fwd) < sb->len)
#ifdef CALC_DEBUG
        return sb->buf[++sb->fwd];
#else
        return sb->buf[sb->pos + ++sb->fwd];
#endif // CALC_DEBUG
    else
        return EOF;
}

int sbsetc(srcbuf_t *const sb, int c)
{
    if ((sb->pos + sb->fwd) < sb->len)
#ifdef CALC_DEBUG
        return sb->buf[sb->fwd] = c;
#else
        return sb->buf[sb->pos + sb->fwd] = c;
#endif
    else
        return EOF;
}

int sbputc(srcbuf_t *const sb, int c)
{
    if ((sb->pos + sb->fwd) < sb->len)
#ifdef CALC_DEBUG
        return sb->buf[++sb->fwd] = c;
#else
        return sb->buf[sb->pos + ++sb->fwd] = c;
#endif
    else
        return EOF;
}

char *sbtops(char *const dst, srcbuf_t *const sb, unsigned int count)
{
    if (!count)
        return NULL;

    char *buf;

    if (!dst)
        buf = stralloc(count);
    else
        buf = dst;

    int i = 0, c;

    do
    {
        c = sbgetc(sb);

        if (c == EOF)
            break;
        else
            buf[i++] = c;
    } while ((c != EOL) && (c != NUL) && (i < count));

    sb->fwd = 0;
    buf[i] = NUL;

    return buf;
}

char *sbgets(char *const dst, srcbuf_t *const sb, unsigned int count)
{
    if (!count)
        return NULL;

    char *buf;

    if (!dst)
        buf = stralloc(count);
    else
        buf = dst;

    int i = 0, c;

    do
    {
        c = sbgetc(sb);

        if (c == EOF)
            break;
        else
            buf[i++] = c;
    } while ((c != EOL) && (c != NUL) && (i < count));

    buf[i] = NUL;

    return buf;
}

char *sbsets(srcbuf_t *const sb, char *const src, unsigned int count)
{
    if (!src || !count)
        return NULL;

    int i = 0, c;

    do
        c = sbputc(sb, src[i++]);
    while ((c != EOF) && src[i] && (i < count));

    sbputc(sb, '\0');

    return sb->fwd = 0, src;
}

char *sbputs(srcbuf_t *const sb, char *const src, unsigned int count)
{
    if (!src || !count)
        return NULL;

    int i = 0, c;

    do
        c = sbputc(sb, src[i++]);
    while ((c != EOF) && src[i] && (i < count));

    sbputc(sb, '\0');

    return src;
}

char *sbchop(srcbuf_t *const sb)
{
#ifdef CALC_DEBUG
    return strndcpy(NULL, sb->buf, sb->fwd);
#else
    return strndcpy(NULL, sb->buf + sb->pos, sb->fwd);
#endif // CALC_DEBUG
}

char *sbchto(char *const dst, srcbuf_t *const sb)
{
#ifdef CALC_DEBUG
    return strndcpy(dst, sb->buf, sb->fwd);
#else
    return strndcpy(dst, sb->buf + sb->pos, sb->fwd);
#endif // CALC_DEBUG
}

char *sbchof(char *const dst, srcbuf_t *const sb, unsigned int count)
{
#ifdef CALC_DEBUG
    return strndcpy(dst, sb->buf, count);
#else
    return strndcpy(dst, sb->buf + sb->pos, count);
#endif // CALC_DEBUG
}

char *sbgetbuf(srcbuf_t *const sb)
{
#ifdef CALC_DEBUG
    return sb->buf;
#else
    return sb->buf + sb->pos;
#endif // CALC_DEBUG
}

void sbadvnc(srcbuf_t *const sb)
{
    if ((sb->pos + sb->fwd) < sb->len)
    {
#ifdef CALC_DEBUG
        sb->buf += sb->fwd;
#endif
        sb->pos += sb->fwd;
        sb->fwd = 0;
    }
    else
    {
#ifdef CALC_DEBUG
        sb->buf += (sb->pos + sb->fwd) - sb->len;
#endif // CALC_DEBUG
        sb->pos += (sb->pos + sb->fwd) - sb->len;
        sb->fwd = 0;
    }

    return;
}

void sbretrt(srcbuf_t *const sb)
{
    sb->fwd = 0;

    return;
}

void sbrewnd(srcbuf_t *const sb)
{
#ifdef CALC_DEBUG
    sb->buf -= sb->pos;
#endif // CALC_DEBUG
    sb->pos = 0;
    sb->fwd = 0;

    return;
}

// Source Stream

#pragma endregion

// +---- Sources -- End

// +---- Tokens

#pragma region Tokens

// +---- Internal (Lexical Errors)

#pragma region Internal (Lexical Errors)

static inline int _expected(const char *const what, const char *const got)
{
    int result;

    if (!what)
        result = error("lexical error: unexpected input sequence");
    else
        result = errorf("lexical error: expected %s", what);

    if (!got)
        result += errorln(".");
    else
        result += errorfn(" (got %s).");

    return result;
}

static inline int _unexpctd(const char *const what, const char *const expected)
{
    int result;

    if (!what)
        result = error("lexical error: unexpected input sequence");
    else
        result = errorf("lexical error: unexpected %s", what);

    if (!expected)
        result += errorln(".");
    else
        result += errorfn(" (expected %s).");

    return result;
}

static inline int _notvalid(const char *const what)
{
    int result;

    if (!what)
        result = errorln("lexical error: input sequence not valid.");
    else
        result = errorfn("lexical error: invalid %s.", what);

    return result;
}

#pragma endregion

// +---- Internal (Lexical Errors) -- End

// +---- Internal (Lexical Scanner)

#pragma region Internal (Lexical Scanner)

// token info structure
struct token
{
    char *lexm;
    tokcode_t code;
};

static inline const struct token *_tok_binsearch(const struct token *const tab, char *const lexeme, const unsigned int count)
{
    int hig = 0, mid, low = count - 1, k = 0;

    do
    {
        mid = (hig + low) >> 1;

        switch (numcmp(*(lexeme + k), *(tab[mid].lexm + k)))
        {
        case +1:
            hig = mid + 1, k = 0;
            break;

        case -1:
            low = mid - 1, k = 0;
            break;

        default:
            if (streq(lexeme, tab[mid].lexm))
                return &tab[mid];
            else
                ++k;

            break;
        }
    } while (hig <= low);

    return NULL;
}

static inline const struct token *_tok_linsearch(const struct token *const tab, char *const lexeme, const unsigned int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (streq(lexeme, tab[i].lexm))
            return &tab[i];
        else
            continue;
    }

    return NULL;
}

static const struct token *_getkword(char *const lexeme)
{
    static const struct token keystab[] = {
#pragma push_macro("defkey")

#ifndef defkey
#   define defkey(tok_name, tok_lexeme) {.code = TOK_##tok_name, .lexm = tok_lexeme},
#endif

#include "calc-parse.inc"

#ifdef defkey
#   undef defkey
#endif

#pragma pop_macro("defkey")

        NULL};

    static const int count = (sizeof(keystab) / sizeof(*keystab));

#if CALC_SORTED_KEYWORDS // if keywords are soreted, it does a binary search
    return _tok_binsearch(keystab, lexeme, count);
#else  // if keywords aren't sorted, it does a linear search
    return _tok_linsearch(keystab, lexeme, count);
#endif // CALC_SORTED_KEYWORDS
}

static const struct token *_getpragm(char *const lexeme)
{
    static const struct token pragtab[] = {
#pragma push_macro("defprg")

#ifndef defprg
#   define defprg(tok_name, tok_lexeme) {.code = TOK_##tok_name, .lexm = tok_lexeme},
#endif

#include "calc-parse.inc"

#ifdef defprg
#   undef defprg
#endif

#pragma pop_macro("defprg")

        NULL};

    static const int count = (sizeof(pragtab) / sizeof(*pragtab));

#if CALC_SORTED_PRAGMATICS // if pragmatics are soreted, it does a binary search
    return _tok_binsearch(pragtab, lexeme, count);
#else  // if pragmatics aren't sorted, it does a linear search
    return _tok_linsearch(pragtab, lexeme, count);
#endif // CALC_SORTED_PRAGMATICS
}

static tokcode_t _gettoken(srcbuf_t *const sb, char *const buf)
{
    int l /* lookahead character */, p = 0, q = 0, t = TOK_INVAL;

redo:
    while (isspace(l = sbtopc(sb)))
        ++sb->buf, ++sb->pos;

    switch (l)
    {
        // Trivia

    case EOF:
        t = TOK_ENDOF;
        break;

    case NUL:
        t = TOK_NULCH;
        break;

        // Brackets

    case '(':
        do
            ++sb->fwd;
        while (isspace(l = sbtopc(sb)));

        if ((l = sbtopc(sb)) == ')')
            t = TOK_PUNCT_ROUND, ++sb->fwd;
        else
            t = TOK_PUNCT_LROUN;

        break;

    case ')':
        t = TOK_PUNCT_RROUN, ++sb->fwd;
        break;

    case '[':
        do
            ++sb->fwd;
        while (isspace(l = sbtopc(sb)));

        if ((l = sbtopc(sb)) == ']')
            t = TOK_PUNCT_SQRED, ++sb->fwd;
        else
            t = TOK_PUNCT_LSQRD;

        break;
    
    case ']':
        t = TOK_PUNCT_RSQRD, ++sb->fwd;
        break;

    case '{':
        do
            ++sb->fwd;
        while (isspace(l = sbtopc(sb)));

        if ((l = sbtopc(sb)) == '}')
            t = TOK_PUNCT_CURLY, ++sb->fwd;
        else
            t = TOK_PUNCT_LCURL;

        break;

    case '}':
        t = TOK_PUNCT_RCURL, ++sb->fwd;
        break;

        // Punctuators

    case '~':
        t = TOK_PUNCT_TILDE, ++sb->fwd;
        break;

    case '?':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '?')
            t = TOK_PUNCT_QUEST_QUEST, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_QUEST_EQUAL, ++sb->fwd;
        else if (l == '.')
            t = TOK_PUNCT_QUEST_POINT, ++sb->fwd;
        else
            t = TOK_PUNCT_QUEST;

        break;

    case '!':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '!')
            t = TOK_PUNCT_EXCLM_EXCLM, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_EXCLM_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_EXCLM;

        break;

    case '&':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '&')
            t = TOK_PUNCT_AMPER_AMPER, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_AMPER_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_AMPER;

        break;

    case '|':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '|')
            t = TOK_PUNCT_PIPEE_PIPEE, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_PIPEE_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_PIPEE;

        break;

    case '^':
        ++sb->fwd;

        if (l == '=')
            t = TOK_PUNCT_CARET_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_CARET;

        break;

    case '<':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '<')
        {
            ++sb->fwd;

            if ((l = sbtopc(sb)) == '=')
                t = TOK_PUNCT_LESST_LESST_EQUAL;
            else
                t = TOK_PUNCT_LESST_LESST;
        }
        else if (l == '=')
        {
            t = TOK_PUNCT_LESST_EQUAL, ++sb->fwd;
        }
        else
        {
            t = TOK_PUNCT_LESST;
        }

        break;

    case '>':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '<')
        {
            ++sb->fwd;

            if ((l = sbtopc(sb)) == '=')
                t = TOK_PUNCT_GREAT_GREAT_EQUAL;
            else
                t = TOK_PUNCT_GREAT_GREAT;
        }
        else if (l == '=')
        {
            t = TOK_PUNCT_GREAT_EQUAL, ++sb->fwd;
        }
        else
        {
            t = TOK_PUNCT_GREAT;
        }

        break;

    case '=':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '=')
            t = TOK_PUNCT_EQUAL_EQUAL, ++sb->fwd;
        else if (l == '>')
            t = TOK_PUNCT_THENN, ++sb->fwd;
        else
            t = TOK_PUNCT_EQUAL;

        break;

    case '+':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '+')
            t = TOK_PUNCT_PLUSS_PLUSS, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_PLUSS_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_PLUSS;

        break;

    case '-':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '-')
            t = TOK_PUNCT_MINUS_MINUS, ++sb->fwd;
        else if (l == '=')
            t = TOK_PUNCT_MINUS_EQUAL, ++sb->fwd;
        else if (l == '>')
            t = TOK_PUNCT_ARROW, ++sb->fwd;
        else
            t = TOK_PUNCT_MINUS;

        break;

    case '*':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '=')
            t = TOK_PUNCT_COLON_COLON, ++sb->fwd;
        else
            t = TOK_PUNCT_COLON;

        break;

    case '/':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '/')
        {
            do
                ++sb->fwd;
            while (((l = sbtopc(sb)) != EOF) && (l != EOL) && (l != NUL));

            goto redo;
        }
        else if (l == '*')
        {
            bool_t stop = FALSE;

            do
            {
                ++sb->fwd;

                if ((l = sbtopc(sb)) == '*')
                {
                    ++sb->fwd;

                    if ((l = sbtopc(sb)) == '/')
                        stop = TRUE;
                    else
                        continue;
                }
            } while (!stop);
            
            goto redo;
        }
        else if (l == '=')
        {
            t = TOK_PUNCT_SLASH_EQUAL, ++sb->fwd;
        }
        else
        {
            t = TOK_PUNCT_SLASH;
        }

        break;

    case '%':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '=')
            t = TOK_PUNCT_PERCN_EQUAL, ++sb->fwd;
        else
            t = TOK_PUNCT_PERCN;

        break;

    case '#':
        do
        {
            ++sb->fwd;

            if ((l = sbtopc(sb)) == EOL)
            {
                do
                    ++sb->fwd;
                while (isspace(l = sbtopc(sb)));

                if (l == '#')
                    continue;
                else
                    break;
            }
        } while ((l != EOF) && (l != NUL));

        sbchto(buf, sb);

        t = TOK_PUNCT_SHARP;

        break;

    case '@':
        ++sb->fwd;

        if (isalnum(l = sbtopc(sb)) || (l == '_') || (l == '$') || (l == '.'))
        {
            do
                buf[p++] = l, ++sb->fwd;
            while (isalnum(l = sbtopc(sb)) || (l == '_') || (l == '$') || (l == '.'));

            t = TOK_IDENT;
        }
        else
        {
            t = TOK_PUNCT_ATSIG;
        }

        break;

    case ',':
        t = TOK_PUNCT_COMMA, ++sb->fwd;
        break;

    case '.':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == '.')
        {
            ++sb->fwd;

            if ((l = sbtopc(sb)) == '.')
                t = TOK_PUNCT_ELLIP;
            else
                t = TOK_PUNCT_POINT_POINT;
        }
        else if (isdigit(l))
        {
            buf[p++] = '.';
        }
        else
        {
            t = TOK_PUNCT_POINT;
        }

        break;

    case ':':
        ++sb->fwd;

        if ((l = sbtopc(sb)) == ':')
            t = TOK_PUNCT_COLON_COLON, ++sb->fwd;
        else
            t = TOK_PUNCT_COLON;

        break;

    case ';':
        t = TOK_PUNCT_SEMIC, ++sb->fwd;
        break;

    case '\\':
        t = TOK_PUNCT_BACKS, ++sb->fwd;
        break;

    case '`':
        t = TOK_PUNCT_BACKT, ++sb->fwd;
        break;

        // Keywords and Identifiers

    case 'a':   case 'b':   case 'c':   case 'd':   case 'e':   case 'f':
    case 'g':   case 'h':   case 'i':   case 'j':   case 'k':   case 'l':
    case 'm':   case 'n':   case 'o':   case 'p':   case 'q':   case 'r':
    case 's':   case 't':   case 'u':   case 'v':   case 'w':   case 'x':
    case 'y':   case 'z':   id_or_kw:
        ++sb->fwd;

        if (islower(l = sbtopc(sb)))
        {
            do
                ++sb->fwd;
            while (islower(l = sbtopc(sb)));

            if (!isalnum(l) && (l != '_') && (l != '$'))
            {
                const struct token *tok;

                sbchto(buf, sb);

                if (!(tok = _getkword(buf)))
                    t = TOK_IDENT;
                else
                    t = tok->code;

                break;  
            }
            else
            {
                goto id;
            }
        }

        if (isalnum(l) || (l == '_') || (l == '$'))
        {
    case 'A':   case 'B':   case 'C':   case 'D':   case 'E':   case 'F':
    case 'G':   case 'H':   case 'I':   case 'J':   case 'K':   case 'L':
    case 'M':   case 'N':   case 'O':   case 'P':   case 'Q':   case 'R':
    case 'S':   case 'T':   case 'U':   case 'V':   case 'W':   case 'X':
    case 'Y':   case 'Z':   case '_':   case '$':   id:
            do
                ++sb->fwd;
            while (isalnum(l) || (l == '_') || (l == '$'));
        }

        sbchto(buf, sb);

        t = TOK_IDENT;

        break;

        // Numerics

    case '0':
        do
            ++sb->fwd;
        while ((l = sbtopc(sb)) == '0');

        if ((l == 'B') || (l == 'b'))
        {
            ++sb->fwd;

            if ((((l = sbtopc(sb)) == '0') || (l == '1')))
            {
                do
                {
                    ++sb->fwd;

                    if (!p && (l == '0'))
                        continue;
                    else
                        buf[p++] = l;
                } while ((((l = sbtopc(sb)) == '0') || (l == '1')));

                if (!p)
                    buf[p++] = '0';

                t = TOK_LITER_INTGR_BIN, buf[p] = NUL;
            }
            else
            {
                t = TOK_INVAL, _expected("binary digit", unesc(buf, l));
            }
        }
        else if ((l == 'C') || (l == 'c'))
        {
            ++sb->fwd;

            if ((((l = sbtopc(sb)) >= '0') && (l <= '7')))
            {
                do
                {
                    ++sb->fwd;

                    if (!p && (l == '0'))
                        continue;
                    else
                        buf[p++] = l;
                } while ((((l = sbtopc(sb)) >= '0') && (l <= '7')));

                if (!p)
                    buf[p++] = '0';

                t = TOK_LITER_INTGR_OCT, buf[p] = NUL;
            }
            else
            {
                t = TOK_INVAL, _expected("octal digit", unesc(buf, l));
            }
        }
        else if ((l == 'D') || (l == 'd'))
        {
            ++sb->fwd;

            if ((((l = sbtopc(sb)) >= '0') && (l <= '9')))
            {
    case '1': case '2': case '3':
    case '4': case '5': case '6':
    case '7': case '8': case '9':
                do
                {
                    ++sb->fwd;

                    if (!p && (l == '0'))
                        continue;
                    else
                        buf[p++] = l;
                } while ((((l = sbtopc(sb)) >= '0') && (l <= '9')));

                if (!p)
                    buf[p++] = '0';

                if (l == '.')
                {
                real:
                    ++sb->fwd, buf[p++] = l;

                    if (isdigit(l = sbtopc(sb)))
                    {
                    real_part:
                        do
                            ++sb->fwd, buf[p++] = l;
                        while (((l = sbtopc(sb)) >= '0') && (l <= '9'));

                        while ((buf[p - 1] == '0') && (buf[p - 2] != '.'))
                            --p;

                    expo_part:
                        if ((l == 'E') || (l == 'e'))
                        {
                            ++sb->fwd, buf[p++] = l;
                        
                            if (((l = sbtopc(sb)) == '+') || (l == '-'))
                                ++sb->fwd, buf[p++] = l;
                        
                            if (isdigit(l = sbtopc(sb)))
                            {
                                q = p;

                                do
                                {
                                    ++sb->fwd;

                                    if ((p == q) && (l == '0'))
                                        continue;
                                    else
                                        buf[p++] = l;
                                } while (isdigit(l = sbtopc(sb)));

                                if (p == q)
                                    buf[p++] = '0';
                            }
                            else
                            {
                                t = TOK_INVAL, _unexpctd(unesc(buf, l), "numeric exponent");
                            }
                        }

                        t = TOK_LITER_FLOAT, buf[p] = NUL;
                    }
                    else
                    {
                        t = TOK_INVAL, _unexpctd(unesc(buf, l), "real part");
                    }
                }
                else
                {
                    t = TOK_LITER_INTGR_DEC, buf[p] = NUL;
                }
            }
            else
            {
                t = TOK_INVAL, _expected("decimal digit", unesc(buf, l));
            }
        }
        else if ((l == 'X') || (l == 'x'))
        {
            ++sb->fwd;

            if ((((l = sbtopc(sb)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F')) || ((l >= 'a') && (l <= 'f')))
            {
                do
                {
                    ++sb->fwd;

                    if (!p && (l == '0'))
                        continue;
                    else
                        buf[p++] = l;
                } while ((((l = sbtopc(sb)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F')) || ((l >= 'a') && (l <= 'f')));

                if (!p)
                    buf[p++] = '0';
            }
            else
            {
                t = TOK_INVAL, _expected("hexadecimal digit", unesc(buf, l));
            }

            t = TOK_LITER_INTGR_HEX, buf[p] = NUL;
        }
        else if (l == '.')
        {
            goto real;
        }
        else
        {
            buf[0] = '0';
            buf[1] = NUL;

            t = TOK_LITER_INTGR_DEC;
        }

        break;

        // Others

    default:
        _notvalid(formatto(buf, "token ('%s)", unesc(buf, l)));
        break;
    }

    return (tokcode_t)t;
}

#pragma endregion

// +---- Internal (Lexical Scanner) -- End

tokcode_t getkword(char *const lexeme)
{
    const struct token *tok = _getkword(lexeme);

    if (tok)
        return tok->code;
    else
        return TOK_IDENT;
}

tokcode_t getpragm(char *const lexeme)
{
    const struct token *tok = _getpragm(lexeme);

    if (tok)
        return tok->code;
    else
        return TOK_IDENT;
}

tokcode_t gettoken(srcbuf_t *const sb, char **const lexeme)
{
    static char buf[BUFSIZ] = { 0 };

    if (lexeme)
        *lexeme = buf;

    return _gettoken(sb, buf);
}

#ifndef _CALC_MINIMAL_BUILD

const char *const toktostr(const tokcode_t tok)
{
    switch (tok)
    {
    case TOK_INVAL:
        return "<inv>";

#pragma push_macro("defstr")

#ifndef defstr
#define defstr(name, lexeme) \
    case TOK_##name:         \
        return lexeme;
#endif

#include "calc-parse.inc"

#ifdef defstr
#undef defstr
#endif

#pragma pop_macro("defstr")

    default:
        return "<\?\?\?>";
    }
}

const char *const tokname(const tokcode_t tok)
{
    switch (tok)
    {
    case TOK_INVAL:
        return "INVALID";

#pragma push_macro("defstr")

#ifndef defstr
#define defstr(name, lexeme) \
    case TOK_##name:         \
        return #name;
#endif

#include "calc-parse.inc"

#ifdef defstr
#undef defstr
#endif

#pragma pop_macro("defstr")

    default:
        return "UKNWOWN";
    }
}

bool_t tokenize(FILE *const stream)
{
}

bool_t tokenzto(FILE *const stream, FILE *const out)
{
}

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Tokens -- End

// +---- Lexer

#pragma region Lexer

lexer_t *create_lexer(unsigned int bufsiz, unsigned int tabsiz);
lexer_t *create_lexer_from(srcbuf_t *const sb, unsigned int tabsiz);
void delete_lexer(lexer_t *const lex);

hashbuc_t *ladd(lexer_t *const lex, char *const name);
hashbuc_t *lget(lexer_t *const lex, char *const name);
hashbuc_t *lset(lexer_t *const lex, char *const name, unsigned int data);

tokcode_t llook(lexer_t *const lex);
tokcode_t lnext(lexer_t *const lex);

bool_t lmatch(lexer_t *const lex, tokcode_t match);
bool_t vlmatch(lexer_t *const lex, unsigned int count, ...);

#pragma endregion

// +---- Lexer -- End

#pragma endregion

/* =---- Syntactic Analyser ------------------------------------= */