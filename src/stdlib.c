#include "stdlib.h"

int
atoi(const char *str)
{
	int res, pow, len;
	char *s;

	len = 0;
	while (str[len] != '\0')
		len++;

	res = 0;
	pow = 1;
	s = str + len - 1;

	/* assume that the base is always 10 */
	while (s >= str) {
		res += (*s - '0') * pow;
		pow *= 10;
		s--;
	}

	return res;
}

