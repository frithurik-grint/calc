#include "calc.h"

int main()
{
    symb_t *i32 = create_symb_dtype("i32", sizeof(int), sizeof(int));
    symb_t *i64 = create_symb_dtype("i64", sizeof(long long), sizeof(long long));

    symbdata_t *membv = create_symbdata_v(2, "x", i32, "y", i32);

    symb_t *point = create_symb_stype("point", 8, 4, 2, membv);

    byte_t *A = malloc_symb(point);

    return 0;
}