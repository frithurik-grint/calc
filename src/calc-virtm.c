#include "calc-virtm.h"

#ifdef __cplusplus
namespace calc::vm
{
#endif

/* =---- Internal Memory Management ----------------------------= */

#pragma region Internal Memory Management

// +---- Internal

#pragma region Internal

static inline void *_safety_abort(
#ifdef CALC_DEBUG
	const char *const funcname
#endif // CALC_DEBUG
)
{
#ifdef CALC_DEBUG
	printf("error: function %s cannot allocate memory\n", funcname);
	exit(3);
#else
	raise(SIGSEGV);
	abort();
#endif // CALC_DEBUG

	return NULL;
}

static inline void *_safety_check(
#ifdef CALC_DEBUG
	const char *const funcname,
#endif // CALC_DEBUG
	void *block
)
{
	if (!block)
		return _safety_abort(
#ifdef CALC_DEBUG
			funcname
#endif // CALC_DEBUG
		);
	else
		return block;
}

static inline void *_zerofy_block(void *block, size_t size)
{
	assert(block != NULL);

	char *p = (char *)block;

	for (; size > 0; size--)
		p[size - 1] = 0x00;

	return (void *)p;
}

#pragma endregion

// +---- Internal -- End

void *malloc_s(size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	);
}

void *calloc_s(size_t count, size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	);
}

#ifndef _get_aligned_size
#	define _get_aligned_size(size, alignment) ((size + alignment) & ~(alignment - 1))
#endif // _get_aligned_size

void *malloca_s(size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(_get_aligned_size(size, alignment))
	);
}

void *calloca_s(size_t count, size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, _get_aligned_size(size, alignment))
	);
}

#ifdef _get_aligned_size
#	undef _get_aligned_size
#endif // undef _get_aligned_size

void *mallocz_s(size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	), size);
}

void *callocz_s(size_t count, size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	), count * size);
}

#pragma endregion

/* =---- Environment -------------------------------------------= */

#pragma region Environment

// +---- Symbol Table

#pragma region Symbol Table

static inline bool_t _is_type_kind(const symbkind_t kind)
{
	return kind == SYMB_DTYPE;
}

// Generic Symbols

bool_t is_type_kind(const symbkind_t kind)
{
	return _is_type_kind(kind);
}

symb_t *create_symb(const char *const name, const symbkind_t kind)
{
	symb_t *symb = alloc(symb_t);

	symb->name = (char *)name;
	symb->kind = kind;
	symb->refs = 0;

	switch (kind)
	{
	case SYMB_DTYPE:
		symb->addr.datatype = alloc(symb_dtype_t);
		break;

	case SYMB_CONST:
		symb->addr.constant = alloc(symb_const_t);
		break;

	case SYMB_LOCAL:
		symb->addr.variable = alloc(symb_local_t);
		break;

	case SYMB_FUNCT:
		symb->addr.function = alloc(symb_funct_t);
		break;

	case SYMB_PARAM:
		symb->addr.parameter = alloc(symb_param_t);
		break;
		
	default:
		symb->addr.datatype = NULL;
		break;
	}

	return symb;
}

// Specialized Symbols

symb_t *create_symb_dtype(const char *const name, unsigned int width, unsigned int align)
{
	symb_t *symb = create_symb(name, SYMB_DTYPE);

	symb->addr.datatype->width = width;
	symb->addr.datatype->align = align;

	return symb;
}

symb_t *create_symb_const(const char *const name, const symb_t *const type)
{
	symb_t *symb = create_symb(name, SYMB_CONST);

	symb->addr.constant->type = type;
	symb->addr.constant->data = NULL;

	return symb;
}

#pragma endregion

// +---- Call Stack

#pragma region Call Stack

#pragma endregion

// +---- Evaluation Stack

#pragma region Evaluation Stack

#pragma endregion

// +---- Environment

#pragma endregion

/* =---- Bytecode ----------------------------------------------= */

#pragma region Bytecode

// +---- Source Stream

#pragma region Source Stream

#pragma endregion

#pragma endregion

/* =---- Hybrid Virtual Machine --------------------------------= */

#pragma region Hybrid Virtual Machine

// +---- Register Record

#pragma region Register Record

#pragma endregion

// +---- Context

#pragma region Context

#pragma endregion

// +---- Control Unit

#pragma region Control Unit

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

#ifdef __cplusplus
}
#endif