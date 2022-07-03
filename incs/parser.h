#ifndef _PARSER_
# define _PARSER_

# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

# define PARSER_MAX_FLAG 42
# define PARSER_MAX_ARGUMENT 42

typedef enum
{
	NO_ERROR,
	BAD_FLAG,
	BAD_FIELD,
	FIELD_MISSING,

	ARGUMENT_MISSING,

	ALLOC_FAIL
} PARSER_ERROR;

typedef enum
{
	PS_PRM_BOOL,
	PS_PRM_INT,

	PS_PRM_UNKNOWN
} PARSER_PRM;

typedef struct
{
	char flag;
	PARSER_PRM type;
	char * name;
	char * description;
} PARSER_PARAMETER;

typedef struct
{
	char flag;
	PARSER_PRM type;
	char * value;
} PARSER_PARAMETER_VALUE;

typedef struct
{
	char flag;
	PARSER_PRM type;
	bool value;
} PARSER_PARAMETER_BOOL;

typedef struct
{
	char flag;
	PARSER_PRM type;
	int value;
} PARSER_PARAMETER_INT;

typedef struct
{
	bool value_field;
} PARSER_STATE;

typedef struct
{
	char * name;
	unsigned int require_size;
	PARSER_PARAMETER * parameters[PARSER_MAX_FLAG];
	unsigned int size;
} PARSER_ENV;

typedef struct
{
	char error_flag;
	char * error_value;
	PARSER_ENV env;
	PARSER_PARAMETER_VALUE * parse[PARSER_MAX_FLAG];
	unsigned int parse_size;
	char * argument[PARSER_MAX_ARGUMENT];
	unsigned int argument_size;
} PARSER_INFO;

/* error */
void parser_error (PARSER_INFO * info, PARSER_ERROR error);

/* type */
bool parser_is_digit (char * str);
bool parser_value_field (PARSER_PRM type);

/* parameter */
PARSER_ERROR parser_new_parameter (PARSER_INFO * info, char flag, PARSER_PRM type);
void parser_parameter_prepare (PARSER_INFO * info, PARSER_PRM type);
PARSER_ERROR parser_parameter_assignment (PARSER_INFO * info, char * str);
PARSER_ERROR parser_argument (PARSER_INFO * info, char * str);

/* env */
PARSER_PARAMETER * parser_get_env_parameter (PARSER_ENV * env, char flag);
PARSER_ERROR parser_add (PARSER_ENV * env, char flag, char * type, char * name, char * description);
PARSER_ERROR parser_set(PARSER_ENV * env, char * name, unsigned int require);

/* free */
void parser_free (PARSER_INFO * info);

/* parser */
bool parser (PARSER_INFO * info, int argc, char * argv[]);

/* option */
int parser_option_int (PARSER_INFO * info, char flag);
bool parser_option_bool (PARSER_INFO * info, char flag);

#endif
