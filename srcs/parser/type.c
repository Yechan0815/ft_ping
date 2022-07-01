#include "parser.h"

bool
parser_is_digit (char * str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isdigit (str[i]))
		{
			return false;
		}
	}
	return true;
}

bool 
parser_value_field (PARSER_PRM type)
{
	bool boolean = false;

	switch (type)
	{
		case PS_PRM_INT:
			boolean = true;
			break;

		case PS_PRM_BOOL:
		case PS_PRM_UNKNOWN:
			break;
	}

	return boolean;
}
