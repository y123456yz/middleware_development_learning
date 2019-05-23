#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h>


void error_handling(char *message);


/*
 * 设置 socket 的最大发送 buffer 字节数
 */
int anetSetSendBuffer(int fd, int buffsize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1)
    {
        printf("set send buf failed\r\n");
        return -1;
    }
    return 0;
}


int main(int argc, char *argv[]){
	int sd;
	int fd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	int str_len;
	int write_len = 1024 * 1024 * 10;
	int ret_len = 0;
	char *write_str = malloc(write_len);
	
	char message[1000];
	if(argc != 2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	sd = socket(PF_INET,SOCK_STREAM,0);
	if(sd == -1){
		error_handling("socket() error");
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(sd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
		error_handling("bind() error");
	}

	if(listen(sd,5) == -1){
		error_handling("listen() error");
	}
    
	clnt_addr_size = sizeof(clnt_addr);
	printf("begin accept\r\n");
	fd = accept(sd,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
	if(fd == -1){
		error_handling("accept() error");
	}
    printf("accept successful from client\r\n\r\n");


	printf("begin recv message\r\n");
    str_len = read(fd, message, sizeof(message)-1);
    if(str_len == -1)
    {
        error_handling("read() error");
    }
    printf("recv message:%s from client\r\n\r\n",message);

    //这里设置需要发送的数据write_len很大，同时设置该链接的内核协议栈发送缓冲区空间比较小，这样缓存区就没有那么多空间来拷贝
    //write期望发送的数据，就会阻塞等待，直到客户端read接收数据后，内核腾出部分发送缓冲区空间，然后继续拷贝，直到拷贝完成，才会
    //返回。或者超过默认的超时时间才会返回

    //这里有一个小技巧，就是客户端不要去读该链接的数据，保证该链接服务端内核发送缓冲区填满，这样就可以模拟write阻塞现象

    //注意这里如果设置协议栈发送缓冲区比write想要发送的数据长度大，则会立马返回，体会不到阻塞现象，因为缓存区空间足够
    anetSetSendBuffer(fd, 10*1024);
	printf("begin write message\r\n");
	ret_len= write(fd, write_str, write_len);
	printf("end write message, write len:%d\r\n", ret_len);

	sleep(1);
	close(fd);
	close(sd);

	return 0;

}

void error_handling(char *message){

	fputs(message,stderr);
	fputs("\n",stderr);
	exit(1);
}
