#include "ft_ping.h"

void open_socket(t_ping *g)
{

	if ((g->socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		ft_help("socket", 1);
	if (setsockopt(g->socket, IPPROTO_IP, IP_HDRINCL, \
		(int[1]){1}, sizeof(int)) == -1)
		ft_help("failed to set socket option", 1);
}


int	make_pkt(t_ping *g)
{
	int i;

	g->pkt.hdr.type = ICMP_ECHO;
        g->pkt.hdr.un.echo.id = getpid() & 0xFFFF;
	i = 0;
	while (i < (sizeof(g->pkt.msg) - 1))
	{
		g->pkt.msg[i] = i + '0';
		i++;
	}
	g->pkt.msg[i] = 0;
	g->pkt.hdr.un.echo.sequence = g->seq++;
        g->pkt.hdr.checksum = checksum(&g->pkt, sizeof(g->pkt));
	//usleep(1000);
	return (1);
}

void aff_head(t_ping *g)
{
	printf("FT_PING %s %d(64) data bytes\n", g->addr, sizeof(g->pkt.msg));	
}


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

static int pinger(t_ping *g)
{
	struct timezone tz;
	int datalen = 64-8;
	int ident;
	u_char outpack[4096];
	struct icmp *icp = (struct icmp *) outpack;
	int i, cc;
	struct timeval *tp = (struct timeval *) &outpack[8];
	char *datap = &outpack[8+sizeof(struct timeval)];

	ident = getpid() & 0xFFFF;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = g->seq++;
	if (g->seq == INT_MAX)
		ft_help("seq max", 1);
	icp->icmp_id = ident;		/* ID */
	cc = datalen+8;			/* skips ICMP portion */
	//if (timing)
	//	gettimeofday( tp, &tz );
	for( i=8; i<datalen; i++)	/* skip 8 for time */
		*datap++ = i;
	icp->icmp_cksum = in_cksum( (u_short *)icp, cc );
	return sendto(g->socket, outpack, cc, 0, (const struct sockaddr*)&g->four, sizeof(struct sockaddr));
}

#include <strings.h>

static void ipvfour(t_ping *g)
{
	struct sockaddr_in r_addr;
	int addr_len;
	int flag;
	int c;
	u_char	packet[4096];
	int len = sizeof (packet);
	c = 0;
	flag = 0;
	aff_head(g);
	while (g->loop)
	{
		flag = 1;
		 bzero(&g->pkt, sizeof(g->pkt));
		 make_pkt(g);
		if (sendto(g->socket, &g->pkt, sizeof(g->pkt), 0, (const struct sockaddr*)&g->four, sizeof(struct sockaddr)) <= 0)
			flag = printf("\nPacket Sending Failed!\n") - 24;
		addr_len=sizeof(r_addr);
		if ( recvfrom(g->socket, &g->pkt, sizeof(g->pkt), 0, (struct sockaddr*)&r_addr, &addr_len) <= 0 && g->seq > 1) 
			printf("\n Packet receive failed!");
		else {
			if(flag)
			{
				if(!g->pkt.hdr.type == 69 && g->pkt.hdr.code == 0)
                    			printf("Error..Packet received with ICMP type %d code %d\n",g->pkt.hdr.type, g->pkt.hdr.code);
				else 
				{
					printf("%d\n", c);
					c++;		
					// printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d  rtt = %Lf ms.\n", PING_PKT_S, ping_dom, rev_host,  ping_ip, msg_count, ttl_val, rtt_msec);		
				}
			}
		}	
	}
}

void rec_socket(t_ping *g)
{

	ipvfour(g);
}
