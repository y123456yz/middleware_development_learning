/* Global vars */

#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <locale.h>

#include "middleware_server.h"


struct middlewareServer server; /* server global state */

/* This function will try to raise the max number of open files accordingly to
 * the configured max number of clients. It also reserves a number of file
 * descriptors (REDIS_MIN_RESERVED_FDS) for extra operations of
 * persistence, listening sockets, log files and so forth.
 *
 * If it will not be possible to set the limit accordingly to the configured
 * max number of clients, the function will do the reverse setting
 * server.maxclients to the value that we can actually handle. */
void adjustOpenFilesLimit(void) {
    rlim_t maxfiles = server.maxclients+REDIS_MIN_RESERVED_FDS;
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE,&limit) == -1) {
        redisLog(REDIS_WARNING,"Unable to obtain the current NOFILE limit (%s), assuming 1024 and setting the max clients configuration accordingly.",
            strerror(errno));
        server.maxclients = 1024-REDIS_MIN_RESERVED_FDS;
    } else {
        rlim_t oldlimit = limit.rlim_cur;

        /* Set the max number of files if the current limit is not enough
         * for our needs. */
        if (oldlimit < maxfiles) {
            rlim_t f;
            int setrlimit_error = 0;

            /* Try to set the file limit to match 'maxfiles' or at least
             * to the higher value supported less than maxfiles. */
            f = maxfiles;
            while(f > oldlimit) {
                int decr_step = 16;

                limit.rlim_cur = f;
                limit.rlim_max = f;
                if (setrlimit(RLIMIT_NOFILE,&limit) != -1) break;
                setrlimit_error = errno;

                /* We failed to set file limit to 'f'. Try with a
                 * smaller limit decrementing by a few FDs per iteration. */
                if (f < decr_step) break;
                f -= decr_step;
            }

            /* Assume that the limit we get initially is still valid if
             * our last try was even lower. */
            if (f < oldlimit) f = oldlimit;

            if (f != maxfiles) {
                int old_maxclients = server.maxclients;
                server.maxclients = f-REDIS_MIN_RESERVED_FDS;
                if (server.maxclients < 1) {
                    redisLog(REDIS_WARNING,"Your current 'ulimit -n' "
                        "of %llu is not enough for Redis to start. "
                        "Please increase your open file limit to at least "
                        "%llu. Exiting.",
                        (unsigned long long) oldlimit,
                        (unsigned long long) maxfiles);
                    exit(1);
                }
                redisLog(REDIS_WARNING,"You requested maxclients of %d "
                    "requiring at least %llu max file descriptors.",
                    old_maxclients,
                    (unsigned long long) maxfiles);
                redisLog(REDIS_WARNING,"Redis can't set maximum open files "
                    "to %llu because of OS error: %s.",
                    (unsigned long long) maxfiles, strerror(setrlimit_error));
                redisLog(REDIS_WARNING,"Current maximum open files is %llu. "
                    "maxclients has been reduced to %d to compensate for "
                    "low ulimit. "
                    "If you need higher maxclients increase 'ulimit -n'.",
                    (unsigned long long) oldlimit, server.maxclients);
            } else {
                redisLog(REDIS_NOTICE,"Increased maximum number of open files "
                    "to %llu (it was originally set to %llu).",
                    (unsigned long long) maxfiles,
                    (unsigned long long) oldlimit);
            }
        }
    }
}

/* Low level logging. To use only for very big messages, otherwise
 * redisLog() is to prefer. */
void redisLogRaw(int level, const char *msg) { //日志这里是打开文件，写入，然后关闭，这里应该可以优化，只需打开一次即可
    const int syslogLevelMap[] = { LOG_DEBUG, LOG_INFO, LOG_NOTICE, LOG_WARNING };
    const char *c = ".-*#";
    FILE *fp;
    char buf[64];
    int rawmode = (level & REDIS_LOG_RAW);
    int log_to_stdout = server.logfile[0] == '\0';

    level &= 0xff; /* clear flags */
    if (level < server.verbosity) return;

    fp = log_to_stdout ? stdout : fopen(server.logfile,"a");
    if (!fp) return;

    if (rawmode) {
        fprintf(fp,"%s",msg);
    } else {
        int off;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        off = strftime(buf,sizeof(buf),"%d %b %H:%M:%S.",localtime(&tv.tv_sec));
        snprintf(buf+off,sizeof(buf)-off,"%03d",(int)tv.tv_usec/1000);
        fprintf(fp,"[%d] %s %c %s\n",(int)getpid(),buf,c[level],msg);
    }
    fflush(fp);

    if (!log_to_stdout) fclose(fp);
    if (server.syslog_enabled) syslog(syslogLevelMap[level], "%s", msg);
}

/* Like redisLogRaw() but with printf-alike support. This is the function that
 * is used across the code. The raw version is only used in order to dump
 * the INFO output on crash. */
void redisLog(int level, const char *fmt, ...) {
    va_list ap;
    char msg[REDIS_MAX_LOGMSG_LEN];

    if ((level&0xff) < server.verbosity) return;
    
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    redisLogRaw(level,msg);
}

//先initServerConfig，后loadServerConfig
void initServerConfig() { 
    int j;
    
    // 设置默认配置文件路径
    server.configfile = NULL;
    // 设置默认服务器频率
    server.hz = REDIS_DEFAULT_HZ;
    // 为运行 ID 加上结尾字符
    server.runid[REDIS_RUN_ID_SIZE] = '\0';
    // 设置服务器的运行架构
    server.arch_bits = (sizeof(long) == 8) ? 64 : 32;
    // 设置默认服务器端口号
    server.port = REDIS_SERVERPORT;
    server.tcp_backlog = REDIS_TCP_BACKLOG;
    server.bindaddr_count = 0;

    server.ipfd_count = 0;
    server.sofd = -1;
    server.dbnum = REDIS_DEFAULT_DBNUM;
    server.verbosity = REDIS_DEFAULT_VERBOSITY;
    server.maxidletime = REDIS_MAXIDLETIME;
    server.tcpkeepalive = REDIS_DEFAULT_TCP_KEEPALIVE;
    
    server.clients = listCreate();
    server.clients_to_close = listCreate();

    adjustOpenFilesLimit();
    server.el = aeCreateEventLoop(server.maxclients+REDIS_EVENTLOOP_FDSET_INCR);

    /* Open the TCP listening socket for the user commands. */
    // 打开 TCP 监听端口，用于等待客户端的命令请求
    if (server.port != 0 &&
        listenToPort(server.port,server.ipfd,&server.ipfd_count) == REDIS_ERR)
        exit(1);

    /* Abort if there are no listening sockets at all. */
    if (server.ipfd_count == 0 && server.sofd < 0) {
        redisLog(REDIS_WARNING, "Configured to not listen anywhere, exiting.");
        exit(1);
    }

    /* Create the serverCron() time event, that's our main way to process
     * background operations. */
    // 为 serverCron() 创建时间事件
    if(aeCreateTimeEvent(server.el, 1, serverCron, NULL, NULL) == AE_ERR) {
        redisPanic("Can't create the serverCron time event.");
        exit(1);
    }

    /* Create an event handler for accepting new connections in TCP and Unix
     * domain sockets. */
    // 为 TCP 连接关联连接应答（accept）处理器
    // 用于接受并应答客户端的 connect() 调用
    for (j = 0; j < server.ipfd_count; j++) {
        if (aeCreateFileEvent(server.el, server.ipfd[j], AE_READABLE,
            acceptTcpHandler,NULL) == AE_ERR)
            {
                redisPanic(
                    "Unrecoverable error creating server.ipfd file event.");
            }
    }

    // 初始化 BIO 系统
    bioInit();
}

int main(int argc, char **argv) {
{

    printf("process Start begin\n");

    //初始化EPOLL事件处理器状态
    g_epoll_loop = aeCreateEventLoop(REDIS_MAX_CLIENTS + REDIS_EVENTLOOP_FDSET_INCR);

    //创建套接字并bind
    int sd = anetTcpServer(g_neterror_buf, REDIS_SERVERPORT, NULL, REDIS_TCP_BACKLOG);
    if( ANET_ERR == sd ) {
        fprintf(stderr, "Open port %d error: %s\n", REDIS_SERVERPORT, g_neterror_buf);
        exit(1);
    }

    //设置socket bind对应的fd为非阻塞
    anetNonBlock(NULL, sd);
    //某些情况下，服务端进程退出后，会有监听端口timwait状态的连接，这时候如果重启进程，会提示error:98，Address already in use，就可以用该设置来解决该问题。
    anetSetReuseAddr(NULL, sd);
    if(aeCreateFileEvent(g_epoll_loop, sd, AE_READABLE,  MainAcceptTcpHandler, NULL) == AE_ERR ) {
        fprintf(stderr, "aeCreateFileEvent failed");
        exit(1);
    }
    
    //设置定时器
    aeCreateTimeEvent(g_epoll_loop, 1, MainTimerExpire, NULL, NULL);
    //在aeMain循环中，优先运行该回调
    aeSetBeforeSleepProc(g_epoll_loop, MainPriorityRun);
    //开启事件循环
    aeMain(g_epoll_loop);

    //删除事件循环
    aeDeleteEventLoop(g_epoll_loop);

    printf("process End\n");

    return 0;
}



