
#ifndef MIDDLEWARE_SERVER_H
#define MIDDLEWARE_SERVER_H

#define MIDDLEWARE_REPLY_CHUNK_BYTES (16*1024) /* 16k output buffer */
#define MIDDLEWARE_BINDADDR_MAX 16

#define MIDDLEWARE_SERVERPORT        6379    /* TCP port */
#define MIDDLEWARE_MAX_QUERYBUF_LEN  10240
#define MIDDLEWARE_TCP_BACKLOG       511     /* TCP listen backlog */
#define MIDDLEWARE_IP_STR_LEN        INET6_ADDRSTRLEN
#define MIDDLEWARE_MIN_RESERVED_FDS     32
#define MIDDLEWARE_EVENTLOOP_FDSET_INCR (REDIS_MIN_RESERVED_FDS+96)
#define MIDDLEWARE_MAX_CLIENTS          10000

struct middlewareServer {
    // 配置文件的绝对路径
    char *configfile;           /* Absolute config file path, or NULL */
    // 事件状态
    aeEventLoop *el;
    
    int port;                   /* TCP listening port */
    int tcp_backlog;            /* TCP listen() backlog */

    // 地址
    char *bindaddr[MIDDLEWARE_BINDADDR_MAX]; /* Addresses we should bind to */
    // 地址数量
    int bindaddr_count;         /* Number of addresses in server.bindaddr[] */

    // 一个链表，保存了所有客户端状态结构  createClient中把redisClient客户端添加到该服务端链表clients链表中  if (fd != -1) listAddNodeTail(server.clients,c);
    list *clients;               /* List of active clients */
    // 链表，保存了所有待关闭的客户端
    list *clients_to_close;     /* Clients to close asynchronously */

    // 是否开启 SO_KEEPALIVE 选项  tcp-keepalive 设置，默认不开启
    int tcpkeepalive;               /* Set SO_KEEPALIVE if non-zero. */

    int daemonize;                  /* True if running as a daemon */

    /* Logging */
    char *logfile;                  /* Path of log file */
    int syslog_enabled;             /* Is syslog enabled? */
    char *syslog_ident;             /* Syslog ident */
    int syslog_facility;            /* Syslog facility */

    /* Limits */
    int maxclients;                 /* Max number of simultaneous clients */
}

typedef struct middlewareClient {   //redisServer与redisClient对应
    // 套接字描述符
    int fd;
    char cip[REDIS_IP_STR_LEN];
    int cport;

    // 查询缓冲区  默认空间大小REDIS_IOBUF_LEN，见readQueryFromClient
    sds querybuf;//解析出的参数存入下面的argc和argv中   

    // 参数数量
    int argc;   //客户端命令解析见processMultibulkBuffer   //注意slowlog最多纪录32个参数，见slowlogCreateEntry

    // 参数对象数组  resetClient->freeClientArgv中释放空间
    robj **argv; //客户端命令解析见processMultibulkBuffer  创建空间和赋值见processMultibulkBuffer，有多少个参数数量multibulklen，就创建多少个robj(redisObject)存储参数的结构


    // 请求的类型：内联命令还是多条命令
    int reqtype;

    /* Response buffer */
    // 回复偏移量
    int bufpos;
    // 回复缓冲区
    char buf[MIDDLEWARE_REPLY_CHUNK_BYTES];
}

#endif



