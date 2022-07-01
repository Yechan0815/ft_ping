#include "parser.h"

PARSER_PRM
string_to_parser_type (char * type)
{
	if (!strcmp (type, "bool"))
		return PS_PRM_BOOL;
	else if (!strcmp (type, "bool"))
		return PS_PRM_INT;

	return PS_PRM_UNKNOWN;
}

PARSER_PARAMETER *
parser_get_parameter (PARSER_ENV * env, char flag)
{
	for (int i = 0; i < 
}

void
parser_add (PARSER_ENV * env, char flag, char * type, char * description)
{
	static unsigned int size = 0;
	PARSER_PARAMETER * prm;

	prm = (PARSER_PARAMETER *) malloc (sizeof(PARSER_PARAMETER));
	prm->flag = flag;
	prm->type = string_to_parser_type (type);
	prm->description = strdup(description);
	env->parameters[size] = prm;

	env->size = ++size;
}
