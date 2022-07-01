#ifndef _PARSER_
# define _PARSER_

# include <stdbool.h>
# include <string.h>
# include <stdlib.h>

# define PARSER_MAX_FLAG 42
# define PARSER_MAX_ARGUMENT 42

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
	PARSER_PARAMETER * parameters[PARSER_MAX_FLAG];
	unsigned int size;
} PARSER_ENV;

typedef struct
{
	PARSER_ENV env;
	PARSER_PARAMETER_VALUE * parse;
	char * argument[PARSER_MAX_ARGUMENT];
} PARSER_INFO;

void	parser_add (PARSER_ENV * env, char flag, char * type, char * description);

//void	parser_option (PARSER_INFO * info, char flag);

int		parser (PARSER_INFO * info, int argc, char * argv[]);

#endif
