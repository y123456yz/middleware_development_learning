#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc,char *argv[]){
	int sd;
	struct sockaddr_in serv_addr;
	int str_len;

    char message[]="block test message";
	if(argc != 3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	sd = socket(PF_INET,SOCK_STREAM,0);
	if(sd == -1){
		error_handling("socket() error");
	}
	
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

    sleep(10);
    printf("begin connect\r\n");
	if(connect(sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
		error_handling("connect() error\r\n");
	}
	printf("end connect\r\n\r\n");
	

    sleep(10);
    printf("begin send message\r\n");
	write(sd, message, sizeof(message));
	printf("end send message:%s to server\r\n", message);
	
    sleep(10);
	close(sd);
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputs("\n",stderr);
	exit(1);
}
