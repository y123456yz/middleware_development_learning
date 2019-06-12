/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */

/*
 * 双端链表节点
 */
typedef struct listNode {

    // 前置节点
    struct listNode *prev; //如果是list的头结点，则prev指向NULL

    // 后置节点
    struct listNode *next;//如果是list尾部结点，则next指向NULL

    // 节点的值
    void *value;

} listNode;

/*
 * 双端链表迭代器
 */
typedef struct listIter {

    // 当前迭代到的节点
    listNode *next;

    // 迭代的方向
    int direction; //取值AL_START_HEAD等

} listIter;


/*
链表和链表节点的 API

表 3-1 列出了所有用于操作链表和链表节点的 API 。


表 3-1 链表和链表节点 API

函数                                                        作用                                                时间复杂度

listSetDupMethod    将给定的函数设置为链表的节点值复制函数。                                O(1) 。 
listGetDupMethod    返回链表当前正在使用的节点值复制函数。                  复制函数可以通过链表的 dup 属性直接获得， O(1) 
listSetFreeMethod   将给定的函数设置为链表的节点值释放函数。                 O(1) 。 
listGetFree         返回链表当前正在使用的节点值释放函数。                  释放函数可以通过链表的 free 属性直接获得， O(1) 
listSetMatchMethod  将给定的函数设置为链表的节点值对比函数。                 O(1) 
listGetMatchMethod  返回链表当前正在使用的节点值对比函数。                  对比函数可以通过链表的 match 属性直接获得， O(1) 
listLength          返回链表的长度（包含了多少个节点）。                    链表长度可以通过链表的 len 属性直接获得， O(1) 。 
listFirst           返回链表的表头节点。                                    表头节点可以通过链表的 head 属性直接获得， O(1) 。 
listLast            返回链表的表尾节点。                                    表尾节点可以通过链表的 tail 属性直接获得， O(1) 。 
listPrevNode        返回给定节点的前置节点。                                前置节点可以通过节点的 prev 属性直接获得， O(1) 。 
listNextNode        返回给定节点的后置节点。                                后置节点可以通过节点的 next 属性直接获得， O(1) 。 
listNodeValue       返回给定节点目前正在保存的值。                          节点值可以通过节点的 value 属性直接获得， O(1) 。 
listCreate          创建一个不包含任何节点的新链表。                         O(1) 
listAddNodeHead     将一个包含给定值的新节点添加到给定链表的表头。           O(1) 
listAddNodeTail     将一个包含给定值的新节点添加到给定链表的表尾。           O(1) 
listInsertNode      将一个包含给定值的新节点添加到给定节点的之前或者之后。   O(1) 
listSearchKey       查找并返回链表中包含给定值的节点。                       O(N) ， N 为链表长度。 
listIndex           返回链表在给定索引上的节点。                             O(N) ， N 为链表长度。 
listDelNode         从链表中删除给定节点。                                   O(1) 。 
listRotate          将链表的表尾节点弹出，然后将被弹出的节点插入到链表
                    的表头， 成为新的表头节点。                              O(1) 
listDup             复制一个给定链表的副本。                                 O(N) ， N 为链表长度。 
listRelease         释放给定链表，以及链表中的所有节点。                      O(N) ， N 为链表长度。 
*/


/*
 dup 函数用于复制链表节点所保存的值；
 free 函数用于释放链表节点所保存的值；
 match 函数则用于对比链表节点所保存的值和另一个输入值是否相等。

 Redis 的链表实现的特性可以总结如下：
 ?双端： 链表节点带有 prev 和 next 指针， 获取某个节点的前置节点和后置节点的复杂度都是 O(1) 。
 ?无环： 表头节点的 prev 指针和表尾节点的 next 指针都指向 NULL ， 对链表的访问以 NULL 为终点。
 ?带表头指针和表尾指针： 通过 list 结构的 head 指针和 tail 指针， 程序获取链表的表头节点和表尾节点的复杂度为 O(1) 。
 ?带链表长度计数器： 程序使用 list 结构的 len 属性来对 list 持有的链表节点进行计数， 程序获取链表中节点数量的复杂度为 O(1) 。
 ?多态： 链表节点使用 void* 指针来保存节点值， 并且可以通过 list 结构的 dup 、 free 、 match 三个属性为节点值设置类型特定函数， 所以链表可以用于保存各种不同类型的值。

 */ //listCreate创建链表

/*
 * 双端链表结构
 */
typedef struct list {

    // 表头节点
    listNode *head;

    // 表尾节点
    listNode *tail;

    // 节点值复制函数
    void *(*dup)(void *ptr);

    // 节点值释放函数
    void (*free)(void *ptr);

    // 节点值对比函数
    int (*match)(void *ptr, void *key);

    // 链表所包含的节点数量
    unsigned long len;

} list;

/* Functions implemented as macros */
// 返回给定链表所包含的节点数量
// T = O(1)
#define listLength(l) ((l)->len)
// 返回给定链表的表头节点
// T = O(1)
#define listFirst(l) ((l)->head)
// 返回给定链表的表尾节点
// T = O(1)
#define listLast(l) ((l)->tail)
// 返回给定节点的前置节点
// T = O(1)
#define listPrevNode(n) ((n)->prev)
// 返回给定节点的后置节点
// T = O(1)
#define listNextNode(n) ((n)->next)
// 返回给定节点的值
// T = O(1)
#define listNodeValue(n) ((n)->value)

// 将链表 l 的值复制函数设置为 m
// T = O(1)
#define listSetDupMethod(l,m) ((l)->dup = (m))
// 将链表 l 的值释放函数设置为 m
// T = O(1)
#define listSetFreeMethod(l,m) ((l)->free = (m))
// 将链表的对比函数设置为 m
// T = O(1)
#define listSetMatchMethod(l,m) ((l)->match = (m))

// 返回给定链表的值复制函数
// T = O(1)
#define listGetDupMethod(l) ((l)->dup)
// 返回给定链表的值释放函数
// T = O(1)
#define listGetFree(l) ((l)->free)
// 返回给定链表的值对比函数
// T = O(1)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void);
void listRelease(list *list);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotate(list *list);

/* Directions for iterators 
 *
 * 迭代器进行迭代的方向
 */
// 从表头向表尾进行迭代
#define AL_START_HEAD 0
// 从表尾到表头进行迭代
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
