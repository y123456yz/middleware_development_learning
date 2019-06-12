/*
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

#include <time.h>

typedef int (*taskRunFunc)(void *arg1, void *arg2, void *arg3);

/* This structure represents a background Job. It is only used locally to this
 * file as the API deos not expose the internals at all.
 *
 * 表示后台任务的数据结构
 *
 * 这个结构只由 API 使用，不会被暴露给外部。
 */

struct bio_job {

    // 任务创建时的时间
    time_t time; /* Time at which the job was created. */

    /* Job specific arguments pointers. If we need to pass more than three
     * arguments we can just pass a pointer to a structure or alike. 
     *
     * 任务的参数。参数多于三个时，可以传递数组或者结构
     */
    void *arg1, *arg2, *arg3;

    taskRunFunc func;
};

/* Exported API */
void bioInit(void);
int bioCreateBackgroundJob(int type, void *arg1, void *arg2, void *arg3, taskRunFunc func);
unsigned long long bioPendingJobsOfType(int type);
void bioWaitPendingJobsLE(int type, unsigned long long num);
//time_t bioOlderJobOfType(int type);
void bioKillThreads(void);
void *bioProcessBackgroundJobs(void *arg);

//任务队列上限，如果超过这么多，说明任务积压很严重
#define BIO_MAX_PENDING_NUM  1000

/* Background job opcodes */
//这里有两组线程池，分别做不同的事情
#define BIO_TASK1     0   //任务1对应的线程池
#define BIO_TASK2     1    //任务2对应的线程池
#define BIO_NUM_OPS   2

#define BIO_TASK_THREAD_NUM     5   //每个线程池中线程数


