#include "utility.hpp"

#include <cstdlib>

int divRem(int& ds, int dd)
{
	div_t q = div(ds, dd);
	ds = q.rem;
	return q.quot;
}