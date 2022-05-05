#include "ft_ping.h"

u_short in_cksum(u_short *addr,  int len)
{
	int nleft = len;
	u_short *w = addr;
	u_short answer;
	int sum = 0;

	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}
	if( nleft == 1 ) {
		u_short	u = 0;
		*(u_char *)(&u) = *(u_char *)w;
		sum += u;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

void ping(int i)
{
	int             len;
	static u_char outpack[128];
	struct icmp *icmp = (struct icmp *) outpack; 
	u_char *datap = &outpack[8+sizeof(struct timeval)];

	(void)i;

	ft_bzero(outpack, sizeof(outpack));
	icmp = (struct icmp *) outpack;
    	icmp->icmp_type = ICMP_ECHO;
    	icmp->icmp_code = 0;
    	icmp->icmp_id = g->pid & 0xffff;
    	icmp->icmp_seq = g->seq++;
    	for( i=8; i<datalen; i++);
		*datap++ = i;
    	gettimeofday ((struct timeval *) icmp->icmp_data, NULL);
    	len = datalen + 8;
    	icmp->icmp_cksum = 0;
    	icmp->icmp_cksum = in_cksum ((u_short *) icmp, len);
    	if(sendto (g->socket, &outpack, len, 0, g->s, g->len) !=  len) {
	}
	alarm(1);
}

void getRtts(struct timeval *out, struct timeval *in) {
	out->tv_usec  = out->tv_usec - in->tv_usec;
	if (out->tv_usec < 0)
    {

        out->tv_sec--;
        out->tv_usec += 1000000;

    }
    out->tv_sec = out->tv_sec - in->tv_sec;
}

double  rtt(struct timeval *a, struct timeval *b)
{
	double rtt;

	rtt = 0;
	getRtts(a, b);
	rtt = (a->tv_sec * 1000.0 + a->tv_usec / 1000.0) - PING_IHR;
	if (g->rec == 0) {
		g->rttmin = rtt;
		g->rttmax = rtt;
	} else {
		if (rtt < g->rttmin)
			g->rttmin = rtt;
		if (rtt > g->rttmax)
			g->rttmax = rtt;
	}
	g->avg += rtt;
	return rtt ;
}


void readmsg(int len, char * pck, struct timeval *e)	
{

    int             icmplen;
    struct ip       *ip;
    struct icmp     *icmp;
    long *lp = (long *) g->sendbuf;
    int i;
    int hlen;
    struct timeval  *t;
    double rtts;

     if (!pck)
	ft_help("empty packet\n", 1);
    ip = (struct ip *) pck;
    hlen = ip->ip_hl << 2;
    if (len < hlen + ICMP_MINLEN) {
	    return;
    }
    len -= hlen;
    icmp = (struct icmp *)(pck + hlen);
    if (icmp->icmp_type != ICMP_ECHOREPLY ) {
	    return;
    }
    if( icmp->icmp_id != g->pid) {
	    return;
    }
    t = (struct timeval *) icmp->icmp_data;
    rtts = rtt(e, t);
    g->rec++;
    printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d rtt=%.3f ms\n", icmplen, g->addr, g->ip,  icmp->icmp_seq, ip->ip_ttl, rtts);
}

void    pong(void)
{
    int si = 84;
    char            recvbuf[si];
    char            controlbuf[si];
    ssize_t         n;
    struct timeval  tval;


    ft_bzero(recvbuf, si);
    ft_bzero(controlbuf, si);   
    g->loop = 1;
    g->avg = 0;
    g->rttmin = 0;
    g->rttmax = 0;
    g->seq = 1;
    g->iov.iov_base = recvbuf;
    g->iov.iov_len = sizeof (recvbuf);
    g->msg.msg_name = g->r;
    g->msg.msg_iov = &g->iov;
    g->msg.msg_iovlen = 1;
    g->msg.msg_control = controlbuf;
    printf("PING %s (%s): %d data bytes\n", g->h->ai_canonname ? g->h->ai_canonname : g->ip, g->ip, si);
    ping(1);
    while (g->loop)
    {
        g->msg.msg_namelen = g->len;
        g->msg.msg_controllen = sizeof (controlbuf);
        if ((n = recvmsg (g->socket, &g->msg, 0)) < 0) {
	}
	gettimeofday(&tval, NULL);
	if (g->loop)
    		readmsg(n, recvbuf, &tval);
	}
    return ;
}

