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


#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("error, please run with config file\r\n");
        exit(1);
    }

    char *configfile = argv[1];
    printf("config file name:%s\r\n", configfile);
    
    configfile = getAbsolutePath(configfile);
    if (configfile == NULL) {
        printf("error, getAbsolutePath\r\n");
        exit(1);
    }

    loadServerConfig(configfile);
    printfRedisConfig();
}
