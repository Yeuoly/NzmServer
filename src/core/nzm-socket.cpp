#include <cstring>
#include <arpa/inet.h>
#include "./nzm-socket.h"

NzmSocket::NzmSocket(){}

NzmSocket::NzmSocket(char *ip, short port) : NzmSocket(){
    this->socket_port = port;
    mempcpy(socket_ip_str, ip, 23);
}

bool NzmSocket::Start(){
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

bool NzmSocket::Listen(){
    if(::listen(socket_fd, 0) == SOCKET_ERROR_CODE){
        return false;
    }
    return true;
}

//记得在使用完以后delete掉client
SocketClient* NzmSocket::Accept() {
    SocketClient* client = new SocketClient();
    socklen_t length = sizeof(sockaddr_in);
    int fd = ::accept(socket_fd, (struct sockaddr *)client->GetAddr(), &length);
    socket_clients[fd] = client;
    client->SetFd(fd);
    return client;
}

int NzmSocket::GetLastError(){
    return socket_last_error;
}

void NzmSocket::ClearClient(int fd){
    if(socket_clients[fd]){
        socket_clients.erase(fd);
    }
}

void NzmSocket::SetServerConfig(NzmServerConfig *server_config){
    this->server_config = server_config;
}

NzmServerConfig* NzmSocket::GetServerConfig(){
    return server_config;
}

int NzmSocket::IsExtAvaliable(char *path){
    return HTTP_PATH_EXT_NORMAL;
}