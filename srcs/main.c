#include "ping.h"
#include "parser.h"
#include "convert.h"

ping_t ping;

void
sigint_handler (int signo)
{
	printf ("\n");

	network_statistics (&ping);

	close (ping.sockfd);
	ping_free (&ping);

	exit (EXIT_SUCCESS);
}

void
sigalrm_handler (int signo)
{
	struct timeval now;
	long time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec - ping.start.tv_sec) + (now.tv_usec - ping.start.tv_usec) / 1000000;

	if (ping.flag.deadline != -1 && time >= ping.flag.deadline)
	{
		sigint_handler (0);
	}

	network_send (&ping);

	alarm (1);
}

void
ping_loop ()
{
	printf ("PING %s (%s) %d(%d) bytes of data.\n", ping.domain, ping.destination, PAYLOAD_SIZE, PACKET_SIZE);
	gettimeofday(&ping.start, NULL);

	network_send (&ping);
	alarm (1);

	while (42)
	{
		network_receive (&ping);
		network_analysis (&ping);
	}
}

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

	if (!parse (&parserinfo, argc, argv))
	{
		return 0;
	}
	if (!convert (&ping, &parserinfo) || !network_init (&ping))
	{
		parser_free (&parserinfo);
		return 0;
	}
	parser_free (&parserinfo);

	signal(SIGINT, sigint_handler);
	signal(SIGALRM, sigalrm_handler);

	ping_loop ();

	return 0;
}
