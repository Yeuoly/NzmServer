#ifndef __NZM_TASK_H
#define __NZM_TASK_H

#include "./nzm-client.h"

class Task {
public:
    Task(){};

    Task(void (* func)(void *), void *data);

    void SetFailCallback(void (* func)());

    void SetData(void *data);

    void Run();

    void Fail();
private:
    void *data;

    void (* cb)(void *);

    void (* fail_cb)();
};

class RecycleThreadTask : public Task {
public:
    using Task::Task;

    pthread_t pid;
};

class SocketTask : public Task {
public:
    using Task::Task;

    ~SocketTask();

    void SetSocketClinet(SocketClient *socket_clinet);

    SocketClient* GetSocketClient();

    //任务执行失败时调用
    void Fail();

    //所属SOCKET父级
    void *parent_sock;

private:
    SocketClient *socket_client;
};

class ServerTask : public Task {
public:
    using Task::Task;

    void *pool;

    void *server;
};

#endif