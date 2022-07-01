#include <stdio.h>
#include "parser.h"

int
main (int argc, char * argv[])
{
	PARSER_INFO parserinfo;

	parser_add(&parserinfo.env, 'f', "bool", "description 1");
	parser_add(&parserinfo.env, 't', "int", "description 2");

	printf("%d\n", parserinfo.env.size);

	if (!parser(&parserinfo, argc, argv))
		return 0;

	//if (parser_option(parserinfo, "m"))
	{
	}
	
	return 0;
}
