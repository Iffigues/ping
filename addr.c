#include "ft_ping.h"

void get_info(t_ping *g)
{
	struct addrinfo			hints;
	struct addrinfo			*res;

	
	printf("%s\n", g->addr);
	hints = (struct addrinfo){.ai_family = AF_INET};
	if (getaddrinfo(g->addr, NULL, &hints, &res))
		ft_help("lol", 1);
	g->four.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(res);
}	
