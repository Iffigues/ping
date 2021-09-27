FROM debian

RUN apt -y update
RUN apt -y upgrade
RUN apt -y install make
RUN apt -y install  gcc
ADD network.txt /.
ADD ft_ping /ft_ping
CMD ["sh"]
