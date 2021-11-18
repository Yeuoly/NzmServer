#include <cstring>
#include <arpa/inet.h>
#include "./nzm-socket.h"

NzmScoket::NzmScoket(){}

NzmScoket::NzmScoket(char *ip, short port){
    this->socket_port = port;
    mempcpy(socket_ip_str, ip, 23);
}

bool NzmScoket::Start(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == SOCKET_ERROR_CODE){
        socket_last_error = SOCKET_ERROR_CODE_APPLY_FAILED;
        return false;
    } 

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(socket_port);
    socket_addr.sin_addr.s_addr = inet_addr(socket_ip_str);

    if(bind(fd, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) == SOCKET_ERROR_CODE){
        socket_last_error = SOCKET_ERROR_CODE_BIND_FAILED;
        return false;
    }

    socket_fd = fd;

    return true;
}

bool NzmScoket::Listen(){
    if(::listen(socket_fd, 0) == SOCKET_ERROR_CODE){
        return false;
    }
    return true;
}

//记得在使用完以后delete掉client
SocketClient* NzmScoket::Accept() {
    SocketClient* client = new SocketClient();
    socklen_t length = sizeof(sockaddr_in);
    int fd = ::accept(socket_fd, (struct sockaddr *)client->GetAddr(), &length);
    socket_clients[fd] = client;
    client->SetFd(fd);
    return client;
}

int NzmScoket::GetLastError(){
    return socket_last_error;
}