#include "ft_ping.h"


suseconds_t	getTime()
{
	struct timeval	times;

	if (gettimeofday(&times, NULL) == -1)
		ft_help("get time errors", 1);
	return (times.tv_sec * 1000000 + times.tv_usec);
}

suseconds_t	getRtt(struct timeval *i)
{
	suseconds_t	times;
	times = getTime();
	return (times - i->tv_sec * 1000000 - i->tv_usec);
}

