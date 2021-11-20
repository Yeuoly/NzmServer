#ifndef __NZM_SOCKET_H
#define __NZM_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <map>

#include "./nzm-client.h"
#include "../types/nzm-cfg.h"

#define SOCKET_ERROR_CODE -1

#define SOCKET_ERROR_CODE_BIND_FAILED 1
#define SOCKET_ERROR_CODE_APPLY_FAILED 2
#define SOCKET_ERROR_CODE_LISTEN_FAILED 3

#define HTTP_PATH_EXT_NORMAL 0
#define HTTP_PATH_EXT_CGI 1
#define HTTP_PATH_EXT_BANNED -1

class NzmSocket {
public:
    NzmSocket();

    NzmSocket(char *ip, short port);

    bool Start();

    bool Listen();

    void ClearClient(int fd);

    void SetServerConfig(NzmServerConfig *server_config);

    NzmServerConfig* GetServerConfig();

    SocketClient* Accept();

    int GetLastError();

    int IsExtAvaliable(char *path);
protected:

private:
    int                           socket_fd;
    short                         socket_port;
    char                          socket_ip_str[24];
    struct sockaddr_in            socket_addr;
    std::map<int, SocketClient *> socket_clients;

    int                           socket_last_error = 0;
    NzmServerConfig              *server_config;
};

#endif