#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

#include "ae.h"
#include "anet.h"

#define REDIS_SERVERPORT        6379    /* TCP port */
#define REDIS_MAX_QUERYBUF_LEN  10240
#define REDIS_TCP_BACKLOG       511     /* TCP listen backlog */
#define REDIS_IP_STR_LEN        INET6_ADDRSTRLEN
//错误信息
char g_neterror_buf[1024];

//epoll事件循环机制
aeEventLoop *g_epoll_loop = NULL;

/*
定时时间到
*/
int MainTimerExpire(struct aeEventLoop *eventLoop, long long id, void *clientData)
{
    printf("MainTimerExpire\n");

    //15秒后再次执行该函数
    return 15000;
}

void MainCloseFd(aeEventLoop *el, int fd)
{

    //删除结点，关闭文件
    aeDeleteFileEvent(el, fd, AE_READABLE);
    close(fd);
}

/*
新链接fd的数据读写
*/
void MainReadFromClient(aeEventLoop *el, int fd, void *privdata, int mask)
{
    char buffer[REDIS_MAX_QUERYBUF_LEN] = { 0 };
    int nread, nwrite;
    
    char client_ip[REDIS_IP_STR_LEN];
    int client_port;
    anetPeerToString(fd, client_ip, REDIS_IP_STR_LEN, &client_port);

    
    nread = read(fd, buffer, REDIS_MAX_QUERYBUF_LEN);
    /* 该fd对应的协议栈buf没有数据可读 */
    if (nread == -1 && errno == EAGAIN) return; /* No more data ready. */

    //说明客户端关闭了链接
    if(nread <= 0) { 
        printf("I/O error reading from node link: %s",
                (nread == 0) ? "connection closed" : strerror(errno));
        MainCloseFd(el, fd);
    } else { //读数据正常，则把读到的数据返回给客户端，也就是客户端会收到发送的数据
        printf("recv from client %s:%d, data:%s\r\n", client_ip, client_port, buffer);
        nwrite = write(fd, buffer, REDIS_MAX_QUERYBUF_LEN);
        if(nwrite == -1) //写异常了，可能是客户端关闭了链接，也可能是客户端进程挂了等
            MainCloseFd(el, fd);
    }
}

/*
处理客户端链接
*/
void MainAcceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask)
{
    int cfd, port;
    char ip_addr[128] = { 0 };
    cfd = anetTcpAccept(g_neterror_buf, fd, ip_addr, sizeof(ip_addr), &port);
    if (cfd == ANET_ERR) { //accept操作异常
        if (errno != EWOULDBLOCK)
            printf("Accepting client connection: %s", g_neterror_buf);
        return;
    }
    printf("client %s:%d Connected\n", ip_addr, port);
    
    // 非阻塞
    anetNonBlock(NULL, cfd);
    // 禁用 Nagle 算法
    anetEnableTcpNoDelay(NULL, cfd);
    
    //把accept返回的新套接字cfd注册到epoll事件集中，该新套接字关注AE_READABLE读事件，如果客户端有数据过来将会
    //触发读回调函数MainReadFromClient
    if(aeCreateFileEvent(el, cfd, AE_READABLE,
        MainReadFromClient, NULL) == AE_ERR ) {
        fprintf(stderr, "client connect fail: %d\n", cfd);
        close(cfd);
    }
}

int main()
{

    printf("process Start begin\n");

    //初始化EPOLL事件处理器状态
    g_epoll_loop = aeCreateEventLoop(1024*10);

    //创建套接字并bind
    int fd = anetTcpServer(g_neterror_buf, REDIS_SERVERPORT, NULL, REDIS_TCP_BACKLOG);
    if( ANET_ERR == fd ) {
        fprintf(stderr, "Open port %d error: %s\n", REDIS_SERVERPORT, g_neterror_buf);
        exit(1);
    }
    
    anetNonBlock(NULL, fd);
    anetSetReuseAddr(NULL, fd);
    if(aeCreateFileEvent(g_epoll_loop, fd, AE_READABLE,  MainAcceptTcpHandler, NULL) == AE_ERR ) {
        fprintf(stderr, "aeCreateFileEvent failed");
        exit(1);
    }
    
    //设置定时器
    aeCreateTimeEvent(g_epoll_loop, 1, MainTimerExpire, NULL, NULL);

    //开启事件循环
    aeMain(g_epoll_loop);

    //删除事件循环
    aeDeleteEventLoop(g_epoll_loop);

    printf("process End\n");

    return 0;
}