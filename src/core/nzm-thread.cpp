#include "./nzm-thread.h"
#include "../utils/debug.h"

pthread_mutex_t ThreadPool::thread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::thread_cond = PTHREAD_COND_INITIALIZER;

bool ThreadPool::shutdown = false;
deque<Task *> ThreadPool::task_queue;

ThreadPool::ThreadPool(int default_thread_count){
    thread_count = default_thread_count;
    thread_ary = NULL;
    CreateThreads();
}

void *ThreadPool::ThreadCallbackFunc(void *lp){
    pthread_t current_pid = pthread_self();

    Debug::Log("[THREAD_POOL] thread started");

    while (true){
        //atomic
        pthread_mutex_lock(&thread_mutex);
        while(task_queue.size() == 0 && !shutdown){
            pthread_cond_wait(&thread_cond, &thread_mutex);
        }
        if(shutdown){
            pthread_mutex_unlock(&thread_mutex);
            pthread_exit(0);
        }

        Task *task = task_queue.front();
        task_queue.pop_front();

        pthread_mutex_unlock(&thread_mutex);

        task->Run();
    }

    return (void *)0;
}

int ThreadPool::CreateThreads(){
    thread_ary = new pthread_t[thread_count];

    for(int i = 0; i < thread_count; i++){
        pthread_create(thread_ary + i, NULL, ThreadCallbackFunc, NULL);
    }

    return 0;
}

bool ThreadPool::AppendTask(Task *task){
    pthread_mutex_lock(&thread_mutex);
    if(task_queue.max_size() == task_queue.size()){
        return false;
    }

    task_queue.push_back(task);
    pthread_mutex_unlock(&thread_mutex);

    pthread_cond_signal(&thread_cond);
    return true;
}