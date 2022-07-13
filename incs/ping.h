#ifndef _PING_
# define _PING_

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <net/if.h>
# include <netdb.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>

# define IP_HEADER_SIZE 20
# define ICMP_HEADER_SIZE 8
# define PAYLOAD_SIZE 56
# define PACKET_SIZE (IP_HEADER_SIZE + ICMP_HEADER_SIZE + PAYLOAD_SIZE)

typedef struct packet packet_t;
struct packet
{
	unsigned int sequence;
	unsigned int bytes;
	struct timeval time;

	struct
	{
		struct iphdr * ip;
		struct icmphdr * icmp;
	} header;
	char buffer[258];
};

typedef struct timelog timelog_t;
struct timelog
{
	long time;
	timelog_t * next;
};

typedef struct ping ping_t;
struct ping
{
	int sockfd;
	char * domain;
	char * destination;
	struct sockaddr_in address;

	packet_t sender;
	packet_t receiver;

	struct
	{
		bool verbose;
		bool nodns;
		int sndbuf;
		int preload;
		int deadline;
		int timeout;
		char * interface;
	} flag;

	unsigned int sent;
	unsigned int received;
	unsigned int error;

	timelog_t * timelog;
	struct timeval start;
};

/* function */

bool network_init (ping_t * ping);
void network_send (ping_t * ping);
void network_receive (ping_t * ping);
void network_analysis (ping_t * ping);
void network_statistic (ping_t * ping);
void network_statistics (ping_t * ping);

void network_show_packet (struct packet * obj);

void network_error_echo (ping_t * ping);
void network_error_time_exceeded (ping_t * ping);
void network_error_default (ping_t * ping);

void ping_free (ping_t * ping);

void fatal (ping_t * ping);

#endif
