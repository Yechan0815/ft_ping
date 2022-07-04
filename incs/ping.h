#ifndef _PING_
# define _PING_

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

# define IP_HEADER_SIZE 20
# define ICMP_HEADER_SIZE 8
# define PAYLOAD_SIZE 56
# define PACKET_SIZE (IP_HEADER_SIZE + ICMP_HEADER_SIZE + PAYLOAD_SIZE)

typedef struct packet packet_t;
struct packet
{
	unsigned int sequence;

	struct
	{
		struct iphdr * ip;
		struct icmphdr * icmp;
	} header;
	char buffer[1024];
};

typedef struct timelog timelog_t;
struct timelog
{
	unsigned int time;
	timelog_t * next;
};

typedef struct ping ping_t;
struct ping
{
	int sockfd;
	char * domain;
	struct sockaddr_in address;

	packet_t sender;
	packet_t receiver;

	timelog_t * timelog;

	struct
	{
		bool verbose;
		bool nodns;
		int sndbuf;
		int deadline;
	} flag;
};

/* function */

bool network_init (ping_t * ping);
bool network_send (ping_t * ping);
bool network_receive (ping_t * ping);

#endif
