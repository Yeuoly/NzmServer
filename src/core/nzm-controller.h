#ifndef __NZM_CONTROLLER_H
#define __NZM_CONTROLLER_H

#include "./nzm-socket.h"
#include "./nzm-thread.h"
#include "./nzm-client.h"
#include "./nzm-task.h"
#include "../types/nzm-cfg.h"

class NzmController {
public:
    NzmController(char *ip, short port);
    NzmController();
    bool LoadConfig();

    bool Run();

    static void HandleRequest(void *data);

    static void HandleServer(void *data);
private:
    NzmSocket *sock;

    ThreadPool *pool;

    NzmConfig *config;

    char ip[24];
    short port;
};

#endif