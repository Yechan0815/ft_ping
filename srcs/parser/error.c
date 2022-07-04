#include "parser.h"

static char * error_string[] = {
	"",	/* NO_ERROR			*/
	"%s: invalid option -- '%c'",				/* BAD_FLAG			*/
	"%s: invalid argument: '%s'",				/* BAD_FIELD		*/
	"%s: option requires an argument -- '%c'",	/* FIELD_MISSING	*/
	"%s: argument required",					/* ARGUMENT_MISSING	*/
	"%s: memory not available"					/* ALLOC_FAIL		*/
};

static void
parser_help (PARSER_INFO * info)
{
	printf("usage: ");
	if (info->env.name)
	{
		printf("./%-10s ", info->env.name);
	}
	for (unsigned int i = 0; i < info->env.size; i++)
	{
		printf("[-%c", info->env.parameters[i]->flag);
		if (info->env.parameters[i]->name)
		{
			printf(" %s", info->env.parameters[i]->name);
		}
		if (i % 4 == 3)
			printf("]\n%20s", " ");
		else
			printf("] ");
	}
	printf("{destination}\n\nOPTIONS:\n");
	for (unsigned int i = 0; i < info->env.size; i++)
	{
		printf("       -%c", info->env.parameters[i]->flag);
		if (info->env.parameters[i]->name)
		{
			printf(" %s", info->env.parameters[i]->name);
		}
		printf("\n           ");
		if (info->env.parameters[i]->description)
		{
			printf("%s", info->env.parameters[i]->description);
		}
		printf("\n\n");
	}
}

void
parser_error (PARSER_INFO * info, PARSER_ERROR error)
{
	switch (error)
	{
		case BAD_FLAG:
			if (info->error_flag == 'h')
				break;
			printf(error_string[BAD_FLAG],
					info->env.name, info->error_flag);
			break;

		case BAD_FIELD:
			printf(error_string[BAD_FIELD],
					info->env.name, info->error_value);
			break;

		case FIELD_MISSING:
			printf(error_string[FIELD_MISSING],
					info->env.name, info->parse[info->parse_size - 1]->flag);
			break;

		case ARGUMENT_MISSING:
			printf(error_string[ARGUMENT_MISSING], info->env.name);
			break;

		case ALLOC_FAIL:
			printf(error_string[ALLOC_FAIL], info->env.name);
			break;
	}
	printf("\n\n");
	parser_help (info);
}

void
parser_error_msg (PARSER_INFO * info, char * msg)
{
	printf("%s: %s\n\n", info->env.name, msg);
	parser_help (info);
}
