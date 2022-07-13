#include "ping.h"
#include "convert.h"

static double
babylonian_sqrt (double n)
{
	double s;

	s = n / 2;
	while (!(-0.000001 <= s * s - n && s * s - n <= 0.000001))
	{
		s = (s * s + n) / (2 * s);
	}
	return s;
}

static void
calculate_statistic (ping_t * ping, long * min, long * avg, long * max, long * mdev)
{
	unsigned int count;
	timelog_t * current;

	count = 0;
	*min = *max = ping->timelog->time;
	*avg = *mdev = 0;
	current = ping->timelog;
	while (current)
	{
		if (current->time < *min)
			*min = current->time;
		if (current->time > *max)
			*max = current->time;
		*avg += current->time;
		++count;
		current = current->next;
	}
	*avg /= count;
	current = ping->timelog;
	while (current)
	{
		*mdev += (current->time - *avg) * (current->time - *avg);
		current = current->next;
	}
	*mdev = babylonian_sqrt (*mdev);
}

void
network_statistic (ping_t * ping)
{
	long time;

	ping->received += ping->receiver.bytes;

	time = (ping->receiver.time.tv_sec - ping->sender.time.tv_sec) * 1000000
	+ (ping->receiver.time.tv_usec - ping->sender.time.tv_usec);

	printf ("%d bytes from %s: icmp_seq=%d ttl=%d time=%ld.%ld ms\n",
			ping->receiver.bytes,
			ping->destination,
			ping->receiver.sequence,
			ping->receiver.header.ip->ttl,
			time / 1000,
			time / 100 % 10);

	if (!timelog_enqueue (&ping->timelog, time))
	{
		printf ("ft_ping: %s\n", strerror (errno));
		fatal (ping);
	}
}

void
network_statistics (ping_t * ping)
{
	struct timeval end;
	unsigned int packet_loss;
	long time;
	long min, avg, max, mdev;

	gettimeofday(&end, NULL);
	time = (end.tv_sec - ping->start.tv_sec) * 1000 + (end.tv_usec - ping->start.tv_usec) / 1000;
	packet_loss = 100 - (int) ((float) ping->received / ping->sent * 100);

	printf ("--- %s ping statistics ---\n", ping->domain);
	printf ("%u packets transmitted, %u received, ",
			ping->sender.sequence, ping->receiver.sequence - ping->error);
	if (ping->error > 0)
		printf ("+%u errors, ", ping->error);
	printf ("%u%% packet loss, time %ld ms\n", packet_loss, time);

	if (ping->receiver.sequence - ping->error > 0)
	{
		calculate_statistic (ping, &min, &avg, &max, &mdev);
		printf ("rtt min/avg/max/mdev = %ld.%ld/%ld.%ld/%ld.%ld/%ld.%ld ms",
				min / 1000, min % 1000,
				avg / 1000, avg % 1000,
				max / 1000, max % 1000,
				mdev / 1000, mdev % 1000
				);
	}
	printf ("\n");
}
