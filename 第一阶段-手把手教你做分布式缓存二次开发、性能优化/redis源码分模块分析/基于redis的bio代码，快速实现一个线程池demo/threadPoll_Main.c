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
#include <syslog.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <execinfo.h>

#include "bio.h"

int task1RunFunc(void *arg1, void *arg2, void *arg3){
    printf("task1RunFunc run, %s %s %s\r\n", (char*)arg1, (char*)arg2, (char*)arg3);
    //通过控制这里的时延可以控制线程池消费任务的速度
    usleep(100000); //这里通过延时来模拟耗时工作, 业务逻辑自己通过这里来替换sleep
}

int task2RunFunc(void *arg1, void *arg2, void *arg3){
    int num1 = 
    printf("task2RunFunc run, %d %d %d\r\n", *(int*)arg1, *(int*)arg2, *(int*)arg3);
    //通过控制这里的时延可以控制线程池消费任务的速度
    usleep(100000); //这里通过延时来模拟耗时工作, 业务逻辑自己通过这里来替换sleep
}

int main(int argc, char **argv) {
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;

    bioInit();
    while(1) {
        //任务一放入task1线程池中执行
        if(bioCreateBackgroundJob(BIO_TASK1, "arg1", "arg2", "arg3", task1RunFunc) != 0) {
            printf("task1 is too much\r\n");
            goto task2;
        }

    task2:
        //任务二放入task2线程池中执行
        if(bioCreateBackgroundJob(BIO_TASK2, &num1, &num2, &num3, task2RunFunc) != 0) {
            printf("task2 is too much\r\n");
        }
        usleep(100000); //通过调整这里的时延，可以控制生成任务的速度
    }

    bioKillThreads();
    return 0;
}


