#include "ft_ping.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char	*h;

	h = (char *)b;
	while (len)
		if (len--)
			*h++ = (unsigned char)c;
	return (b);
}


void	ft_bzero(void *s, size_t n)
{
	s = ft_memset(s, 0, n);
}

char	*ft_strcpy(char *dst, const char *src)
{
	char *p;

	p = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (p);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2) && s1++ && s2++)
		;
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
