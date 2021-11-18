#include "./nzm-client.h"

SocketClient::SocketClient() {
    addr = new sockaddr_in();
}

SocketClient::SocketClient(int fd){
    this->fd = fd;
    SocketClient();
}

SocketClient::~SocketClient() {
    delete addr;
}

void SocketClient::SetFd(int fd){
    this->fd = fd;
}

sockaddr_in* SocketClient::GetAddr() {
    return addr;
}