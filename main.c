#include "ft_ping.h"

t_ping *g = NULL;

void poong()
{
	char packett[20 + 56 + ICMP_MINLEN];
}

void intHandler(int dummy)
{
  g->loop = 0;
  close(g->socket);
  freeaddrinfo(g->h);
  exit(0);
}

void open_socket()
{
	int size;

	size = 60 * 1024;
	struct timeval tv_out;
   	 tv_out.tv_sec = 4;
   	 tv_out.tv_usec = 0;
	if ((g->socket = socket(g->s->sa_family, SOCK_RAW, IPPROTO_ICMP)) < 0)
		ft_help("socket", 1);
	if ( setsockopt (g->socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof (size)) < 0)
		ft_help("failed to set socket option", 1);
	// setting timeout of recv setting
 	//setsockopt(g->socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
//	int on = 1;
	 //if (setsockopt(g->socket, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
	// 	perror("zz");
	 //}
/*		int ttl = 64;
  if (setsockopt(g->socket, SOL_SOCKET, SO_TIMESTAMP, &on, (int)sizeof(on)) != 0)
    fprintf(stderr, "\nSetting socket options to TIMESTAMP failed!\n");
  if (setsockopt(g->socket, IPPROTO_IP, IP_RECVTTL, &on, (int)sizeof(on)) != 0)
    fprintf(stderr, "\nSetting socket options to RECVTTL failed!\n");
  if (setsockopt(g->socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
    fprintf(stderr, "\nSetting socket options to TTL failed!\n");
*/

}

static void start()
{
	int ttl;

	ttl = 1;
	struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;
	g->ip = NULL;
	g->ip = get_info();
	if (g->family == 6)
		ft_help("ipv6 not supported", 1);
	open_socket(g);
	signal(SIGINT, intHandler);
	signal(SIGALRM, ping);
	readloop();
}

int main(int argc, char **argv)
{
	if (argc == 1)
		ft_help("error", 1);
	if (!(g = (t_ping *)malloc(sizeof(t_ping))))
		ft_help("zzz", 1);		
	g->s = NULL;
	g->r = NULL;
	get_opt(argc, argv, g);
	start();
	close(g->socket);
	freeaddrinfo(g->h);
	return (0);
}
