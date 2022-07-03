#include <stdio.h>
#include "parser.h"

int
main (int argc, char * argv[])
{
	PARSER_INFO parserinfo;

	memset (&parserinfo, 0x0, sizeof (PARSER_INFO));
	parser_set(&parserinfo.env, "ft_ping", 1);
	parser_add(&parserinfo.env, 'f', "bool", NULL, "description 1");
	parser_add(&parserinfo.env, 't', "int", "TTL", "description 2");
	parser_add(&parserinfo.env, 'Q', "int", "tos", "description 3");
	parser_add(&parserinfo.env, 'T', "int", "timestamp option", "description 4");
	parser_add(&parserinfo.env, 'W', "int", "timeout", "description 5");
	parser_add(&parserinfo.env, 'P', "bool", NULL, NULL);

	if (!parser(&parserinfo, argc, argv))
		return 0;

	if (parser_option_bool(&parserinfo, 'f'))
		printf("-f: true\n");

	if (parser_option_bool(&parserinfo, 't'))
		printf("-t: %d\n", parser_option_int(&parserinfo, 't'));

	parser_free (&parserinfo);
	
	return 0;
}
