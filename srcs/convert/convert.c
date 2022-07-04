#include "convert.h"

static void
init (ping_t * ping)
{
	struct iphdr * ipheader;
	struct icmphdr * icmpheader;

	memset (ping, 0, sizeof (ping_t));

	ping->address.sin_port = 0;
	ping->address.sin_family = AF_INET;

	ping->sender.header.ip = (struct iphdr *) ping->sender.buffer;
	ping->sender.header.icmp = (struct icmphdr *) (ping->sender.buffer + IP_HEADER_SIZE);
	ping->receiver.header.ip = (struct iphdr *) ping->receiver.buffer;
	ping->receiver.header.icmp = (struct icmphdr *) (ping->receiver.buffer + IP_HEADER_SIZE);
	
	ipheader = ping->sender.header.ip;
	ipheader->version = 4;
	ipheader->ihl = IP_HEADER_SIZE / 4;
	ipheader->tos = 0;
	ipheader->tot_len = htons (PACKET_SIZE);
	ipheader->id = htons (0);
	ipheader->frag_off = htons (0);
	ipheader->ttl = 64;
	ipheader->protocol = IPPROTO_ICMP;
	ipheader->check = 0;
	ipheader->saddr = INADDR_ANY;

	icmpheader = ping->sender.header.icmp;
	icmpheader->type = ICMP_ECHO;
	icmpheader->code = 0;
	icmpheader->checksum = 0;
	icmpheader->un.echo.id = htons (42);
	
	ping->flag.verbose = false;
	ping->flag.nodns = false;
	ping->flag.sndbuf = -1;
	ping->flag.deadline = -1;
}

static bool
destination (ping_t * ping, char * dest)
{
	struct addrinfo hints = {
		.ai_family = AF_INET
	};
	struct addrinfo *result;

	if (getaddrinfo (dest, NULL, &hints, &result))
	{
		printf ("ft_ping: %s: Name or service not known\n", dest);
		return false;
	}
	ping->domain = dest;
	ping->address.sin_addr.s_addr = ((struct sockaddr_in *) result->ai_addr)->sin_addr.s_addr;
	ping->sender.header.ip->daddr = ping->address.sin_addr.s_addr;
	freeaddrinfo (result);

	return true;
}

bool
convert (ping_t * ping, PARSER_INFO * parserinfo)
{
	init (ping);

	if (parser_option_bool (parserinfo, 'v'))
	{
		ping->flag.verbose = true;
	}
	if (parser_option_bool (parserinfo, 'n'))
	{
		ping->flag.nodns = true;
	}
	if (parser_option_bool (parserinfo, 'S'))
	{
		if (parser_option_int(parserinfo, 'S') < 1)
		{
			parser_error_msg (parserinfo, "out of range: 1 <= value <= 2147483647");
			return false;
		}
		ping->flag.sndbuf = parser_option_int(parserinfo, 'S');
	}
	if (parser_option_bool (parserinfo, 'w'))
	{
		if (parser_option_int(parserinfo, 'w') < 0)
		{
			parser_error_msg (parserinfo, "out of range: 0 <= value <= 2147483647");
			return false;
		}
		ping->flag.deadline = parser_option_int(parserinfo, 'w');
	}

	if (parser_option_bool (parserinfo, 'I'))
		printf("-I: %s\n", parser_option_string(parserinfo, 'I'));

	if (parser_option_bool (parserinfo, 'p'))
		printf("-p: %s\n", parser_option_string(parserinfo, 'p'));

	if (parser_option_bool (parserinfo, 't'))
	{
		if (parser_option_int(parserinfo, 't') < 1 || parser_option_int(parserinfo, 't') > 255)
		{
			parser_error_msg (parserinfo, "out of range: 1 <= value <= 255");
			return false;
		}
		ping->sender.header.ip->ttl = parser_option_int(parserinfo, 't');
	}

	if (!destination (ping, parser_argument_last (parserinfo)))
	{
		return false;
	}

	return true;
}
