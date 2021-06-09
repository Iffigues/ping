#!/bin/sh
apt-get -y update
apt-get -y upgrade
apt-get -y install iptables
for var in "$@"
do
    echo "$var"
done
tail -f /dev/null
