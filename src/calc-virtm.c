#include "calc-virtm.h"

/* =---- Environment -------------------------------------------= */

#pragma region Environment

// +---- Symbol Table

#pragma region Symbol Table

static inline bool_t _is_type_kind(const symbkind_t kind)
{
	return (kind == SYMB_DTYPE)
		|| (kind == SYMB_STYPE);
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

	case SYMB_STYPE:
		symb->addr.structure = alloc(symb_stype_t);
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

byte_t *malloc_symb(const symb_t *const type)
{
	return mallocaz_s(sizeof_symb(type), alignof_symb(type));
}

byte_t *packlc_symb(const symb_t *const type)
{
	return mallocz_s(sizeof_symb(type));
}

unsigned int sizeof_symb(const symb_t *const symb)
{
	switch (symb->kind)
	{
	case SYMB_DTYPE:
		return symb->addr.datatype->width;

	case SYMB_STYPE:
		return symb->addr.structure->width;

	case SYMB_CONST:
		return sizeof_symb(symb->addr.constant->type);

	case SYMB_LOCAL:
		return sizeof_symb(symb->addr.variable->type);

	/*
	case SYMB_FUNCT:
		return sizeof_symb(symb->addr.function->type);

	case SYMB_PARAM:
		return sizeof_symb(symb->addr.parameter->type);*/

	default:
		return 0;
	}
}

unsigned int alignof_symb(const symb_t *const symb)
{
	switch (symb->kind)
	{
	case SYMB_DTYPE:
		return symb->addr.datatype->align;

	case SYMB_STYPE:
		return symb->addr.structure->align;

	case SYMB_CONST:
		return alignof_symb(symb->addr.constant->type);

	case SYMB_LOCAL:
		return alignof_symb(symb->addr.variable->type);

		/*
		case SYMB_FUNCT:
			return alignof_symb(symb->addr.function->type);

		case SYMB_PARAM:
			return alignof_symb(symb->addr.parameter->type);*/

	default:
		return 0;
	}
}

symbdata_t *create_symbdata(const char *const name, const symb_t *const type)
{
	symbdata_t *data = alloc(symbdata_t);

	data->name = (char*)name;
	data->type = type;

	return data;
}

symbdata_t *create_symbdata_array(unsigned int count, const char *const *const names, const symb_t *const *const types)
{
	symbdata_t *data;

	if (count > 0)
	{
		unsigned int i;

		data = dim(symbdata_t, count);

		for (i = 0; i < count; i++)
		{
			data[i].name = names[i];
			data[i].type = types[i];
		}
	}
	else
	{
		data = NULL;
	}

	return data;
}

symbdata_t *create_symbdata_va(unsigned int count, ...)
{
	symbdata_t *data;

	if (count > 0)
	{
		unsigned int i;
		va_list args;

		data = dim(symbdata_t, count);

		va_start(args, count);

		for (i = 0; i < count; i++)
		{
			data[i].name = va_arg(args, char *);
			data[i].type = va_arg(args, symb_t *);
		}

		va_end(args);
	}
	else
	{
		data = NULL;
	}

	return data;
}

unsigned int sizeof_symbdata(const symbdata_t *const data)
{
	return sizeof_symb(data->type);
}

unsigned int sizeof_symbdata_array(unsigned int count, const symbdata_t *const data)
{
	unsigned int i, s;

	for (i = 0, s = 0; i < count; i++)
		s += sizeof_symb(data[i].type);

	return s;
}

unsigned int alignof_symbdata(const symbdata_t *const data)
{
	return alignof_symb(data->type);
}

unsigned int alignof_symbdata_array(unsigned int count, const symbdata_t *const data)
{
	if (count > 0)
	{
		unsigned int i, m, t;

		m = alignof_symb(data->type);

		for (i = 1; i < count; i++)
		{
			t = alignof_symb(data[i].type);

			if (m < t)
				m = t;
			else
				continue;
		}

		return m;
	}
	else
	{
		return 0;
	}
}

// Specialized Symbols

symb_t *create_symb_dtype(const char *const name, unsigned int width, unsigned int align)
{
	symb_t *symb = create_symb(name, SYMB_DTYPE);

	symb->addr.datatype->width = width;
	symb->addr.datatype->align = align;

	return symb;
}

symb_t *create_symb_stype(const char *const name, unsigned int width, unsigned int align, unsigned int membc, const symbdata_t *const membv)
{
	symb_t *symb = create_symb(name, SYMB_STYPE);

	symb->addr.structure->width = width;
	symb->addr.structure->align = align;
	symb->addr.structure->membc = membc;
	symb->addr.structure->membv = membv;

	return symb;
}

symb_t *create_symb_stype_autosiz(const char *const name, unsigned int membc, const symbdata_t *const membv)
{
	symb_t *symb = create_symb(name, SYMB_STYPE);

	symb->addr.structure->width = sizeof_symbdata_array(membc, membv);
	symb->addr.structure->align = alignof_symbdata_array(membc, membv);
	symb->addr.structure->membc = membc;
	symb->addr.structure->membv = membv;

	return symb;
}

symb_t *create_symb_const(const char *const name, const symb_t *const type, const byte_t *const data)
{
	symb_t *symb = create_symb(name, SYMB_CONST);

	symb->addr.constant->type = type;
	symb->addr.constant->data = data;

	return symb;
}

symb_t *create_symb_local(const char *const name, const symb_t *const type)
{
	symb_t *symb = create_symb(name, SYMB_LOCAL);

	symb->addr.variable->type = type;
	symb->addr.variable->data = NULL;

	return symb;
}

symb_t *malloc_symb_local(const char *const name, const symb_t *const type)
{
	symb_t *symb = create_symb(name, SYMB_LOCAL);

	symb->addr.variable->type = type;
	symb->addr.variable->data = malloc_symb(type);

	return symb;
}

symb_t *packlc_symb_local(const char *const name, const symb_t *const type)
{
	symb_t *symb = create_symb(name, SYMB_LOCAL);

	symb->addr.variable->type = type;
	symb->addr.variable->data = packlc_symb(type);

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