#pragma once

/* stdmem.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_BASE_STDMEM_H_
#define CALC_BASE_STDMEM_H_

#include "calc/base/stderr.h"

#include <assert.h>
#include <stdlib.h>

CALC_C_HEADER_BEGIN

/* =---- Standard Memory Management Header ---------------------= */

/// @brief Allocates a block of bytes on the heap.
/// @param size Number of bytes to allocate.
/// @return A pointer to the allocated block of memory.
void *_CDECL checked_malloc(size_t size);
/// @brief Allocates a series of contiguous blocks
///		   of memory on the heap.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return A pointer to the beginning of the first
///			allocated block.
void *_CDECL checked_calloc(size_t count, size_t size);

#ifndef alloc
/// @brief Allocates an instance of the specified type on
///        the heap.
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#   define alloc(type) (type *)checked_malloc(sizeof(type))
#endif // alloc

#ifndef dim
/// @brief Allocates an array of instances of the specified
///        type.
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the first instance in the array.
#   define dim(type, count) (type *)checked_calloc((count), sizeof(type))
#endif // dim

/// @brief Allocates a block of bytes on the heap and
///        sets them to zero.
/// @param size Number of bytes to allocate.
/// @return A pointer to the allocated block of memory.
void *_CDECL checked_mallocz(size_t size);
/// @brief Allocates a series of contiguous blocks
///		   of memory on the heap and sets them to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return A pointer to the beginning of the first
///			allocated block.
void *_CDECL checked_callocz(size_t count, size_t size);

#ifndef allocz
/// @brief Allocates an instance of the specified type on
///        the heap and sets its value to zero.
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#   define allocz(type) (type *)checked_mallocz(sizeof(type))
#endif // allocz

#ifndef dimz
/// @brief Allocates an array of instances of the specified
///        type and sets their values to zero.
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the first instance in the array.
#   define dimz(type, count) (type *)checked_callocz((count), sizeof(type))
#endif // dimz

// Aligned Allocators

#ifndef alignto
/// @brief Gets the aligned size.
#   define alignto(size, alignment) (((size) + (alignment) - 1) & ~((alignment) - 1))
#endif // alignto

/// @brief Allocates an aligned block of bytes on the heap.
/// @param align Alignment to follow.
/// @param size Number of bytes to allocate.
/// @return A pointer to the allocated block of memory.
_INLINE void *_CDECL checked_aligned_malloc(size_t align, size_t size)
    _Return(checked_malloc(alignto(size, align)))
/// @brief Allocates a series of contiguous aligned blocks
///		   of memory on the heap and sets them to zero.
/// @param align Alignment to follow.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return A pointer to the beginning of the first
///			allocated block.
_INLINE void *_CDECL checked_aligned_calloc(size_t align, size_t count, size_t size)
    _Return(checked_calloc(count, alignto(size, align)))

#ifndef aligned_alloc
/// @brief Allocates an aligned instance of the specified type on
///        the heap.
/// @param alignment Alignment to follow.
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#   define aligned_alloc(type, alignment) (type *)checked_aligned_malloc((alignment), sizeof(type))
#endif // aligned_alloc

#ifndef aligned_dim
/// @brief Allocates an array of aligned instances of the specified
///        type.
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the first instance in the array.
#   define aligned_dim(type, count, alignment) (type *)checked_aligned_calloc((alignment), (count), sizeof(type))
#endif // aligned_dim

/// @brief Allocates an aligned block of bytes on the heap and
///        sets them to zero.
/// @param size Number of bytes to allocate.
/// @return A pointer to the allocated block of memory.
_INLINE void *_CDECL checked_aligned_mallocz(size_t align, size_t size)
    _Return(checked_mallocz(alignto(size, align)))
/// @brief Allocates a series of contiguous aligned blocks
///		   of memory on the heap and sets them to zero.
/// @param align Alignment to follow.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return A pointer to the beginning of the first
///			allocated block.
_INLINE void *_CDECL checked_aligned_callocz(size_t align, size_t count, size_t size)
    _Return(checked_callocz(count, alignto(size, align)))

#ifndef aligned_allocz
/// @brief Allocates an aligned instance of the specified type on
///        the heap and sets its value to zero.
/// @param alignment Alignment to follow.
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#   define aligned_allocz(type, alignment) (type *)checked_aligned_mallocz((alignment), sizeof(type))
#endif // aligned_allocz

#ifndef aligned_dimz
/// @brief Allocates an array of aligned instances of the specified
///        type and sets their values to zero.
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the first instance in the array.
#   define aligned_dimz(type, count, alignment) (type *)checked_aligned_callocz((alignment), (count), sizeof(type))
#endif // aligned_dimz

// Stack Allocators

#ifndef stackalloc
#   ifdef malloca
/// @brief If its possible, allocates an instance of the specified
///        type on the stack.
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#       define stackalloc(type) malloca(sizeof(type))
#   elif defined alloca
/// @brief If its possible, allocates an instance of the specified
///        type on the stack. (can cause overflow)
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#       define stackalloc(type) alloca(sizeof(type))
#   else
/// @brief If its possible, allocates an instance of the specified
///        type on the stack.
///        
///        NOTE: Function alloca not found, currently this macro
///        allocates on the heap (using allocz).
/// 
/// @param type The type to instantiate.
/// @return A pointer to the instanced block of memory.
#       define stackalloc(type) allocz(type)
#   endif // malloca/alloca
#endif // stackalloc

#ifndef stackdim
#   ifdef malloca
/// @brief If its possible, allocates an array of instances of the
///        specified type.
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the instanced block of memory.
#       define stackdim(type, count) malloca(sizeof(type) * (count))
#   elif defined alloca
/// @brief If its possible, allocates an array of instances of the
///        specified type. (can cause overflow)
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the instanced block of memory.
#       define stackdim(type, count) alloca(sizeof(type) * (count))
#   else
/// @brief If its possible, allocates an array of instances of the
///        specified type.
///        
///        NOTE: Function alloca not found, currently this macro
///        allocates on the heap (using dimz).
/// 
/// @param type The type to instantiate.
/// @param count Number of instances to allocate.
/// @return A pointer to the instanced block of memory.
#       define stackdim(type, count) dimz(type, count)
#   endif // malloca/alloca
#endif // stackdim

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#endif // CALC_BASE_STDMEM_H_
