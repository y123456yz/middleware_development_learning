# middleware_development_learning
手把手教你做中间件、高性能服务器、分布式存储等(redis、memcache、pika、rocksdb、mongodb、wiredtiger、高性能代理中间件)二次开发、性能优化，逐步整理文档说明并配合demo指导  
  
近期抽业余时间来总结多年工作中学习到的各种中间件、分布式存储、高性能服务端等技术，通过文章博客和程序demo等方式呈现给大家，暂定分享思路如下：
  
分阶段分享  
===================================  
第一阶段：手把手教你做分布式缓存源码学习、二次开发、性能及稳定性优化  
主要涉及网络实现、memcache redis重要模块源码分析、memcache redis性能稳定性优化及二次开发等  
  
  
第二阶段：手把手教你做高性能代理中间件开发(主要结合高性能web服务器nginx、缓存代理中间件twemproxy、mysql代理中间件dbproxy、mongodb代理中间件mongos等源码进行二次开发)  
主要涉及代理中间件源码分析、性能优化、二次开发等  
  
  
  
第三阶段：手把手教你做分布式大容量nosql存储系统二次开发(突破缓存内存容量限制)  
主要涉及pika源码、rocksdb存储引擎源码分析及pika性能优化等  
  
  
  
第四阶段：手把手教你做mongodb数据库内核开发(突破redis、pika只支持redis协议限制，学习更复杂查询、索引、更多数据库相关等实现原理)  
主要涉及mongodb源码、mongos源码、rocksdb存储引擎源码、wiredtiger存储引擎源码分析及二次开发  

  
  
## 第一阶段：手把手教你做分布式缓存开发、性能稳定性优化:    
  * [memcached源码详细分析注释，带详尽中文注释及函数调用关系](https://github.com/y123456yz/Reading-and-comprehense-redis-cluster)  
  * [redis源码详细分析注释，带详尽中文注释及函数调用关系](https://github.com/y123456yz/Reading-and-comprehense-memcached-1.4.22)  
    * [借助redis已有的网络相关.c和.h文件，半小时快速实现一个epoll异步网络框架，程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network)  
    * [借助redis已有的网络相关.c和.h文件，半小时快速实现一个epoll异步网络框架，程序demo-文档说明](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network.md)    
    * [阻塞、非阻塞程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/block_noblock_demo)  
    * [阻塞、非阻塞、同步、异步、epoll说明](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network.md)    
    * [借助redis的配置解析模块，快速实现一个配置文件解析程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%A7%A3%E6%9E%90%E7%A8%8B%E5%BA%8F%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%A7%A3%E6%9E%90%E7%A8%8B%E5%BA%8Fdemo)
    * [借助redis的日志模块，快速实现一个同步日志写、异步日志写程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E6%97%A5%E5%BF%97%E4%BB%A3%E7%A0%81%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E6%97%A5%E5%BF%97%E5%90%8C%E6%AD%A5%E5%86%99%E5%92%8C%E5%BC%82%E6%AD%A5%E5%86%99%EF%BC%8C%E4%BD%93%E9%AA%8C%E5%90%8C%E6%AD%A5%E5%86%99%E5%92%8C%E5%BC%82%E6%AD%A5%E5%86%99%E5%8C%BA%E5%88%AB)
    * [借助redis的StackTrace功能，快速StackTrace程序demo, 记录程序异常时的函数栈信息，便于快速定位bug](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E4%BF%A1%E5%8F%B7%E5%92%8Cbacktrace%E6%9C%BA%E5%88%B6%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0backtrace%E4%BB%A3%E7%A0%81%EF%BC%8C%E8%AE%B0%E5%BD%95%E7%A8%8B%E5%BA%8Fbug%E5%BC%82%E5%B8%B8%E9%80%80%E5%87%BA%E7%9A%84%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8%E6%A0%88%EF%BC%8C%E4%BE%BF%E4%BA%8E%E5%BF%AB%E9%80%9F%E5%AE%9A%E4%BD%8Dbug)
    * [借助redis的bio模块，快速实现线程池组demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E7%9A%84bio%E4%BB%A3%E7%A0%81%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E7%BA%BF%E7%A8%8B%E6%B1%A0demo)
  
  
  
## 第二阶段：手把手教你做高性能代理中间件开发   
  * [redis、memcached缓存代理twemproxy源码详细分析注释，带详尽中文注释及函数调用关系](https://github.com/y123456yz/Reading-and-comprehense-twemproxy0.4.1)  
  * [nginx-1.9.2源码通读分析注释，带详尽函数中文分析注释](https://github.com/y123456yz/reading-code-of-nginx-1.9.2)  
  * [nginx多进程、高性能、低时延、高可靠机制应用于缓存中间件twemproxy，对twemproxy进行多进程优化改造，提升TPS，降低时延，代理中间件长连接百万TPS/短连接五十万TPS实现原理](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E9%AB%98%E6%80%A7%E8%83%BD%E4%BB%A3%E7%90%86%E4%B8%AD%E9%97%B4%E4%BB%B6%E5%BC%80%E5%8F%91/nginx%E5%A4%9A%E8%BF%9B%E7%A8%8B%E9%AB%98%E5%B9%B6%E5%8F%91%E4%BD%8E%E6%97%B6%E5%BB%B6%E6%9C%BA%E5%88%B6%E5%9C%A8%E7%BC%93%E5%AD%98%E4%BB%A3%E7%90%86%E4%B8%AD%E9%97%B4%E4%BB%B6twemproxy%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A8/nginx_twemproxy.md)  
  
  
  
## 第三阶段：手把手教你做wiredtiger、rocksdb存储引擎开发，大容量nosql存储系统二次开发   
  * [文档数据库mongodb kv存储引擎wiredtiger源码详细分析注释](https://github.com/y123456yz/reading-and-annotate-wiredtiger-3.0.0)  
  * [rocksdb-6.1.2 KV存储引擎源码中文注释分析](https://github.com/y123456yz/reading-and-annotate-rocksdb-6.1.2)  
   
   
   
## 其他分享   
  * [阿里巴巴分布式消息队列中间件rocketmq-3.4.6源码分析](https://github.com/y123456yz/reading-and-annotate-rocketmq-3.4.6)  
  * [服务器时延统计工具tcprstat,增加时延阈值统计，记录超过阈值的包个数，并把数据包时间戳记录到日志文件，这样可以根据时间戳快速定位到抓包文件中对应的包，从而可以快速定位到大时延包，避免了人肉搜索抓包文件，提高问题排查效率](https://github.com/y123456yz/tcprstat)   
  * [linux内核网络协议栈源码阅读分析注释](https://github.com/y123456yz/Reading-and-comprehense-linux-Kernel-network-protocol-stack)  
  * [docker-17.05.0源码中文注释详细分析](https://github.com/y123456yz/reading-and-annotate-docker-17.05.0)  
  * [lxc源码详细注释分析](https://github.com/y123456yz/reading-and-annotate-lxc-1.0.9)  
  * [source insight代码中文注释乱码、背景色等配置调整](https://github.com/y123456yz/middleware_development_learning/tree/master/source%20insight%20configure)   
  * [linux内核协议栈TCP time_wait原理、优化、副作用](https://my.oschina.net/u/4087916/blog/3051356)         
  * [为何需要对开源社区版本mongodb做二次开发，需要做哪些二次开发](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E5%9B%9B%E9%98%B6%E6%AE%B5-mongodb%E6%95%B0%E6%8D%AE%E5%BA%93/development_mongodb.md)  
  * [在线引流工具Tcpcopy原理、环境搭建、使用、采坑](https://my.oschina.net/u/4087916/blog/3064268)         
  
  
  
## 技术交流群  
对linux c/c++ nginx redis memcache twemproxy mongodb 中间件 存储引擎 分布式 高并发 高性能服务端等技术敢兴趣的同学可以加群： QQ交流群1(针对在校生)-(群号：568892619 )      QQ交流群2(针对已工作)-(581089275)     
微信二维码(由于群人员超过100，只能靠邀请加入，请先加如下微信，然后我拉进群)：
   
![](/img/9A1AA1E5ED9CE5F11CA0781DEC2D5AEC.jpeg)
   
  
  