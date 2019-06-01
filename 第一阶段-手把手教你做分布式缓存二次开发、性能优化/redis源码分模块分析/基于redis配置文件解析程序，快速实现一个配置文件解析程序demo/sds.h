/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
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

#ifndef __SDS_H
#define __SDS_H

/*
 * 最大预分配长度
 */
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

/*
 * 类型别名，用于指向 sdshdr 的 buf 属性
 */
typedef char *sds;

/*
二进制安全

C 字符串中的字符必须符合某种编码（比如 ASCII）， 并且除了字符串的末尾之外， 字符串里面不能包含空字符， 否则最先被程序读入的空字符
将被误认为是字符串结尾 —— 这些限制使得 C 字符串只能保存文本数据， 而不能保存像图片、音频、视频、压缩文件这样的二进制数据。

举个例子， 如果有一种使用空字符来分割多个单词的特殊数据格式(Redis\0Cluster\0)， 那么这种格式就不能使用 C 字符串来保存， 因为C 
字符串所用的函数只会识别出其中的 "Redis" ， 而忽略之后的 "Cluster" 。

虽然数据库一般用于保存文本数据， 但使用数据库来保存二进制数据的场景也不少见， 因此， 为了确保 Redis 可以适用于各种不同的使用场景， 
SDS 的 API 都是二进制安全的（binary-safe）： 所有 SDS API 都会以处理二进制的方式来处理 SDS 存放在 buf 数组里的数据， 程序不会对
其中的数据做任何限制、过滤、或者假设 —— 数据在写入时是什么样的， 它被读取时就是什么样。

这也是我们将 SDS 的 buf 属性称为字节数组的原因 —— Redis 不是用这个数组来保存字符， 而是用它来保存一系列二进制数据。
*/

/*
表 2-1 C 字符串和 SDS 之间的区别

C 字符串                                                            SDS
获取字符串长度的复杂度为 O(N) 。                            获取字符串长度的复杂度为 O(1) 。 
API 是不安全的，可能会造成缓冲区溢出。                      API 是安全的，不会造成缓冲区溢出。 
修改字符串长度 N 次必然需要执行 N 次内存重分配。            修改字符串长度 N 次最多需要执行 N 次内存重分配。 
只能保存文本数据。                                          可以保存文本或者二进制数据。 
可以使用所有 <string.h> 库中的函数。                        可以使用一部分 <string.h> 库中的函数。 
*/

/*
表 2-2 SDS 的主要操作 API


函数                                    作用                                                时间复杂度


sdsnew           创建一个包含给定 C 字符串的 SDS 。                                     O(N) ， N 为给定 C 字符串的长度。 
sdsempty        创建一个不包含任何内容的空 SDS 。                                       O(1) 
sdsfree         释放给定的 SDS 。                                                       O(1) 
sdslen          返回 SDS 的已使用空间字节数。                                           这个值可以通过读取 SDS 的 len 属性来直接获得， 复杂度为 O(1) 。 
sdsavail            返回 SDS 的未使用空间字节数。                                       这个值可以通过读取 SDS 的 free 属性来直接获得， 复杂度为 O(1) 。 
sdsdup          创建一个给定 SDS 的副本（copy）。                                       O(N) ， N 为给定 SDS 的长度。 
sdsclear        清空 SDS 保存的字符串内容。                                             因为惰性空间释放策略，复杂度为 O(1) 。 
sdscat          将给定 C 字符串拼接到 SDS 字符串的末尾。                                O(N) ， N 为被拼接 C 字符串的长度。 
sdscatsds       将给定 SDS 字符串拼接到另一个 SDS 字符串的末尾。                        O(N) ， N 为被拼接 SDS 字符串的长度。 
sdscpy          将给定的 C 字符串复制到 SDS 里面， 覆盖 SDS 原有的字符串。              O(N) ， N 为被复制 C 字符串的长度。 
sdsgrowzero         用空字符将 SDS 扩展至给定长度。                                     O(N) ， N 为扩展新增的字节数。 
sdsrange        保留 SDS 给定区间内的数据， 不在区间内的数据会被覆盖或清除。            O(N) ， N 为被保留数据的字节数。 
sdstrim         接受一个 SDS 和一个 C 字符串作为参数， 从 SDS 左右两端分别移除
                所有在 C 字符串中出现过的字符。                                         O(M*N) ， M 为 SDS 的长度， N 为给定 C 字符串的长度。 
sdscmp          对比两个 SDS 字符串是否相同。                                           O(N) ， N 为两个 SDS 中较短的那个 SDS 的长度。 
*/

/*
空间预分配?
空间预分配用于优化 SDS 的字符串增长操作： 当 SDS 的 API 对一个 SDS 进行修改， 并且需要对 SDS 进行空间扩展的时候， 程序不仅会为 
SDS 分配修改所必须要的空间， 还会为 SDS 分配额外的未使用空间。

其中， 额外分配的未使用空间数量由以下公式决定：
如果对 SDS 进行修改之后， SDS 的长度（也即是 len 属性的值）将小于 1 MB ， 那么程序分配和 len 属性同样大小的未使用空间， 这时 
SDS len 属性的值将和 free 属性的值相同。 举个例子， 如果进行修改之后， SDS 的 len 将变成 13 字节， 那么程序也会分配 13 字节的
未使用空间， SDS 的 buf 数组的实际长度将变成 13 + 13 + 1 = 27 字节（额外的一字节用于保存空字符）。

如果对 SDS 进行修改之后， SDS 的长度将大于等于 1 MB ， 那么程序会分配 1 MB 的未使用空间。 举个例子， 如果进行修改之后， SDS 
的 len 将变成 30 MB ， 那么程序会分配 1 MB 的未使用空间， SDS 的 buf 数组的实际长度将为 30 MB + 1 MB + 1 byte 。

通过空间预分配策略， Redis 可以减少连续执行字符串增长操作所需的内存重分配次数。 
通过这种预分配策略， SDS 将连续增长 N 次字符串所需的内存重分配次数从必定 N 次降低为最多 N 次。 见sdscat
*/


/*
 * 保存字符串对象的结构
 */
struct sdshdr {//该结构一般分配过程可以参考createStringObject
    
    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[]; //可以参考sdsnewlen
};

/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);
size_t sdslen(const sds s);
sds sdsdup(const sds s);
void sdsfree(sds s);
size_t sdsavail(const sds s);
sds sdsgrowzero(sds s, size_t len);
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);
sds sdscatsds(sds s, const sds t);
sds sdscpylen(sds s, const char *t, size_t len);
sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...);
sds sdstrim(sds s, const char *cset);
void sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);
void sdsclear(sds s);
int sdscmp(const sds s1, const sds s2);
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void sdsfreesplitres(sds *tokens, int count);
void sdstolower(sds s);
void sdstoupper(sds s);
sds sdsfromlonglong(long long value);
sds sdscatrepr(sds s, const char *p, size_t len);
sds *sdssplitargs(const char *line, int *argc);
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
sds sdsjoin(char **argv, int argc, char *sep);

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);
void sdsIncrLen(sds s, int incr);
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSize(sds s);

#endif
