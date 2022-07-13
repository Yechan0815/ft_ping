#include "ping.h"

static unsigned short 
checksum (unsigned short * pointer, int size)
{
	unsigned short result;
    long sum;

	sum = 0;
    while (size > 1)
    {
        sum += *(pointer++);
		size -= 2;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += sum >> 16;
    
	result = sum;
    return ~result;
}

void
network_send (ping_t * ping)
{
	char buffer[258];
	int bytes;

	++ping->sender.sequence;
	memcpy (buffer, ping->sender.buffer, 258);
	ping->sender.header.icmp->un.echo.sequence = htons (ping->sender.sequence);
	ping->sender.header.icmp->checksum = checksum (
			(unsigned short *) (ping->sender.buffer + IP_HEADER_SIZE),
			PACKET_SIZE - IP_HEADER_SIZE
			);
	if (ping->flag.verbose)
	{
		printf (" - send - \n");
		network_show_packet (&ping->sender);
	}
	gettimeofday(&ping->sender.time, NULL);
	bytes = sendto (ping->sockfd,
			ping->sender.buffer,
			PACKET_SIZE,
			0,
			(struct sockaddr *) &ping->address,
			sizeof (struct sockaddr));
	if (bytes < 0)
	{
		printf ("ft_ping: %s\n", strerror (errno));
		fatal (ping);
	}	
	ping->sender.bytes = bytes - IP_HEADER_SIZE;
	ping->sent += ping->sender.bytes;
	memcpy (ping->sender.buffer, buffer, 258);
}

void
network_receive (ping_t * ping)
{
	char buffer[258];
	int bytes;

	struct iovec iov = {
		.iov_base = ping->receiver.buffer,
		.iov_len = sizeof (ping->receiver.buffer)
	};
	struct msghdr msg = {
		.msg_name = &ping->address,
		.msg_namelen = sizeof (struct sockaddr_in),
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_control = buffer,
		.msg_controllen = sizeof (buffer),
		.msg_flags = 0
	};

	bytes = recvmsg (ping->sockfd, &msg, 0);
	if (bytes < 0)
	{
		printf ("ft_ping: %s\n", strerror (errno));
		fatal (ping);
	}
	gettimeofday(&ping->receiver.time, NULL);
	if (ping->flag.verbose)
	{
		printf (" - receive - \n");
		network_show_packet (&ping->receiver);
	}
	ping->receiver.bytes = bytes - IP_HEADER_SIZE;
	++ping->receiver.sequence;
}

bool
network_init (ping_t * ping)
{
	ping->sockfd = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping->sockfd < 0)
	{
		printf ("ft_ping: %s\n", strerror (errno));
		return false;
	}
	if (setsockopt (ping->sockfd, IPPROTO_IP, IP_HDRINCL, (int [1]) { 1 }, sizeof (int)) < 0)
	{
		printf ("ft_ping: %s\n", strerror (errno));
		return false;
	}
	if (ping->flag.sndbuf != -1)
	{
		if (setsockopt (ping->sockfd, SOL_SOCKET, SO_SNDBUF, (int [1]) { ping->flag.sndbuf }, sizeof (int)) < 0)
		{
			printf ("ft_ping: %s\n", strerror (errno));
			return false;
		}
	}
	return true;
}
