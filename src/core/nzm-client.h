#ifndef __NZM_CLIENT_H
#define __NZM_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

class SocketClient {
public:
    SocketClient();
    SocketClient(int fd);
    ~SocketClient();

    void SetFd(int fd);

    sockaddr_in* GetAddr();
private:
    struct sockaddr_in *addr;

    int fd;
};

#endif