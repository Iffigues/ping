#include "ft_ping.h"

t_ping *g = NULL;


long long current() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}


struct timeval hid(struct timeval begin, struct timeval end)
{
	struct timeval ans;
	ans.tv_sec = end.tv_sec - begin.tv_sec;
	ans.tv_usec = end.tv_usec - begin.tv_usec;
	if(ans.tv_usec < 0)
	{
		ans.tv_sec--;
		ans.tv_usec+=1000000;
	}
	return ans;
}



void intHandler(int d)
{
	int h = 0;
  (void)d;
  struct timeval  tval;
  gettimeofday(&tval, NULL);
  struct timeval ll = hid(g->tl, tval);
  long time = ll.tv_sec*1000+ll.tv_usec/1000;
  g->loop = 0;
  close(g->socket);
  freeaddrinfo(g->h);
  printf("\n--- %s ping statistics ---\n", g->addr);
  g->seq--;
  int y = g->seq - g->rec;
  if (y < 0)
	  y = 0;
  if (g->rec > 0)
	  h = (y * 100) / g->rec;
  else 
	  if (g->seq > 0)
	  	h = 100;
  printf("%d packets transmitted, %d recceived, %d%% packet lost, time %ldms\n", g->seq, g->rec, h, time);
  if (h != 100)
	  printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f ms", g->rttmin, g->rttmax, g->avg / g->rec);
  free(g);
  exit(0);
}

void open_socket()
{
	int size;

	size = 84;
	if ((g->socket = socket(g->s->sa_family, SOCK_RAW, IPPROTO_ICMP)) < 0)
		ft_help("snot found", 1);
	if ( setsockopt (g->socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof (size)) < 0)
		ft_help("failed to set socket option", 1);
	int on = 1;
		int ttl = 64;
  if (setsockopt(g->socket, SOL_SOCKET, SO_TIMESTAMP, &on, (int)sizeof(on)) != 0)
    fprintf(stderr, "\nSetting socket options to TIMESTAMP failed!\n");
  if (setsockopt(g->socket, IPPROTO_IP, IP_RECVTTL, &on, (int)sizeof(on)) != 0)
    fprintf(stderr, "\nSetting socket options to RECVTTL failed!\n");
  if (setsockopt(g->socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
    fprintf(stderr, "\nSetting socket options to TTL failed!\n");

	struct timeval tv_out;
    tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;

  setsockopt(g->socket, SOL_SOCKET, SO_RCVTIMEO,
                   (const char*)&tv_out, sizeof tv_out);

}

static void start()
{
	g->ip = NULL;
	g->pert = 0;
	g->ip = get_info();
	if (g->family == 6)
		ft_help("ipv6 not supported", 1);
	open_socket(g);
	signal(SIGINT, intHandler);
	gettimeofday(&g->tl, NULL);
	signal(SIGALRM, ping);
	pong();
}

int main(int argc, char **argv)
{
	if (argc == 1)
		ft_help("-h: help\n-v: verbose", 1);
	if (!(g = (t_ping *)malloc(sizeof(t_ping))))
		ft_help("malloc fail", 1);		
	g->s = NULL;
	g->r = NULL;
	g->pid = getpid() & 0xFFFF;
	get_opt(argc, argv, g);
	start();
	free(g);
	return (0);
}
