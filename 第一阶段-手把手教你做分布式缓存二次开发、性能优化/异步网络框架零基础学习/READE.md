借助redis已有的网络相关.c和.h文件，半小时快速实现一个epoll异步网络框架，包括网络收发，epoll注册过程，定时器实现原理  
  
linux安装编译测试
===================================  
make编译出可执行文件net,然后运行
  
服务端(运行net可执行文件)  
[root@bogon asyn_network]# ./net  
  
客户端用telnet链接，发送字符串  
[root@bogon asyn_network]# telnet 127.0.0.1 6379
Trying 127.0.0.1...  
Connected to 127.0.0.1.  
Escape character is '^]'.  
hello boy  
hello boy  
  
看看服务端打印：
[root@bogon asyn_network]# ./net
process Start begin  
MainTimerExpire  
client 127.0.0.1:60044 Connected  
MainTimerExpire  
recv from client 127.0.0.1:60044, data:hello boy  