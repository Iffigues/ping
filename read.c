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
	icmp = (struct icmp *) g->sendbuf;
    	icmp->icmp_type = ICMP_ECHO;
    	icmp->icmp_code = 0;
    	icmp->icmp_id = g->pid;
    	icmp->icmp_seq = g->seq++;
    	memset (icmp->icmp_data, 0xa5, g->len);
    	gettimeofday ((struct timeval *) icmp->icmp_data, NULL);
    	len = 8 + PING_PKT_S;
    	icmp->icmp_cksum = 0;
    	icmp->icmp_cksum = in_cksum ((u_short *) icmp, len);
    	if(sendto (g->socket, g->sendbuf, len, 0, g->s, g->len) !=  len)
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
	rtt = a->tv_sec * 1000.0 + a->tv_usec / 1000.0;
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
	return rtt;
}

void readmsg(struct msghdr  *ptr, int len, char * pck, struct timeval *e)	
{
	struct cmsghdr *cmhdr;



	if (!pck) {
		ft_help("eee\n", 1);
	}

	int             hlenl;
    int             icmplen;
    struct ip       *ip;
    struct icmp     *icmp;
    struct timeval  *t;
    double rtts;

    ip = (struct ip *) pck;
    hlenl = ip->ip_hl << 2;
    if (ip->ip_p != IPPROTO_ICMP) {
	    return ;
    }
    icmp = (struct icmp *) (pck + hlenl);
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
	    return;
    }
    printf("icmp = %d %d\n", icmp->icmp_type, ICMP_ECHOREPLY);
     if (icmp->icmp_id != g->pid)
            return ;
    if ((icmplen = len - hlenl) < 8)
        return;
    t = (struct timeval *) icmp->icmp_data;
    rtts = rtt(t, e);
    g->rec++;
    if (!(g->flags & 2))
    	printf("%d bytes from %s (%s) icmp_seq=%u ttl=%d rtt=%.3f ms\n", icmplen, g->addr, g->ip,  icmp->icmp_seq, ip->ip_ttl, rtts);
    else
 printf("%d bytes from %s (%s) type=%d code=%d\n", icmplen, g->addr, g->ip,  icmp->icmp_type, icmp->icmp_code);	
}

void    readloop(void)
{

    char            recvbuf[1500];
    char            controlbuf[1500];
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
    ping(1);
    printf("PING %s (%s): %d data bytes\n", g->h->ai_canonname ? g->h->ai_canonname : g->ip, g->ip, g->len);
    while (g->loop)
    {
        g->msg.msg_namelen = g->len;
        g->msg.msg_controllen = sizeof (controlbuf);
	gettimeofday(&tval, NULL);
        if ((n = recvmsg (g->socket, &g->msg, 0)) < 0)
	     	ft_help("recvmsg error d",1);
	gettimeofday(&tval, NULL);
	if (g->loop)
    		readmsg(&g->msg,n, recvbuf, &tval);
	}
    return ;
}

