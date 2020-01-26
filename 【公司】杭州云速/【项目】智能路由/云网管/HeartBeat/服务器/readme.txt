1.可执行文件在bin目录下，进入bin目录，使用./heartbeat >>../log/log.log 2>&1；即可运行程序；
2.其中log文件夹存放程序运行过程中产生的日志文件；
3.如果需要修改监听端口、redis数据库等设置，请修改bin目录下的配置文件initConf.conf；