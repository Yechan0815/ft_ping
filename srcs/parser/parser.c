#include "parser.h"

static PARSER_ERROR
parser_handle_argument (PARSER_INFO * info, PARSER_STATE * state, char * str)
{
	PARSER_PARAMETER * prm;
	PARSER_ERROR error = NO_ERROR;
	
	if (*str == '-')
	{
		info->error_flag = *(str + 1);
		if (state->value_field)
		{
			error = FIELD_MISSING;
			goto end;
		}
		prm = parser_get_parameter (&info->env, *(str + 1));
		if (prm)
		{
			state->value_field = parser_value_field (prm->type);
			error = parser_new_parameter (info, *(str + 1), prm->type);
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
		error = parser_parameter_assignment (info, str);
		state->value_field = false;
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
	for (int i = 0; i < argc - 1; i++)
	{
		error = parser_handle_argument (info, &state, argv[i + 1]);
		if (error != NO_ERROR)
		{
			goto end;
		}
	}
	if (state.value_field)
	{
		error = FIELD_MISSING;
		goto end;
	}
	if (info->env.require_size > info->argument_size)
	{
		error = ARGUMENT_MISSING;
	}

end:
	if (error != NO_ERROR)
	{
		parser_error (info, error);
		parser_free (info);
		return false;
	}
	return true;
}
