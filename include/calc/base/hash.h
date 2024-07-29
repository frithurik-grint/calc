#pragma once

/* hash.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_HASH_H_
#define CALC_BASE_HASH_H_

#include "calc/base/stddef.h"
#include "calc/base/stdmem.h"
#include "calc/base/stdout.h"
#include "calc/base/stdstr.h"

/// @brief Hash code data type.
typedef unsigned int hash_t;

#ifndef HASH_MIN
/// @brief Minimum hash code value.
#   define HASH_MIN ((hash_t)0x00)
#endif // HASH_MIN

#ifndef HASH_MAX
/// @brief Maximum hash code value.
#   define HASH_MAX ((hash_t)-2)
#endif // HASH_MAX

#ifndef HASH_INV
/// @brief Erroneus hash code value.
#   define HASH_INV ((hash_t)-1)
#endif // HASH_INV

/// @brief Hashing function data type.
typedef hash_t(*hashfnc_t)(char *const);

// Hash Bucket

/// @brief Hash table bucket structure.
typedef struct _calc_hash_table_bucket
{
    /// @brief Key string.
    char *name;
    /// @brief Hash code of the bucket in
    ///        the table.
    hash_t hash;
    /// @brief Index of the datas in the
    ///        data record.
    unsigned int data;
    /// @brief Pointer to the colliding
    ///        bucket with the same hash.
    struct _calc_hash_table_bucket *next;
} hashbuc_t;

/// @brief Creates a new hash table bucket
///        with data address and colliding
///        bucket.
/// @param name Key string.
/// @param hash Computed hash code.
/// @param data Data address.
/// @param prev In case of collision this points
///             to the colliding bucket.
/// @return A pointer to the new hash table
///         bucket.
hashbuc_t *_CDECL create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev);
/// @brief Deletes the specified hash table
///        bucket, releasing its memory. (this
///        frees name pointer)
/// @param bucket Bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *_CDECL delete_hashbuc(hashbuc_t *const bucket);

// Hash Table

#ifndef CALC_HASHTAB_BUCKSNUM
/// @brief Default number of buckets in a chunk
///        of the hash table.
#   define CALC_HASHTAB_BUCKSNUM 0xFF
#endif // CALC_HASHTAB_BUCKSNUM

/// @brief Hash table data structure.
typedef struct _calc_hash_table
{
    /// @brief Array of pointers to the buckets.
    hashbuc_t **buck;
    /// @brief Hashing function for this table.
    hashfnc_t func;
    /// @brief Maximum number of buckets.
    unsigned int size;
    /// @brief Number of used buckets.
    unsigned int used;
    /// @brief Pointer to previous hash table
    ///        chunk.
    struct _calc_hash_table *prev;
} hashtab_t;

/// @brief Creates a new hash table with the
///        specified number of buckets.
/// @param size Numeber of buckets to allocate
///             in the chunk. (if size is 0 will
///             be allocated a chunk of
///             CALC_HASHTAB_BUCKSNUM buckets)
/// @param prev Pointer to the previous chunk.
/// @return A pointer to the new hash table chunk.
hashtab_t *_CDECL create_hashtab(unsigned int size, hashfnc_t func, hashtab_t *const prev);
/// @brief Deletes the specified hash table with
///        its buckets, releasing all used memory.
/// @param tab Pointer to table to delete.
/// @return A pointer to the previous hash table chunk.
hashtab_t *_CDECL delete_hashtab(hashtab_t *const tab);

/// @brief If specified key is not inserted in the hash table
///        chunk, inserts it, else gets it.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to add or get.
/// @return A pointer to the new (or old) hash bucket.
hashbuc_t *_CDECL hashtab_add(hashtab_t *const tab, char *const key);
/// @brief Gets bucket with the specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *_CDECL hashtab_get(hashtab_t *const tab, char *const key);
/// @brief Sets the data address of the bucket at the specified
///        key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @param data Data address to set to the found bucket.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *_CDECL hashtab_set(hashtab_t *const tab, char *const key, unsigned int data);

/// @brief Checks if hash table contains a specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to check.
/// @return TRUE if the hash table contains the value, else FALSE.
bool_t _CDECL hashtab_contains(hashtab_t *const tab, char *const key);

/// @brief Removes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to remove.
/// @return A pointer to the removed hash bucket.
hashbuc_t *_CDECL hashtab_remove(hashtab_t *const tab, char *const key);
/// @brief Deletes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *_CDECL hashtab_delete(hashtab_t *const tab, char *const key);

#ifndef _CALC_MINIMAL_BUILD

/// @brief Dumps hash table content on a stream.
/// @param stream Stream in which dump content of the hash
///               table.
/// @param tab Pointer to the last hash table chunk of the
///            hash table to dump on the selected stream.
void _CDECL hashtab_dump(FILE *const stream, hashtab_t *const tab);

#endif // _CALC_MINIMAL_BUILD

#endif // CALC_BASE_HASH_H_
