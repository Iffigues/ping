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
		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

void ping(int i)
{
	int             len;
	struct icmp     *icmp;
	static u_char outpack[4096];
	u_char *datap = &outpack[8+sizeof(struct timeval)];

	(void)i;
	icmp = (struct icmp *) outpack;
    	icmp->icmp_type = ICMP_ECHO;
    	icmp->icmp_code = 0;
    	icmp->icmp_id = g->pid;
    	icmp->icmp_seq = g->seq++;
    	for( i=8; i<datalen; i++)	/* skip 8 for time */
		*datap++ = i;
    	gettimeofday ((struct timeval *) icmp->icmp_data, NULL);
    	len = datalen + 8;
    	icmp->icmp_cksum = 0;
    	icmp->icmp_cksum = in_cksum ((u_short *) icmp, len);
    	if(sendto (g->socket, outpack, len, 0, g->s, g->len) !=  len)
	     	ft_help("zzz",1);
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
    	printf("eee\n");
   	//return;
    }
    len -= ip->ip_hl << 2;
    icmp = (struct icmp *)(pck + hlen);
    if (icmp->icmp_type != ICMP_ECHOREPLY ) {
	    printf("%d\n", icmp->icmp_type);
	    return;
    }
    if (ip->ip_p != IPPROTO_ICMP) {
		printf("eesd\n");
	    return ;
    }
    printf("aaaa\n");
 /*   icmp = (struct icmp *) (pck + (ip->ip_hl << 2));
    if ((icmplen = len - (ip->ip_hl << 2)) < 8)
        return ;
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
	    if (icmp->icmp_type != ICMP_ECHO)
		    return;
	    return;
    }
     if (icmp->icmp_id != g->pid)
            return ;
     if (icmplen < 16)
            return;
    t = (struct timeval *) icmp->icmp_data;
    rtts = rtt(e, t);
    g->rec++;
    printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d rtt=%.3f ms", icmplen, g->addr, g->ip,  icmp->icmp_seq, ip->ip_ttl, rtts);
    if (g->flags & 2)
 	printf(" type=%d code=%d", icmp->icmp_type, icmp->icmp_code);
    printf("\n");*/
}

void    pong(void)
{

    char            recvbuf[BUFFSIZE];
    char            controlbuf[BUFFSIZE];
    ssize_t         n;
    struct timeval  tval;
   
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
    printf("PING %s (%s): %d data bytes\n", g->h->ai_canonname ? g->h->ai_canonname : g->ip, g->ip, BUFFSIZE);
    ping(1);
    while (g->loop)
    {
        g->msg.msg_namelen = g->len;
        g->msg.msg_controllen = sizeof (controlbuf);
	//gettimeofday(&tval, NULL);
        if ((n = recvmsg (g->socket, &g->msg, 0)) < 0) {
	}
	     	//ft_help("recvmsg error d",1);
	gettimeofday(&tval, NULL);
	if (g->loop)
    		readmsg(n, recvbuf, &tval);
	}
    return ;
}

