#include "parser.h"

static PARSER_ERROR
parser_handle_argument (PARSER_INFO * info, PARSER_STATE * state, char * str)
{
	PARSER_PARAMETER * prm;
	PARSER_ERROR error = NO_ERROR;
	
	if (*str == '-')
	{
		if (state->value_field)
		{
			error = FIELD_MISSING;
			goto end;
		}
		prm = parser_get_parameter (&info->env, *(str + 1));
		if (prm)
		{
			state->value_field = parser_value_field (prm->type);
			error = parser_new_parameter (info, prm->type);
			if (error != NO_ERROR)
			{
				goto end;
			}
			parser_parameter_prepare (info, prm->type);
		}
		else
		{
			error = BAD_FLAG;
		}
		goto end;
	}

	if (state->value_field)
	{
		parser_parameter_assignment (info, str);
		goto end;
	}

	parser_argument (info, str);

end:
	return error;
}

bool
parser (PARSER_INFO * info, int argc, char * argv[])
{
	PARSER_STATE state = {
		.value_field = false
	};
	PARSER_ERROR error = NO_ERROR;

	info->parse_size = 0;
	info->argument_size = 0;
	for (int i = 0; i < argc; i++)
	{
		error = parser_handle_argument (info, &state, argv[i]);
		if (error != NO_ERROR)
		{
			goto end;
		}
	}
	if (state.value_field)
	{
		error = FIELD_MISSING;
	}

end:
	if (error != NO_ERROR)
	{
		parser_free (info);
		/* error manage */
		return false;
	}
	return true;
}
