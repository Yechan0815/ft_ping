#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <stdbool.h>
# include <math.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>

int in_cksum(void *pp, int n )
{
	 u_short *p = (u_short *) pp;
    register u_short answer;
    register long sum = 0;
    u_short odd_byte = 0;

    while( n > 1 )
    {
        sum += *p++;
        n -= 2;
    
    }/* WHILE */


    /* mop up an odd byte, if necessary */
    if( n == 1 )
    {
        *( u_char* )( &odd_byte ) = *( u_char* )p;
        sum += odd_byte;
    
    }/* IF */

    sum = ( sum >> 16 ) + ( sum & 0xffff );    /* add hi 16 to low 16 */
    sum += ( sum >> 16 );                    /* add carry */
    answer = ~sum;                            /* ones-complement, truncate*/
    
    return ( answer );

} /* in_cksum() */
	

int		main(void)
{
	char buffer[1024];
	struct iphdr * iph;
	struct icmphdr * icmph;
	struct sockaddr_in addr;
	int sock;
	int ret;
	
	struct addrinfo			hints = {.ai_family = AF_INET};
	struct addrinfo			*res;

    memset(&addr, 0, sizeof(addr));

	addr.sin_port = 0;
    addr.sin_family = AF_INET;

	getaddrinfo("8.8.8.8", NULL, &hints, &res);
	addr.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	
//	printf(": %x\n", inet_addr("8.8.8.8"));
	printf("b: %x\n", addr.sin_addr.s_addr);

	iph = (struct iphdr *) buffer;
	iph->version = 4;
	iph->ihl = 20 / 4;
	iph->tos = 0;
	iph->tot_len = htons(84); // 20 + 8 + 56
	iph->id = htons(0);
	iph->frag_off = htons(0);
	iph->ttl = 64;
	iph->protocol = IPPROTO_ICMP;
	iph->check = 0;
	iph->saddr = INADDR_ANY;
	iph->daddr = addr.sin_addr.s_addr;

	icmph = (struct icmphdr *) (buffer + iph->ihl * 4);
	icmph->type = ICMP_ECHO;
	icmph->code = 0;
	icmph->checksum = 0;
	icmph->un.echo.id = htons(13246);
	icmph->un.echo.sequence = htons(12345);

	icmph->checksum = in_cksum(buffer + 20, 64);

	sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0)
	{
		printf("error\n");
		return 0;
	}
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (int[1]) { 1 }, sizeof(int)) < 0)
	{
		printf("error\n");
		return 0;
	}
	

    ret = sendto (sock, buffer, 84, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr));
	printf("send %d\n", ret);
	if (ret < 0)
	{
		printf("error\n");
		return 0;
	}

	char buf[512];

	struct iovec	io =
	{
		.iov_base = buffer,
		.iov_len = 84
	};
	struct msghdr	msg =
	{
		.msg_name = &addr,
		.msg_namelen = sizeof(struct sockaddr_in),
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buf,
		.msg_controllen = sizeof(buf),
		.msg_flags = 0
	};

	printf("%ld\n", sizeof(struct sockaddr_in));
	ret = recvmsg (sock, &msg, 0);
    if (ret < 0)
    {
		printf("error\n");
		return 0;
    }
	printf("recv %d\n", ret);
	
	iph = (struct iphdr *) buffer;
	icmph = (struct icmphdr *) (buffer + 20);

	printf("src: %x\n", ntohl(iph->saddr));
	printf("dst: %x\n", ntohl(iph->daddr));
	printf("ttl: %d\n", iph->ttl);

	printf("code: %d\n", icmph->code);
	printf("bytes: %d\n", (uint16_t)(ntohs(iph->tot_len) - 20));
	return 0;
}
