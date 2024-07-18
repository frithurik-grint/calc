#include "calc-liber.h"

int main(void)
{
	symbtab_t *stab = create_symbtab(CALC_SYMBTAB_CHUNKSIZ, NULL);

	symbtab_add(stab, "ciao", 0);
	symbtab_add(stab, "Ciao", 1);
	symbtab_add(stab, "cIao", 2);
	symbtab_add(stab, "ciAo", 3);
	symbtab_add(stab, "ciaO", 4);
	symbtab_add(stab, "fede", 5);
	symbtab_add(stab, "DIOP", 6);
	symbtab_add(stab, "ciao", 7);
	symbtab_add(stab, "nope", 8);
	symbtab_add(stab, "xnny", 9);

	symbtab_print(stab);

	return 0;
}