#include "ping.h"
#include "parser.h"
#include "convert.h"

bool
parse (PARSER_INFO * parserinfo, int argc, char * argv[])
{
	memset (parserinfo, 0x0, sizeof (PARSER_INFO));
	parser_set (&parserinfo->env, "ft_ping", 1);
	parser_add (&parserinfo->env, 'v', "bool", NULL, "verbose output");
	parser_add (&parserinfo->env, 'I', "string", "interface", "either interface name or address");
	parser_add (&parserinfo->env, 'n', "bool", NULL, "no dns name resolution");
	parser_add (&parserinfo->env, 'p', "string", "pattern", "contents of padding byte");
	parser_add (&parserinfo->env, 'S', "int", "size", "use <size> as SO_SNDBUF socket option value");
	parser_add (&parserinfo->env, 't', "int", "TTL", "define time to live");
	parser_add (&parserinfo->env, 'w', "int", "deadline", "reply wait <deadline> in seconds");

	if (!parser (parserinfo, argc, argv))
		return false;
	return true;
}

int
main (int argc, char * argv[])
{
	PARSER_INFO parserinfo;
	ping_t ping;

	if (!parse (&parserinfo, argc, argv))
	{
		return 0;
	}
	if (!convert (&ping, &parserinfo) || !network_init (&ping))
	{
		parser_free (&parserinfo);
		return 0;
	}

	network_send (&ping);
	network_receive (&ping);

	parser_free (&parserinfo);
	
	return 0;
}
