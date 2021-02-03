#include "string.h"

int
strlen(const char *str)
{
	char *s;

	s = str;
	while (*s != '\0')
		s++;

	return s - str;
}

