#include "ping.h"

void
ping_free (ping_t * ping)
{
	timelog_t * node;
	timelog_t * next_node;

	if (ping->domain)
		free (ping->domain);
	if (ping->destination)
		free (ping->destination);

	node = ping->timelog;
	while (node)
	{
		next_node = node->next;
		free (node);
		node = next_node;
	}
}

void
fatal (ping_t * ping)
{
	if (ping->sockfd > 0)
		close (ping->sockfd);
	ping_free (ping);
	exit (EXIT_SUCCESS);
}
