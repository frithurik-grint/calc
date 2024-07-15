#include "calc.h"

int main()
{
    symb_t *i32 = create_symb_dtype("i32", sizeof(int), sizeof(int));
    symb_t *i64 = create_symb_dtype("i64", sizeof(long long), sizeof(long long));

    char *names[] = { "a", "b" };
    symb_t *types[] = { i32, i64 };

    symbdata_t *membv = create_symbdata_array(2, names, types);

    symb_t *point = create_symb_stype_autosiz("point", 2, membv);

    symb_t *A = malloc_symb_local("A", point);

    unsigned int size = sizeof_symb(A);

    return 0;
}