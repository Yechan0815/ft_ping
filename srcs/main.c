#include <stdio.h>
#include "parser.h"

int
main (int argc, char * argv[])
{
	PARSER_INFO parserinfo;

	memset (&parserinfo, 0x0, sizeof (PARSER_INFO));
	parser_add(&parserinfo.env, 'f', "bool", "description 1");
	parser_add(&parserinfo.env, 't', "int", "description 2");

	if (!parser(&parserinfo, argc, argv))
		return 0;

	//if (parser_option(parserinfo, "m"))
	{
	}

	parser_free (&parserinfo);
	
	return 0;
}
