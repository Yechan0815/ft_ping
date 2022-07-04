#include "parser.h"

static PARSER_PARAMETER_VALUE *
parser_get_parameter (PARSER_INFO * info, char flag)
{
	for (unsigned int i = 0; i < info->parse_size; i++)
	{
		if (info->parse[i]->flag == flag)
		{
			return info->parse[i];
		}
	}
	return NULL;
}

bool
parser_option_bool (PARSER_INFO * info, char flag)
{
	PARSER_PARAMETER_VALUE * prm;

	prm = parser_get_parameter (info, flag);
	if (!prm)
	{
		return false;
	}
	return true;
}

int
parser_option_int (PARSER_INFO * info, char flag)
{
	PARSER_PARAMETER_VALUE * prm;

	prm = parser_get_parameter (info, flag);
	if (!prm)
	{
		return 0;
	}
	return ((PARSER_PARAMETER_INT *) prm)->value;
}

char *
parser_option_string (PARSER_INFO * info, char flag)
{
	PARSER_PARAMETER_VALUE * prm;

	prm = parser_get_parameter (info, flag);
	if (!prm)
	{
		return NULL;
	}
	return ((PARSER_PARAMETER_STRING *) prm)->value;
}
