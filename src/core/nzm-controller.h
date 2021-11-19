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

    static void HandleRequest(void *data);

    static void HandleServer(void *data);
private:
    ThreadPool      *pool;
    NzmConfig       *config;

    pthread_cond_t   controller_cond;
    pthread_mutex_t  controller_mutex;

    char ip[24];
    short port;
};

#endif