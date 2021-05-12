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
    	icmp->icmp_id = getpid();
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

void readmsg(struct msghdr  *ptr, int len, char * pck, int *cc)	
{
	struct cmsghdr *cmhdr;



	if (len < 84)
		return;
	//d
	if (!pck) {
		ft_help("eee\n", 1);
	}

	    int             hlenl;
    int             icmplen;
    double          rtt;
    struct ip       *ip;
    struct icmp     *icmp;
    struct timeval  *tvsend;
    
    ip = (struct ip *) pck;
    hlenl = ip->ip_hl << 2;
    if (ip->ip_p != IPPROTO_ICMP) {
	    printf("zzz\n");
	    return ;
    }
    icmp = (struct icmp *) (pck + hlenl);
    if (icmp->icmp_type != ICMP_ECHOREPLY)
	    return;
    if ((icmplen = len - hlenl) < 8)
        return ;
    printf("uu %d = %d\n", ICMP_ECHOREPLY, icmp->icmp_seq);
}

void    readloop(void)
{

    char            recvbuf[1500];
    char            controlbuf[1500];
    ssize_t         n;
    struct timeval  tval;
   int cc;

  cc = 1;   
    g->iov.iov_base = recvbuf;
    g->iov.iov_len = sizeof (recvbuf);
    g->msg.msg_name = g->r;
    g->msg.msg_iov = &g->iov;
    g->msg.msg_iovlen = 1;
    g->msg.msg_control = controlbuf;
    ping(1);
    printf("PING %s (%s): %d data bytes\n", g->h->ai_canonname ? g->h->ai_canonname : g->ip, g->ip, g->len);
    while (1)
    {
        g->msg.msg_namelen = g->len;
        g->msg.msg_controllen = sizeof (controlbuf);
	gettimeofday(&tval, NULL);
        if ((n = recvmsg (g->socket, &g->msg, 0)) < 0) {
		perror("z");
         	printf("%d\n", n);
	     	ft_help("recvmsg error d",1);
	}else
    		readmsg(&g->msg,n, recvbuf, &cc);
    }
    return ;
}

