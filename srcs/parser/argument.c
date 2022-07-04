#include "parser.h"

char *
parser_argument_last (PARSER_INFO * info)
{
	if (info->argument_size >= 1)
		return info->argument[info->argument_size - 1];
	return NULL;
}
