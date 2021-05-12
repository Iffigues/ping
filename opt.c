#include "ft_ping.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void gettype(char *a, t_ping *g)
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
				g->flags = g->flags | 2;
			else if (*a == 'h')
				g->flags = g->flags | 1;
			else
				ft_help("bad option", 1);
			a++;
		}
		return ;
	}
	if (g->addr != NULL)
		ft_help("route", 1);
	g->addr = a;
}

void get_opt(int a, char **b, t_ping *g)
{
	int i;

	i = 1;
	while (i < a)
		gettype(b[i++], g);
	if (g->flags & 1)
		ft_help("help", 0);
	if (g->addr == NULL)
		ft_help("NULL", 0);
}
