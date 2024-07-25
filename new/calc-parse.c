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
        return sb->buf[sb->fwd++];
#else
        return sb->buf[sb->pos + sb->fwd++];
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
        return sb->buf[sb->fwd++] = c;
#else
        return sb->buf[sb->pos + sb->fwd++] = c;
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