#!/bin/sh
sed -i '/dnsmasq.ads/d' /etc/dnsmasq.conf
grep conf-file /etc/dnsmasq.conf || echo -e "#add dnsmasq.ads rule list.\nconf-file=/opt/app/ADBlock/etc/dnsmasq.ads" >> /etc/dnsmasq.conf
wget -O - http://adblock-chinalist.googlecode.com/svn/trunk/adblock-lazy.txt | grep ^\|\|[^\*]*\^$ | sed -e 's:||:address\=\/:' -e 's:\^:/127\.0\.0\.1:' > /opt/app/ADBlock/etc/dnsmasq.ads
/etc/init.d/dnsmasq restart
