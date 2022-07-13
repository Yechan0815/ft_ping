#include "ping.h"

static const char * header_form = \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"| Ver  %d | IHL %2d |     TOS %2d      |        Total Length  %6d       |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|      Identification  %6d       |        Fragment Offset %3d        |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|     TTL %3d     |   Protocol %3d  |          Checksum  0x%04x         |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|                 Source Address          %-15s               |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|                 Destination Address     %-15s               |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|    Type  %3d    |    Code  %3d    |         Checksum   0x%04x         |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n" \
"|        Identifier   %6d        |         Sequence   %6d         |\n" \
"+--------+--------+--------+--------+--------+--------+--------+--------+\n";

void
network_show_packet (struct packet * obj)
{
	char saddr[128];
	char daddr[128];

	inet_ntop (AF_INET, (void *) &obj->header.ip->saddr, saddr, sizeof (saddr));
	inet_ntop (AF_INET, (void *) &obj->header.ip->daddr, daddr, sizeof (daddr));

	printf (header_form, obj->header.ip->version, obj->header.ip->ihl,
		obj->header.ip->tos, ntohs (obj->header.ip->tot_len), ntohs (obj->header.ip->id),
		ntohs (obj->header.ip->frag_off), obj->header.ip->ttl, obj->header.ip->protocol,
		obj->header.ip->check, saddr, daddr, obj->header.icmp->type,
		obj->header.icmp->code, obj->header.icmp->checksum, ntohs (obj->header.icmp->un.echo.id),
		ntohs (obj->header.icmp->un.echo.sequence));
	printf ("IP Header %d byte\nICMP Header %d byte\nPayload %d byte\n\n",
			IP_HEADER_SIZE, ICMP_HEADER_SIZE, ntohs (obj->header.ip->tot_len) - IP_HEADER_SIZE - ICMP_HEADER_SIZE);
}

void
network_analysis (ping_t * ping)
{
	if (ping->receiver.bytes < ICMP_HEADER_SIZE)
	{
		printf ("ft_ping: Could not find icmp header\n");
		fatal (ping);
	}

	switch (ping->receiver.header.icmp->type)
	{
		case ICMP_ECHOREPLY:
			network_statistic (ping);
			break;

		case ICMP_ECHO:
			network_error_echo (ping);
			break;

		case ICMP_TIME_EXCEEDED:
			network_error_time_exceeded (ping);
			break;

		default:
			network_error_default (ping);
			break;
	}
}
