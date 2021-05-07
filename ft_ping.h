/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bordenoy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 13:23:16 by bordenoy          #+#    #+#             */
/*   Updated: 2019/04/25 12:47:25 by bordenoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <limits.h>
#define PING_PKT_S 64

static int yl = 10;

typedef	struct	s_pkt
{
		struct icmphdr hdr;
		char msg[PING_PKT_S-sizeof(struct icmphdr)];
}		t_pkt;

typedef struct		s_ping
{
	int		loop;
	char		s[100];
	char		*addr;
	char		lddr[100];
	char		lddrsix[100];
	int		flags;
	int		socket;
	int		seq;
	int		rec;
	int		family;
	struct sockaddr_in four;
	t_pkt		pkt;
	int		ttl;
}			t_ping;

void			ft_help(char *a, int i);
void			get_info(t_ping *g);
void			*ft_memset(void *b, int c, size_t len);
void			open_socket(t_ping *g);
void			rec_socket(t_ping *g);
void			ft_bzero(void *s, size_t n);
unsigned short 		checksum(void *b, int len);
char			*ft_strcpy(char *dst, const char *src);
int	ft_strcmp(const char *s1, const char *s2);
suseconds_t	getRtt(struct timeval *i);
suseconds_t	getTime();
#endif
