# middleware_development_learning
中间件、高性能服务器、分布式存储等(redis、memcache、pika、rocksdb、mongodb、wiredtiger、高性能代理中间件)二次开发、性能优化，逐步整理文档说明并配合demo指导  
  
近期抽业余时间来总结多年工作中学习到的各种中间件、分布式存储、高性能服务端等技术，通过文章博客和程序demo等方式呈现给大家，暂定分享思路如下：
  
### 对外演讲   
|#|对外演讲内容|
|:-|:-|
|1|[Qcon全球软件开发大会分享：OPPO万亿级文档数据库MongoDB集群性能优化实践](https://qcon.infoq.cn/2020/shenzhen/track/916)|
|2|[2019年mongodb年终盛会：OPPO百万级高并发MongoDB集群性能数十倍提升优化实践](https://www.shangyexinzhi.com/article/428874.html)|
|3|[2020年mongodb年终盛会：万亿级文档数据库集群性能优化实践](https://mongoing.com/archives/76151)|
|4|[2021年dbaplus分享：万亿级文档数据库集群性能优化实践](http://dbaplus.cn/news-162-3666-1.html)|
|5|[2021年度Gdevops全球敏捷运维峰会：PB级万亿数据库性能优化及最佳实践](https://gdevops.com/index.php?m=content&c=index&a=lists&catid=87)|
  
### 专栏  
|#|专栏名内容|
|:-|:-|
|1|[infoq专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.infoq.cn/profile/8D2D4D588D3D8A/publish)|
|2|[oschina专栏:《mongodb内核源码中文注释详细分析及性能优化实践系列》](https://my.oschina.net/u/4087916)|
|3|[知乎专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.zhihu.com/people/yang-ya-zhou-42/columns)|
|4|[itpub专栏:《mongodb内核源码设计实现、性能优化、最佳运维实践》](http://blog.itpub.net/column/150)|

   

分阶段分享  
===================================  
|#|阶段|内容|说明|
|:-|:-|:-|:-|
|1|第一阶段|分布式缓存源码学习、二次开发、性能及稳定性优化|主要涉及网络实现、memcache redis重要模块源码分析、memcache redis性能稳定性优化及二次开发等|
|2|第二阶段|高性能代理中间件开发(nginx、wemproxy、dbproxy、mongos等源码进行二次开发)|主要涉及代理中间件源码分析、性能优化、二次开发等|
|3|第三阶段|分布式大容量nosql存储系统二次开发(突破缓存内存容量限制)|主要涉及pika、tendis源码、rocksdb存储引擎源码分析及pika性能优化等|
|4|第四阶段|mongodb数据库内核开发|主要涉及mongodb源码、mongos源码、rocksdb存储引擎源码、wiredtiger存储引擎源码分析及二次开发|
   
  
  
## 第一阶段：分布式缓存开发、性能稳定性优化:    
|#|内容|
|:-|:-|
|1|[memcached源码详细分析注释，带详尽中文注释及函数调用关系](https://github.com/y123456yz/Reading-and-comprehense-redis-cluster)|
|2|[借助redis已有的网络相关.c和.h文件，半小时快速实现一个epoll异步网络框架，程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network)|
|3|[借助redis已有的网络相关.c和.h文件，半小时快速实现一个epoll异步网络框架，程序demo-文档说明](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network.md)|
|4|[阻塞、非阻塞程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/block_noblock_demo)|
|5|[阻塞、非阻塞、同步、异步、epoll说明](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%BC%82%E6%AD%A5%E7%BD%91%E7%BB%9C%E6%A1%86%E6%9E%B6%E9%9B%B6%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/asyn_network.md)|
|6|[借助redis的配置解析模块，快速实现一个配置文件解析程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%A7%A3%E6%9E%90%E7%A8%8B%E5%BA%8F%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%A7%A3%E6%9E%90%E7%A8%8B%E5%BA%8Fdemo)|
|7|[借助redis的日志模块，快速实现一个同步日志写、异步日志写程序demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E6%97%A5%E5%BF%97%E4%BB%A3%E7%A0%81%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E6%97%A5%E5%BF%97%E5%90%8C%E6%AD%A5%E5%86%99%E5%92%8C%E5%BC%82%E6%AD%A5%E5%86%99%EF%BC%8C%E4%BD%93%E9%AA%8C%E5%90%8C%E6%AD%A5%E5%86%99%E5%92%8C%E5%BC%82%E6%AD%A5%E5%86%99%E5%8C%BA%E5%88%AB)|
|8|[借助redis的bio模块，快速实现线程池组demo](https://github.com/y123456yz/middleware_development_learning/tree/master/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E5%88%86%E5%B8%83%E5%BC%8F%E7%BC%93%E5%AD%98%E4%BA%8C%E6%AC%A1%E5%BC%80%E5%8F%91%E3%80%81%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%A8%A1%E5%9D%97%E5%88%86%E6%9E%90/%E5%9F%BA%E4%BA%8Eredis%E7%9A%84bio%E4%BB%A3%E7%A0%81%EF%BC%8C%E5%BF%AB%E9%80%9F%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E7%BA%BF%E7%A8%8B%E6%B1%A0demo)|
|9|[常用高并发网络线程模型设计(最全高并发网络IO线程模型设计及优化)](https://my.oschina.net/u/4087916/blog/4431422)  |
  
  
## 第二阶段：高性能代理中间件开发  
|#|内容|
|:-|:-|
|1|[redis、memcached缓存代理twemproxy源码详细分析注释，带详尽中文注释及函数调用关系](https://github.com/y123456yz/Reading-and-comprehense-twemproxy0.4.1)|
|2|[nginx-1.9.2源码通读分析注释，带详尽函数中文分析注释](https://github.com/y123456yz/reading-code-of-nginx-1.9.2)|
|3|[nginx多进程、高性能、低时延、高可靠机制应用于缓存中间件twemproxy，对twemproxy进行多进程优化改造，提升TPS，降低时延，代理中间件长连接百万TPS/短连接五十万TPS实现原理](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5-%E6%89%8B%E6%8A%8A%E6%89%8B%E6%95%99%E4%BD%A0%E5%81%9A%E9%AB%98%E6%80%A7%E8%83%BD%E4%BB%A3%E7%90%86%E4%B8%AD%E9%97%B4%E4%BB%B6%E5%BC%80%E5%8F%91/nginx%E5%A4%9A%E8%BF%9B%E7%A8%8B%E9%AB%98%E5%B9%B6%E5%8F%91%E4%BD%8E%E6%97%B6%E5%BB%B6%E6%9C%BA%E5%88%B6%E5%9C%A8%E7%BC%93%E5%AD%98%E4%BB%A3%E7%90%86%E4%B8%AD%E9%97%B4%E4%BB%B6twemproxy%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A8/nginx_twemproxy.md)|
|4|[常用高并发网络线程模型设计](https://my.oschina.net/u/4087916/blog/4431422)|
  
  
  
## 第三阶段：wiredtiger、rocksdb存储引擎开发，大容量nosql存储系统二次开发   
|#|内容|
|:-|:-|
|1|[文档数据库mongodb kv存储引擎wiredtiger源码详细分析注释](https://github.com/y123456yz/reading-and-annotate-wiredtiger-3.0.0)|
|2|[rocksdb-6.1.2 KV存储引擎源码中文注释分析](https://github.com/y123456yz/reading-and-annotate-rocksdb-6.1.2)|
|3|[百万级高并发mongodb集群性能数十倍提升优化实践(上篇)](https://my.oschina.net/u/4087916/blog/3141909)|
   
   
## 第四阶段：mongodb数据库源码学习，二次开发等   
###《mongodb内核源码设计与实现》源码模块化分析  
#### 第一阶段：单机内核源码分析  
![mongodb单机模块化架构图](/image/单机模块架构.png)  
|#|单机模块名|核心代码中文注释|说明|模块文档输出|
|:-|:-|:-|:-|:-|
|1|[网络收发处理(含工作线程模型)](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L8)|网络处理模块核心代码实现(100%注释分析)|完成ASIO库、网络数据收发、同步线程模型、动态线程池模型等功能|[详见infoq专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.infoq.cn/profile/8D2D4D588D3D8A/publish)|
|2|[command命令处理模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L85)|命令处理相关模块源码分析(100%注释分析)|完成命令注册、命令执行、命令分析、命令统计等功能|[详见oschina专栏:《mongodb内核源码中文注释详细分析及性能优化实践系列》](https://www.infoq.cn/profile/8D2D4D588D3D8A/publish)|
|3|[write写(增删改操作)模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L115))|增删改写模块(100%注释分析)|完成增删改对应命令解析回调处理、事务封装、storage存储模块对接等功能|[详见知乎专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.zhihu.com/people/yang-ya-zhou-42/columns)|
|4|[query查询引擎模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L131))|query查询引擎模块(核心代码注释)|完成expression tree解析优化处理、querySolution生成、最优索引选择等功能|[详见知乎专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.zhihu.com/people/yang-ya-zhou-42/columns)|
|5|[concurrency并发控制模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/tree/master/mongo/src/mongo/db/concurrency)|并发控制模块(核心代码注释)|完成信号量、读写锁、读写意向锁相关实现及封装|[详见infoq专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.infoq.cn/profile/8D2D4D588D3D8A/publish)|
|6|[index索引模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L240)|index索引模块(100%注释分析)|完成索引解析、索引管理、索引创建、文件排序等功能|[详见oschina专栏:《mongodb内核源码中文注释详细分析及性能优化实践系列》](https://www.infoq.cn/profile/8D2D4D588D3D8A/publish)|
|7|[storage存储模块](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6/blob/master/mongo/README.md#L115))|storage存储模块(100%注释分析)|完成存储引擎注册、引擎选择、中间层实现、KV实现、wiredtiger接口实现等功能|[详见知乎专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://www.zhihu.com/people/yang-ya-zhou-42/columns)|
|8|[wiredtiger存储引擎](https://github.com/y123456yz/reading-and-annotate-wiredtiger-3.0.0)) |wiredtiger存储引擎设计与实现专栏分析(已分析部分)|完成expression tree解析优化处理、querySolution生成、最优索引选择等功能|[详见知乎专栏：《MongoDB内核源码设计、性能优化、最佳运维实践》](https://github.com/y123456yz/reading-and-annotate-wiredtiger-3.0.0)|
  
#### 第二阶段：复制集内核源码分析(已分析部分源码，待整理)  
     
     
#### 第三阶段：sharding分片内核源码分析(已分析部分源码，待整理)  
  
  
### <<千万级峰值tps/十万亿级数据量文档数据库内核研发及运维之路>>   
|#|文章内容|
|:-|:-|
|1|[盘点 2020 - 我要为分布式数据库 mongodb 在国内影响力提升及推广做点事](https://xie.infoq.cn/article/372320c6bb93ddc5b7ecd0b6b)|
|2|[万亿级数据库 MongoDB 集群性能数十倍提升及机房多活容灾实践](https://xie.infoq.cn/article/304a748ad3dead035a449bd51)|
|3|[Qcon现代数据架构 -《万亿级数据库 MongoDB 集群性能数十倍提升优化实践》核心 17 问详细解答](https://xie.infoq.cn/article/0c51f3951f3f10671d7d7123e)|
|4|[话题讨论 - mongodb 相比 mysql 拥有十大核心优势，为何国内知名度不高？](https://xie.infoq.cn/article/180d98535bfa0c3e71aff1662)|
|5|[万亿级数据库 MongoDB 集群性能数十倍提升及机房多活容灾实践](https://xie.infoq.cn/article/304a748ad3dead035a449bd51)|
|6|[百万级高并发mongodb集群性能数十倍提升优化实践(上篇)](https://my.oschina.net/u/4087916/blog/3141909)|
|7|[Mongodb网络传输处理源码实现及性能调优-体验内核性能极致设计](https://my.oschina.net/u/4087916/blog/4295038)|
|8|[常用高并发网络线程模型设计及mongodb线程模型优化实践(最全高并发网络IO线程模型设计及优化)](https://my.oschina.net/u/4087916/blog/4431422) |
|9|[Mongodb集群搭建一篇就够了-复制集模式、分片模式、带认证、不带认证等(带详细步骤说明)](https://my.oschina.net/u/4087916/blog/4661542)|
|10|[Mongodb特定场景性能数十倍提升优化实践(记一次mongodb核心集群雪崩故障)](https://blog.51cto.com/14951246)|
|11|[mongodb内核源码设计实现、性能优化、最佳运维系列-mongodb网络传输层模块源码实现二](https://zhuanlan.zhihu.com/p/265701877)|
|12|[为何需要对开源mongodb社区版本做二次开发，需要做哪些必备二次开发](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6.1/blob/master/development_mongodb.md)|
|13|[对开源mongodb社区版本做二次开发收益列表](https://my.oschina.net/u/4087916/blog/3063529)|
|14|[盘点 2020 - 我要为分布式数据库 mongodb 在国内影响力提升及推广做点事](https://xie.infoq.cn/article/372320c6bb93ddc5b7ecd0b6b)|
|15|[mongodb内核源码实现、性能调优、最佳运维实践系列-数百万行mongodb内核源码阅读经验分享](https://my.oschina.net/u/4087916/blog/4696104)|
|16|[mongodb内核源码实现、性能调优、最佳运维实践系列-mongodb网络传输层模块源码实现一](https://my.oschina.net/u/4087916/blog/4295038)|
|17|[mongodb内核源码实现、性能调优、最佳运维实践系列-mongodb网络传输层模块源码实现二](https://my.oschina.net/u/4087916/blog/4674521)|
|18|[mongodb内核源码实现、性能调优、最佳运维实践系列-mongodb网络传输层模块源码实现三](https://my.oschina.net/u/4087916/blog/4678616)|
|19|[mongodb内核源码实现、性能调优、最佳运维实践系列-mongodb网络传输层模块源码实现四](https://my.oschina.net/u/4087916/blog/4685419)|
|20|[mongodb内核源码实现、性能调优、最佳运维实践系列-command命令处理模块源码实现一](https://my.oschina.net/u/4087916/blog/4709503)|
|21|[mongodb内核源码实现、性能调优、最佳运维实践系列-command命令处理模块源码实现二](https://my.oschina.net/u/4087916/blog/4748286)|
|22|[mongodb内核源码实现、性能调优、最佳运维实践系列-command命令处理模块源码实现三](https://my.oschina.net/u/4087916/blog/4782741)|
|23|[mongodb内核源码实现、性能调优、最佳运维实践系列-记mongodb详细表级操作及详细时延统计实现原理(教你如何快速进行表级时延问题分析)](https://xie.infoq.cn/article/3184cdc42c26c86e2749c3e5c)|
|24|[mongodb内核源码实现、性能调优、最佳运维实践系列-Mongodb write写(增、删、改)模块设计与实现](https://my.oschina.net/u/4087916/blog/4974132)|
   
    
## 其他分享   
|#|内容|
|:-|:-|
|1|[阿里巴巴分布式消息队列中间件rocketmq-3.4.6源码分析](https://github.com/y123456yz/reading-and-annotate-rocketmq-3.4.6)|
|2|[服务器时延统计工具tcprstat,增加时延阈值统计，记录超过阈值的包个数，并把数据包时间戳记录到日志文件，这样可以根据时间戳快速定位到抓包文件中对应的包，从而可以快速定位到大时延包，避免了人肉搜索抓包文件，提高问题排查效率](https://github.com/y123456yz/tcprstat)|
|3|[linux内核网络协议栈源码阅读分析注释](https://github.com/y123456yz/Reading-and-comprehense-linux-Kernel-network-protocol-stack)|
|4|[docker-17.05.0源码中文注释详细分析](https://github.com/y123456yz/reading-and-annotate-docker-17.05.0)|
|5|[lxc源码详细注释分析](https://github.com/y123456yz/reading-and-annotate-lxc-1.0.9)|
|6|[source insight代码中文注释乱码、背景色等配置调整](https://github.com/y123456yz/middleware_development_learning/tree/master/source%20insight%20configure)|
|7|[linux内核协议栈TCP time_wait原理、优化、副作用](https://my.oschina.net/u/4087916/blog/3051356)|
|8|[为何需要对开源社区版本mongodb做二次开发，需要做哪些二次开发](https://github.com/y123456yz/middleware_development_learning/blob/master/%E7%AC%AC%E5%9B%9B%E9%98%B6%E6%AE%B5-mongodb%E6%95%B0%E6%8D%AE%E5%BA%93/development_mongodb.md)|
|9|[在线引流工具Tcpcopy原理、环境搭建、使用、采坑](https://my.oschina.net/u/4087916/blog/3064268)|
  
  
  
## 技术交流群  
对linux c/c++ nginx redis memcache twemproxy mongodb 中间件 存储引擎 分布式 高并发 高性能服务端等技术敢兴趣的同学可以加群： QQ交流群1(针对在校生)-(群号：568892619)      QQ交流群2(针对已工作)-(581089275)     

   
  
  