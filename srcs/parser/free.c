#include "parser.h"

static void
parser_free_parameter (PARSER_PARAMETER_VALUE * prm)
{
	switch (prm->type)
	{
		case PS_PRM_BOOL:
			break;

		case PS_PRM_INT:
			break;

		case PS_PRM_STRING:
			if (((PARSER_PARAMETER_STRING *) prm)->value)
			{
				free(((PARSER_PARAMETER_STRING *) prm)->value);
			}
			break;

		case PS_PRM_UNKNOWN:
			break;
	}
}

static void
parser_free_env (PARSER_ENV * env)
{
	if (env->name)
	{
		free (env->name);
	}
	for (unsigned int i = 0; i < env->size; i++)
	{
		if (env->parameters[i]->name)
			free (env->parameters[i]->name);
		if (env->parameters[i]->description)
			free (env->parameters[i]->description);
		free (env->parameters[i]);
	}
}

void
parser_free (PARSER_INFO * info)
{
	parser_free_env (&info->env);
	for (unsigned int i = 0; i < info->parse_size; i++)
	{
		parser_free_parameter (info->parse[i]);
		free (info->parse[i]);
	}
	for (unsigned int i = 0; i < info->argument_size; i++)
	{
		free (info->argument[i]);
	}
}
