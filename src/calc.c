#include "calc-parse.h"

int hash(char *const id)
{
	char *str = id;

	if (!str || (*str <= 0x20))
		return HASH_INV; // intentionally returns an unmanageable value

	hash_t hash;

	for (hash = HASH_MIN; *str > 0x20; str++)
		hash += *str - 0x21;

	return hash;
}

int main()
{
    

    return 0;
}