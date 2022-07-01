#include "parser.h"

PARSER_ERROR
parser_argument (PARSER_INFO * info, char * str)
{
	info->argument[info->argument_size] = strdup (str);
	if (!info->argument[info->argument_size])
	{
		return ALLOC_FAIL;
	}
	++info->argument_size;

	return NO_ERROR;
}

PARSER_ERROR
parser_new_parameter (PARSER_INFO * info, PARSER_PRM type)
{
	unsigned int size;
	PARSER_ERROR error = NO_ERROR;

	switch (type)
	{
		case PS_PRM_BOOL:
			size = sizeof (PARSER_PARAMETER_BOOL);
			break;

		case PS_PRM_INT:
			size = sizeof (PARSER_PARAMETER_INT);
			break;

		case PS_PRM_UNKNOWN:
			size = sizeof (PARSER_PARAMETER_VALUE);
			break;
	}

	info->parse[info->parse_size] = (PARSER_PARAMETER_VALUE *) malloc (size);
	if (!info->parse[info->parse_size])
	{
		error = ALLOC_FAIL;
	}
	++info->parse_size;

	return error;
}

void
parser_parameter_prepare (PARSER_INFO * info, PARSER_PRM type)
{
	PARSER_PARAMETER_VALUE * prm;

	prm = info->parse[info->parse_size - 1];
	switch (type)
	{
		case PS_PRM_BOOL:
			((PARSER_PARAMETER_BOOL *) prm)->value = true;
			break;

		case PS_PRM_INT:
			((PARSER_PARAMETER_INT *) prm)->value = 0;

		case PS_PRM_UNKNOWN:
			prm->value = NULL;
			break;
	}
}

PARSER_ERROR 
parser_parameter_assignment (PARSER_INFO * info, char * str)
{
	PARSER_PARAMETER_VALUE * prm = info->parse[info->parse_size - 1];
	PARSER_ERROR error = NO_ERROR;

	switch (prm->type)
	{
		case PS_PRM_INT:
			if (!parser_is_digit (str))
			{
				error = BAD_FIELD;
				break;
			}
			((PARSER_PARAMETER_INT *) prm)->value = atoi (str);
			break;

		case PS_PRM_BOOL:
		case PS_PRM_UNKNOWN:
			break;
	}

end:
	return error;
}

