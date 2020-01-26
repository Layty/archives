# cat nf_conntrack_expect |grep 192.168.13.152
133 l3proto = 2 proto=47 src=192.168.13.152 dst=192.168.13.118 srckey=0xa9a0 dstkey=0x6bd8 pptp
# 
# cat nf_conntrack |grep 192.168.13.152       

ipv4     2 gre      47 17999 timeout=600, stream_timeout=18000 src=192.168.188.114 dst=192.168.13.152 srckey=0xa56c dstkey=0xa9a0 src=192.168.13.152 dst=192.168.13.118 srckey=0xa9a0 dstkey=0x6bd8 [ASSURED] use=3

ipv4     2 tcp      6 1558 ESTABLISHED src=192.168.188.114 dst=192.168.13.152 sport=27608 dport=1723 src=192.168.13.152 dst=192.168.13.118 sport=1723 dport=27608 [ASSURED] use=3