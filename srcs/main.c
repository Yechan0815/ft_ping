#include "ping.h"
#include "parser.h"
#include "convert.h"

ping_t ping;

void
sigint_handler (int signo)
{
	(void) signo;

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
	long gtime;
	long time;

	(void) signo;

	gettimeofday(&now, NULL);

	/* deadline */
	time = (now.tv_sec - ping.start.tv_sec) + (now.tv_usec - ping.start.tv_usec) / 1000000;
	if (ping.flag.deadline != -1 && time >= ping.flag.deadline)
	{
		sigint_handler (0);
	}

	/* timeout */
	gtime = (now.tv_sec - ping.sender.time.tv_sec) + (now.tv_usec - ping.sender.time.tv_usec) / 1000000;
	time = (ping.sender.time.tv_sec - ping.receiver.time.tv_sec) * 1000000 + (ping.sender.time.tv_usec - ping.receiver.time.tv_usec);
	if (ping.flag.timeout != -1 && time >= 0 && gtime >= ping.flag.timeout)
	{
		/* not received */	
		printf ("request timeout for icmp_seq=%d\n", ping.sender.sequence);
		network_send (&ping);
	}

	/* send */
	time = (ping.sender.time.tv_sec - ping.receiver.time.tv_sec) * 1000000 + (ping.sender.time.tv_usec - ping.receiver.time.tv_usec);
	if (time < 0)
	{
		/* received */
		network_send (&ping);
	}

	alarm (1);
}

void
ping_preload ()
{
	if (ping.flag.preload < 0)
	{
		return ;
	}

	for (int i = 0; i < ping.flag.preload; i++)
	{
		network_send (&ping);
		network_receive (&ping);
		network_analysis (&ping);
	}
}

void
ping_loop ()
{
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
	parser_add (&parserinfo->env, 'l', "int", "preload", "send <preload> number of packages while waiting replies");
	parser_add (&parserinfo->env, 'n', "bool", NULL, "no dns name resolution");
	parser_add (&parserinfo->env, 'p', "string", "pattern", "contents of padding byte");
	parser_add (&parserinfo->env, 'S', "int", "size", "use <size> as SO_SNDBUF socket option value");
	parser_add (&parserinfo->env, 't', "int", "TTL", "define time to live");
	parser_add (&parserinfo->env, 'w', "int", "deadline", "reply wait <deadline> in seconds");
	parser_add (&parserinfo->env, 'W', "int", "timeout", "time to wait for response");

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

	printf ("PING %s (%s) %d(%d) bytes of data.\n", ping.domain, ping.destination, PAYLOAD_SIZE, PACKET_SIZE);
	gettimeofday(&ping.start, NULL);
	gettimeofday(&ping.receiver.time, NULL);

	ping_preload ();
	ping_loop ();

	return 0;
}
