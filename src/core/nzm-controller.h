#ifndef __NZM_CONTROLLER_H
#define __NZM_CONTROLLER_H

#include "./nzm-socket.h"
#include "./nzm-thread.h"
#include "./nzm-client.h"
#include "./nzm-task.h"
#include "../types/nzm-cfg.h"

class NzmController {
public:
    NzmController();
    NzmController(char *ip, short port);

    bool LoadConfig();

    bool Run();

    //用于处理HTTP请求
    static void HandleRequest(void *data);

    //用于处理服务器
    static void HandleServer(void *data);

    //用于处理SOCKET请求
    static void HandleSocket(void *data);

private:
    ThreadPool      *pool;
    NzmConfig       *config;

    pthread_cond_t   controller_cond;
    pthread_mutex_t  controller_mutex;

    char ip[24];
    short port;
};

#endif