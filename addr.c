#include "ft_ping.h"

char *get_info()
{
	int             n;
	struct addrinfo hints;
	struct addrinfo *res;
	static char str[126];
	void *ptr;

	bzero (&hints, sizeof (struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	hints.ai_socktype = 0;
	if ((n = getaddrinfo(g->addr, NULL, &hints, &res)) != 0)
	    ft_help("name or service not known", 1);
	g->h = res;
	g->len = res->ai_addrlen;
	while (res) {
	if (res->ai_addr->sa_family == AF_INET)
   	{
		ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
        	if (inet_ntop(res->ai_family, ptr, str, sizeof(str)) == NULL)
        	    return (NULL);
		g->s = res->ai_addr;
        	return (str);
    	}
	res = res->ai_next;
	}
	ft_help("6",1);
	return NULL;
}
