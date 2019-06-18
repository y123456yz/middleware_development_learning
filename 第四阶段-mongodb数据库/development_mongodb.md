      Mongodb数据库版本包含企业版本和社区版本，他们的区别是企业版本相比有更多功能，使用企业版本必须购买付费，所以mongodb部分核心功能没有开源。为了增强mongodb集群稳定性，企业需要对开源版本内核进行二次开发，主要包括以下功能模块的开发(增加以下功能后，会有很好的收益)：

1.  各种操作时延统计。
2.  Mongos代理慢日志记录。
3.  普通用户权限细化控制，危险操作过滤。
4.  危险操作日志审计。
5.  所有增删改操作sql异步日志记录。
6.  mongodb增加热备功能。  
......
  

   如果对mongodb数据库和wiredtiger存储引擎源码感兴趣，可以参考如下注释:

[https://github.com/y123456yz/reading-and-annotate-mongodb-3.6.1](https://github.com/y123456yz/reading-and-annotate-mongodb-3.6.1)