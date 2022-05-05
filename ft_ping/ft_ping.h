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
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>
# include <limits.h>
# include <strings.h>
# include <string.h> 
# define datalen 64 - 8
# define PING_PKT_S 56
# define PING_IHR  0.03
# define BUFFSIZE 4096

typedef struct		s_ping
{
	struct timeval  tl;
	int		loop;
	char		*addr;
	int		flags;
	int		socket;
	int		seq;
	int		pert;
	int		rec;
	int		family;
	size_t          len;	
	struct sockaddr *s;
	struct sockaddr *r;
	struct addrinfo  *h;
	struct msghdr   msg;
   	struct iovec    iov;
	char		*ip;
	u_char            sendbuf[BUFFSIZE];
	int		ttl;
	int		pid;
	double		rttmin;
	double 		rttmax;
	double		avg;
}			t_ping;

extern t_ping		*g;
double  rtt(struct timeval *a, struct timeval *b);
void			sig_alrm(int signo);
void			gettype(char *a, t_ping *g);
void			get_opt(int a, char **b, t_ping *g);
void			ft_help(char *a, int i);
char			*get_info();
void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
unsigned short 		checksum(void *b, int len);
char			*ft_strcpy(char *dst, const char *src);
int			ft_strcmp(const char *s1, const char *s2);
suseconds_t		getRtt(struct timeval *i);
suseconds_t		getTime();
void			pong();
void			ping();
#endif
