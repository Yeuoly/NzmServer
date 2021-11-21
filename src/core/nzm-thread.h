#ifndef __NZM_THREAD_H
#define __NZM_THREAD_H

#include <queue>
#include <pthread.h>
#include <signal.h>
#include "./nzm-task.h"

using namespace std;

#define NZM_SIG_RESTART 20

class ThreadPool {
public:
    ThreadPool(){};

    ThreadPool(int default_thread_count = 10);

    static bool AppendTask(Task *task);

    static bool RestartThread(pthread_t pid);
protected:
    int CreateThreads();

    int DestoryThreads();

    static void *ThreadCallbackFunc(void *lp);

    static void HandleSignal(int signum);

    //在一个任务中回收线程资源
    static void RecycleResouce(void *task);
private:
    static deque<Task *>    task_queue;
    static int              thread_count;
    static pthread_t       *thread_ary;
    static pthread_mutex_t  thread_mutex;
    static pthread_cond_t   thread_cond;
    static bool             shutdown;
};

#endif