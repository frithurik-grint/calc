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

static tokcode_t _gettoken(srcbuf_t *const sb, char **const out)
{
    static char buf[BUFSIZ] = { 0 };

    int l /* lookahead character */, p = 0, q = 0, t = TOK_INVAL;

redo:
    while (isspace(l = sbtopc(sb)))
        ++sb->buf, ++sb->pos;

    buf[0] = NUL;

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
            buf[p++] = '0';
            buf[p++] = NUL;

            t = TOK_LITER_INTGR_DEC;
        }

        break;

        // Others

    default:
        _notvalid(formatto(buf, "token ('%s)", unesc(buf, l))), buf[0] = NUL;
        break;
    }

    if (out)
        *out = buf;

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
    return _gettoken(sb, lexeme);
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

void tokenize()
{
    char line[BUFSIZ];
    bool_t end = FALSE;
    lexer_t *lex = create_lexer(BUFSIZ, 32);

    do
    {
        printf("TOK > ");
        fgets(line, BUFSIZ, stdin);
        sbputs(lex->sbuf, line, BUFSIZ);

        tokcode_t c;

        do
        {
            c = lnext(lex);

            if (lex->lexm)
                printfn("(%02d) %-24s -> '%s'", (int)c, toktostr(c), lex->lexm);
            else
                printfn("(%02d) %-24s -> '%s'", (int)c, tokname(c), toktostr(c));

            if (c == TOK_KWORD_END)
                end = TRUE;
        } while (!end && (c > TOK_NULCH));

        putln();
    } while (!end);

    hashtab_dump(stdout, lex->ptab);
    hashtab_dump(stdout, lex->htab);

    return;
}

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Tokens -- End

// +---- Lexer

#pragma region Lexer

// +---- Internal

#pragma region Internal

static hash_t _get_id_hashcode(char *const id)
{
    char *str = id;

    if (!str || (*str <= 0x20))
		return HASH_INV; // intentionally returns an unmanageable value

	hash_t hash;

	for (hash = HASH_MIN; *str > 0x20; str++)
		hash += *str - 0x21;

	return hash;
}

#pragma endregion

// +---- Internal -- End

lexer_t *create_lexer(unsigned int bufsiz, unsigned int tabsiz)
{
    return create_lexer_from(create_srcbuf(NULL, bufsiz), tabsiz);
}

lexer_t *create_lexer_from(srcbuf_t *const sb, unsigned int tabsiz)
{
    lexer_t *lex = alloc(lexer_t);

    lex->sbuf = sb;
    lex->ptab = create_hashtab(tabsiz, _get_id_hashcode, NULL);
    lex->htab = create_hashtab(tabsiz, _get_id_hashcode, NULL);
    lex->buck = NULL;
    lex->look = TOK_INVAL;
    lex->last = TOK_INVAL;

    return lex;
}

void delete_lexer(lexer_t *const lex)
{
    delete_srcbuf(lex->sbuf);
    delete_hashtab(lex->ptab);
    delete_hashtab(lex->htab);

    free(lex);

    return;
}

hashbuc_t *ladd(lexer_t *const lex, char *const name)
{
    return hashtab_add(lex->ptab, name);
}

hashbuc_t *lget(lexer_t *const lex, char *const name)
{
    return hashtab_get(lex->ptab, name);
}

hashbuc_t *lset(lexer_t *const lex, char *const name, unsigned int data)
{
    return hashtab_set(lex->ptab, name, data);
}

tokcode_t llook(lexer_t *const lex)
{
    tokcode_t tokcode = _gettoken(lex->sbuf, NULL);

    if (tokcode != TOK_INVAL)
        sbretrt(lex->sbuf);
    else
        sbadvnc(lex->sbuf);

    return lex->look = tokcode;
}

tokcode_t lnext(lexer_t *const lex)
{
    tokcode_t tokcode = _gettoken(lex->sbuf, &lex->lexm);

    if (lex->lexm && (tokcode == TOK_IDENT))
    {
        if (hashtab_contains(lex->ptab, lex->lexm))
            tokcode = TOK_RWORD, lex->buck = hashtab_get(lex->ptab, lex->lexm);
        else
            lex->buck = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->sbuf->fwd));
    }

    sbadvnc(lex->sbuf);

    return tokcode;
}

bool_t lmatch(lexer_t *const lex, tokcode_t match)
{
    tokcode_t tokcode = _gettoken(lex->sbuf, &lex->lexm);

    if (tokcode == match)
    {
        if (lex->lexm && (tokcode == TOK_IDENT))
        {
            if (hashtab_contains(lex->ptab, lex->lexm))
                tokcode = TOK_RWORD, lex->buck = hashtab_get(lex->ptab, lex->lexm);
            else
                lex->buck = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->sbuf->fwd));
        }

        sbadvnc(lex->sbuf);

        return TRUE;
    }
    else
    {
        return lex->lexm = NULL, lex->sbuf->fwd = 0, FALSE;
    }
}

bool_t vlmatch(lexer_t *const lex, unsigned int count, ...)
{
    unsigned int i;
    bool_t result = FALSE;
    tokcode_t tokcode = _gettoken(lex->sbuf, &lex->lexm);
    va_list list;

    va_start(list, count);

    for (i = 0; !result && (i < count); i++)
    {
        if (tokcode == va_arg(list, tokcode_t))
        {
            if (lex->lexm && (tokcode == TOK_IDENT))
            {
                if (hashtab_contains(lex->ptab, lex->lexm))
                    tokcode = TOK_RWORD, lex->buck = hashtab_get(lex->ptab, lex->lexm);
                else
                    lex->buck = hashtab_add(lex->htab, strndcpy(NULL, lex->lexm, lex->sbuf->fwd));
            }

            sbadvnc(lex->sbuf);

            lex->last = tokcode, result = TRUE;
        }
        else
        {
            continue;
        }
    }

    va_end(list);

    if (!result)
        lex->sbuf->fwd = 0;

    return result;
}

#pragma endregion

// +---- Lexer -- End

#pragma endregion

/* =---- Syntactic Analyser ------------------------------------= */

#pragma region Syntactic Analyser

// +---- Abstract Syntax Tree

#pragma region Abstract Syntax Tree

// +---- Types

#pragma region Types

// +---- Data Types

#pragma region Data Types

// +---- Internal (Floating-Point Types)

#pragma region Internal (Floating-Point Types)

static type_t _ty_floats[] = {
    {
        .name = "binary16",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "binary32",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "binary64",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "binary80",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "binary128",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "binary256",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "decimal32",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "decimal64",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    },
    {
        .name = "decimal128",
        .code = TYCOD_DT_FLOAT,
        .data = NULL
    }
};

static const type_t *const _get_flt_binary16()
{
    type_t *bin16 = &_ty_floats[TYFLT_BIN16];

    if (!bin16->data.fltty)
    {
        bin16->data.fltty = alloc(type_float_t);

        bin16->data.fltty->width = 16;
        bin16->data.fltty->expnt = 5;
        bin16->data.fltty->signf = 10;
    }

    return bin16;
}

static const type_t *const _get_flt_binary32()
{
    type_t *bin32 = &_ty_floats[TYFLT_BIN32];

    if (!bin32->data.fltty)
    {
        bin32->data.fltty = alloc(type_float_t);

        bin32->data.fltty->width = 32;
        bin32->data.fltty->expnt = 8;
        bin32->data.fltty->signf = 23;
    }

    return bin32;
}

static const type_t *const _get_flt_binary64()
{
    type_t *bin64 = &_ty_floats[TYFLT_BIN64];

    if (!bin64->data.fltty)
    {
        bin64->data.fltty = alloc(type_float_t);

        bin64->data.fltty->width = 64;
        bin64->data.fltty->expnt = 11;
        bin64->data.fltty->signf = 53;
    }

    return bin64;
}

static const type_t *const _get_flt_binary128()
{
    type_t *bin128 = &_ty_floats[TYFLT_BIN128];

    if (!bin128->data.fltty)
    {
        bin128->data.fltty = alloc(type_float_t);

        bin128->data.fltty->width = 128;
        bin128->data.fltty->expnt = 15;
        bin128->data.fltty->signf = 112;
    }

    return bin128;
}

static const type_t *const _get_flt_binary256()
{
    type_t *bin256 = &_ty_floats[TYFLT_BIN256];

    if (!bin256->data.fltty)
    {
        bin256->data.fltty = alloc(type_float_t);

        bin256->data.fltty->width = 256;
        bin256->data.fltty->expnt = 19;
        bin256->data.fltty->signf = 236;
    }

    return bin256;
}

static const type_t *const _get_flt_decimal32()
{
    type_t *dec32 = &_ty_floats[TYFLT_DEC32];

    if (!dec32->data.fltty)
    {
        dec32->data.fltty = alloc(type_float_t);

        dec32->data.fltty->width = 32;
        dec32->data.fltty->expnt = 11;
        dec32->data.fltty->signf = 20;
    }

    return dec32;
}

static const type_t *const _get_flt_decimal64()
{
    type_t *dec64 = &_ty_floats[TYFLT_DEC32];

    if (!dec64->data.fltty)
    {
        dec64->data.fltty = alloc(type_float_t);

        dec64->data.fltty->width = 64;
        dec64->data.fltty->expnt = 13;
        dec64->data.fltty->signf = 50;
    }

    return dec64;
}

static const type_t *const _get_flt_decimal128()
{
    type_t *dec128 = &_ty_floats[TYFLT_DEC128];

    if (!dec128->data.fltty)
    {
        dec128->data.fltty = alloc(type_float_t);

        dec128->data.fltty->width = 128;
        dec128->data.fltty->expnt = 17;
        dec128->data.fltty->signf = 110;
    }

    return dec128;
}

#pragma endregion

// +---- Internal (Floating-Point Types) -- End

type_t *get_type_float_by_format(tyflt_format_t format)
{
    
}

type_t *get_type_float_by_name(char *const name);

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

tydata_t alloc_tydata(tycode_t code);

type_t *create_type(char *name, tycode_t code);

#pragma endregion

// +---- Types -- End

// +---- Symbols

#pragma region Symbols

#pragma endregion

// +---- Symbols -- End

#pragma endregion

// +---- Abstract Syntax Tree -- End

#pragma endregion

/* =------------------------------------------------------------= */