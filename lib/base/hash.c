#include "calc/base/hash.h"

/* =---- Hashing -----------------------------------------------= */

#pragma region Hashing

_API hashbuc_t *_CDECL create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev)
{
    hashbuc_t *bucket = alloc(hashbuc_t);

    bucket->name = name;
    bucket->hash = hash;
    bucket->data = data;

    if (prev)
        bucket->next = prev->next, prev->next = bucket;
    else
        bucket->next = NULL;

    return bucket;
}

_API hashbuc_t *_CDECL delete_hashbuc(hashbuc_t *const bucket)
{
    hashbuc_t *next = bucket->next;

    free(bucket->name);
    free(bucket);

    return next;
}

// +---- Hash Table --------------------------------------------+

#pragma region Hash Table

_API hashtab_t *_CDECL create_hashtab(unsigned int size, hashfnc_t func, hashtab_t *const prev)
{
    hashtab_t *tab = alloc(hashtab_t);

    if (!size)
        size = CALC_HASHTAB_BUCKSNUM;

    tab->buck = dimz(hashbuc_t *, size);
    tab->func = func;
    tab->size = size;
    tab->used = 0;
    tab->prev = prev;

    return tab;
}

_API hashtab_t *_CDECL delete_hashtab(hashtab_t *const tab)
{
    unsigned int i;

    for (i = 0; tab->used && (i < tab->size); i++)
    {
        hashbuc_t *bucket = tab->buck[i];

        do
            bucket = delete_hashbuc(bucket);
        while (bucket);

        --tab->used;
    }

    hashtab_t *prev = tab->prev;

    free(tab->buck);
    free(tab);

    return prev;
}

_API hashbuc_t *_CDECL hashtab_add(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashbuc_t *prev;

    if (tab->buck[hash])
    {
        hashbuc_t *buck = tab->buck[hash];

        do
        {
            if (streq(key, buck->name))
                return buck;
            else
                prev = buck, buck = prev->next;
        } while (buck);
    }
    else
    {
        prev = NULL;
    }

    return tab->used++, tab->buck[hash] = create_hashbuc(key, hash, 0, prev);
}

_API hashbuc_t *_CDECL hashtab_get(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);

    return NULL;
}

_API hashbuc_t *_CDECL hashtab_set(hashtab_t *const tab, char *const key, unsigned int data)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return buck->data = data, buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);

    return NULL;
}

_API bool_t _CDECL hashtab_contains(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return TRUE;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);

    return FALSE;
}

_API hashbuc_t *_CDECL hashtab_remove(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return tbp->buck[hash] = buck->next, buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);

    return NULL;
}

_API hashbuc_t *_CDECL hashtab_delete(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return tbp->buck[hash] = delete_hashbuc(buck);
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);

    return NULL;
}

#ifndef _CALC_MINIMAL_BUILD

_API void _CDECL hashtab_dump(FILE *const stream, hashtab_t *const tab)
{
    unsigned int i, c, d;

    for (i = 0, c = tab->used; c && (i < tab->size); i++)
    {
        if (tab->buck[i])
        {
            hashbuc_t *buck = tab->buck[i];

            d = 0;

            do
            {
                printf("0x%04X-%02X -> 0x%04X ('%s')\n", i, d, buck->data, buck->name);

                buck = buck->next;
                ++d;
            } while (buck);

            --c;
        }
    }

    return;
}

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Hash Table -------------------- End -------------------+

#pragma endregion

/* =------------------------------------------------------------= */
