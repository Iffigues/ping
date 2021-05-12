#include "ft_ping.h"

void    sig_alrm (int signo)
{
    ping();
    alarm(1);
}
