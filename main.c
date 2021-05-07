#include "ft_ping.h"

static t_ping g =  {
	.loop = 1,
	.flags = 0,
	.addr = NULL,
	.seq = 0,
	.rec = 0,
	.ttl = 64,
	.family = 0,
	.four.sin_family = AF_INET,
	.four.sin_port = 0,
};

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void gettype(char *a)
{
	int i;
	int b;

	i = 0;
	if (a[0] == '\0')
		ft_help("error", 1);
	if (a[0] == '-')
	{
		if (ft_strlen(a) == 1)
			ft_help("error", 1);
		a++;
		while (*a) 
		{
			if (*a == 'v')
				g.flags = g.flags | 2;		
			else if (*a == 'h')
				g.flags = g.flags | 1;
			else
				ft_help("bad option", 1);
			a++;
		}
		return ;
	}
	if (g.addr != NULL)
		ft_help("route", 1);
	g.addr = a;
	printf("%s\n", g.addr);
}

static void get_opt(int a, char **b)
{
	int i;

	i = 1;
	while (i < a)
		gettype(b[i++]);
	if (g.flags & 1)
		ft_help("help", 0);
	if (g.addr == NULL)
		ft_help("NULL", 0);
}

void intHandler(int dummy)
{
    g.loop = 0;
}

static void start()
{
	int ttl;

	ttl = 1;
	struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;
	printf("%s\n", g.addr);
	get_info(&g);
	if (g.family == 6)
		ft_help("ipv6 not supported", 1);
	open_socket(&g);
	if (setsockopt(g.socket, IPPROTO_IP, IP_HDRINCL, (int[1]){1}, sizeof(int)) != 0)
	{
		perror("zzz");
		ft_help("ttl", 1);
	}
	signal(SIGINT, intHandler);
	signal(SIGALRM, intHandler);;
	rec_socket(&g);
}

int main(int argc, char **argv)
{
	
	if (argc == 1)
		ft_help("error", 1);
	get_opt(argc, argv);
	start();
	return (0);
}
