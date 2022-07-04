#include "parser.h"

PARSER_PRM
string_to_parser_type (char * type)
{
	if (!strcmp (type, "bool"))
		return PS_PRM_BOOL;
	else if (!strcmp (type, "int"))
		return PS_PRM_INT;
	else if (!strcmp (type, "string"))
		return PS_PRM_STRING;

	return PS_PRM_UNKNOWN;
}

PARSER_PARAMETER *
parser_get_env_parameter (PARSER_ENV * env, char flag)
{
	for (unsigned int i = 0; i < env->size; i++)
	{
		if (env->parameters[i]->flag == flag)
			return env->parameters[i];
	}
	return NULL;
}

PARSER_ERROR
parser_add (PARSER_ENV * env, char flag, char * type, char * name, char * description)
{
	PARSER_PARAMETER * prm;

	prm = (PARSER_PARAMETER *) malloc (sizeof(PARSER_PARAMETER));
	if (!prm)
	{
		return ALLOC_FAIL;
	}
	prm->flag = flag;
	prm->type = string_to_parser_type (type);
	if (name)
	{
		prm->name = strdup(name);
		if (!prm->name)
		{
			return ALLOC_FAIL;
		}
	}
	if (description)
	{
		prm->description = strdup(description);
		if (!prm->description)
		{
			return ALLOC_FAIL;
		}
	}
	env->parameters[env->size++] = prm;

	return NO_ERROR;
}

PARSER_ERROR
parser_set(PARSER_ENV * env, char * name, unsigned int require)
{
	env->name = strdup(name);
	if (!env->name)
	{
		return ALLOC_FAIL;
	}
	env->require_size = require;
	return NO_ERROR;
}
