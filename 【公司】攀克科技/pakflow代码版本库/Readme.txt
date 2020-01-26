1.0
driver-smp-leadsec  
最早收到的最老版本，目前只是用来看代码。

2.0
linux-2.6.26-PakFlow-1.0
linux-2.6.26-PakFlow-1.0-lastnew
这两个版本为包含linux-2.6.26内核的版本，lastnew为当时最新版本。

3.0
pakflow
从linux-2.6.26-PakFlow-1.0-lastnew中取出来的pakflow的驱动。

4.0
driver.113.newrx
为了pakflow驱动，取消了收包中buffer操作，仿照inter的普通网卡修改而成的驱动。

5.0
driver-smp-leadsec-2013-6-5
pakflow驱动已经修改到支持64位的版本，并且加入了netmap头文件，但在netmap模式和普通网卡模式的转换中还存在问题，必须有待修改。

6.0
driver-smp-leadsec-2013-6-7
完成了netmap模式和普通网卡模式的转换，但现在收包的网口仅限于eth5口收包。
6.1
driver-smp-leadsec-2013-6-8
可以进行双口收包测是，双口均为线速收包，但是由于对于硬件初始化收包过程有疑问，在netmap模式和普通网卡模式下转换出现了问题。
此版本作为备份版本。

7.0
driver-smp-leadsec-2013-6-8-1.0
在6.0与6.1的版本中存在两个bug，（1）切换到netmap模式时，第一个描述符没有被初始化。（2）没有初始化netmap中开ring和ring的cur指针，导致切换到netmap模式下，高速发包无法收包问题。
修改了这两个bug，形成改版本，两个口收包可达到线速，模式切换无问题。

8.0
driver-smp-leadsec-2013-6-20
将整个开发编译环境迁移到了fedora14的64位平台上，针对的网卡也是万兆网卡，需要在64位的系统上才能正常运行。将收发包调试完成。因为是万兆网卡
（1）发包没有办法达到线速，60字节2.4Mpps 2.4Gbps 1024字节6Kpps 5.8Gbps（未用netmap模式，4口同时发包4个G流量）。
（2）收包两个口可以通，使用htt没有测出真实性能。用自循环测试，60字节2.1Mpps

