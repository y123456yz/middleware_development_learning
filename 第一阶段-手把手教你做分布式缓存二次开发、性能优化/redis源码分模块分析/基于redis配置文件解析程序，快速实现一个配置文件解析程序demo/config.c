/* Configuration file parsing and CONFIG GET/SET commands implementation.
 *
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "zmalloc.h"

struct redisServer server;

/*-----------------------------------------------------------------------------
 * Config file parsing
 *----------------------------------------------------------------------------*/

int yesnotoi(char *s) {
    if (!strcasecmp(s,"yes")) return 1;
    else if (!strcasecmp(s,"no")) return 0;
    else return -1;
}

void loadServerConfigFromString(char *config) {
    char *err = NULL;
    int linenum = 0, totlines, i;
    int slaveof_linenum = 0;
    sds *lines;

    lines = sdssplitlen(config,strlen(config),"\n",1,&totlines);

    for (i = 0; i < totlines; i++) {
        sds *argv;
        int argc;

        linenum = i+1;
        // 移除字符串的前置空白和后缀空白
        lines[i] = sdstrim(lines[i]," \t\r\n");

        /* Skip comments and blank lines */
        // 跳过空白行
        if (lines[i][0] == '#' || lines[i][0] == '\0') continue;

        /* Split into arguments */
        // 将字符串分割成多个参数
        argv = sdssplitargs(lines[i],&argc);
        if (argv == NULL) {
            err = "Unbalanced quotes in configuration line";
            goto loaderr;
        }

        /* Skip this line if the resulting command vector is empty. */
        // 跳过空白参数
        if (argc == 0) {
            sdsfreesplitres(argv,argc);
            continue;
        }

        // 将选项名字转换成小写
        // 例如 TIMEOUT 转换成 timeout
        sdstolower(argv[0]);

        /* Execute config directives */
        if (!strcasecmp(argv[0],"maxidletime") && argc == 2) {
            server.maxidletime = atoi(argv[1]);
            if (server.maxidletime < 0) {
                err = "Invalid timeout value"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"port") && argc == 2) {
            server.port = atoi(argv[1]);
            if (server.port < 0 || server.port > 65535) {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"appendfsync") && argc == 2) {
            if (!strcasecmp(argv[1],"no")) {
                server.aof_fsync = AOF_FSYNC_NO;
            } else if (!strcasecmp(argv[1],"always")) {
                server.aof_fsync = AOF_FSYNC_ALWAYS;
            } else if (!strcasecmp(argv[1],"everysec")) {
                server.aof_fsync = AOF_FSYNC_EVERYSEC;
            } else {
                err = "argument must be 'no', 'always' or 'everysec'";
                goto loaderr;
            }
        }
    }

    sdsfreesplitres(lines,totlines);
    return;

loaderr:
    fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");
    fprintf(stderr, "Reading the configuration file, at line %d\n", linenum);
    fprintf(stderr, ">>> '%s'\n", lines[i]);
    fprintf(stderr, "%s\n", err);
    exit(1);
}

/* Load the server configuration from the specified filename.
 *
 * 从给定文件中载入服务器配置。
 *
 * The function appends the additional configuration directives stored
 * in the 'options' string to the config file before loading.
 *
 * options 字符串会被追加到文件所载入的内容的后面。
 *
 * Both filename and options can be NULL, in such a case are considered
 * empty. This way loadServerConfig can be used to just load a file or
 * just load a string. 
 *
 * filename 和 options 都可以是 NULL ，在这种情况下，
 * 服务器配置文件视为空文件。
 */ //先initServerConfig，后loadServerConfig
void loadServerConfig(char *filename) {
    sds config = sdsempty();
    char buf[REDIS_CONFIGLINE_MAX+1];

    /* Load the file content */
    // 载入文件内容
    if (filename) {
        FILE *fp;

        if (filename[0] == '-' && filename[1] == '\0') {
            fp = stdin;
        } else {
            if ((fp = fopen(filename,"r")) == NULL) {
                printf("Fatal error, can't open config file '%s'", filename);
                exit(1);
            }
        }
        while(fgets(buf,REDIS_CONFIGLINE_MAX+1,fp) != NULL)
            config = sdscat(config,buf);
        if (fp != stdin) fclose(fp);
    }

    // 根据字符串内容，设置服务器配置
    loadServerConfigFromString(config);

    sdsfree(config);
}

sds getAbsolutePath(char *filename) {
    char cwd[1024];
    sds abspath;
    sds relpath = sdsnew(filename);

    relpath = sdstrim(relpath," \r\n\t");
    if (relpath[0] == '/') return relpath; /* Path is already absolute. */

    /* If path is relative, join cwd and relative path. */
    if (getcwd(cwd,sizeof(cwd)) == NULL) {
        sdsfree(relpath);
        return NULL;
    }
    abspath = sdsnew(cwd);
    if (sdslen(abspath) && abspath[sdslen(abspath)-1] != '/')
        abspath = sdscat(abspath,"/");

    /* At this point we have the current path always ending with "/", and
     * the trimmed relative path. Try to normalize the obvious case of
     * trailing ../ elements at the start of the path.
     *
     * For every "../" we find in the filename, we remove it and also remove
     * the last element of the cwd, unless the current cwd is "/". */
    while (sdslen(relpath) >= 3 &&
           relpath[0] == '.' && relpath[1] == '.' && relpath[2] == '/')
    {
        sdsrange(relpath,3,-1);
        if (sdslen(abspath) > 1) {
            char *p = abspath + sdslen(abspath)-2;
            int trimlen = 1;

            while(*p != '/') {
                p--;
                trimlen++;
            }
            sdsrange(abspath,0,-(trimlen+1));
        }
    }

    /* Finally glue the two parts together. */
    abspath = sdscatsds(abspath,relpath);
    sdsfree(relpath);
    return abspath;
}

void printfRedisConfig(void)
{
    printf("maxidletime:%d\r\n", server.maxidletime);
    printf("port:%d\r\n", server.maxidletime);
    if(server.aof_fsync == AOF_FSYNC_NO) {
        printf("appendfsync:no\r\n");
    } else if(server.aof_fsync == AOF_FSYNC_ALWAYS) {
        printf("appendfsync:always\r\n");
    } else if(server.aof_fsync == AOF_FSYNC_EVERYSEC) {
        printf("appendfsync:everysec\r\n");
    } else {
        printf("appendfsync config failed, error\r\n");
    }
}

