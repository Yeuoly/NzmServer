#ifndef __NZM_THREAD_H
#define __NZM_THREAD_H

#include <queue>
#include <pthread.h>
#include "./nzm-task.h"

using namespace std;

class ThreadPool {
public:
    ThreadPool(){};

    ThreadPool(int default_thread_count = 10);

    bool AppendTask(Task *task);

protected:
    int CreateThreads();

    int DestoryThreads();

    static void *ThreadCallbackFunc(void *lp);
private:
    static deque<Task *>    task_queue;
    int                     thread_count;
    pthread_t               *thread_ary;
    static pthread_mutex_t  thread_mutex;
    static pthread_cond_t   thread_cond;
    static bool             shutdown;
};

#endif