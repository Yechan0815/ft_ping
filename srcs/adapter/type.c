#include "convert.h"

bool
is_argu_hex (char * str)
{
	while (*str)
	{
		if (!isxdigit (*str))
		{
			return false;
		}
		++str;
	}
	return true;
}

int
hex_to_int (char h)
{
	char * shex = "0123456789abcdef";
	char * bhex = "0123456789ABCDEF";

	for (int i = 0; i < 16; i++)
	{
		if (h == shex[i] || h == bhex[i])
		{
			return i;
		}
	}

	return -1;
}
