#include "./nzm-client.h"

SocketClient::SocketClient() {
    addr = new sockaddr_in();
    alive = true;
}

SocketClient::SocketClient(int fd) : SocketClient() {
    this->fd = fd;
    SocketClient();
}

SocketClient::~SocketClient() {
    delete addr;
}

void SocketClient::SetFd(int fd){
    this->fd = fd;
}

int SocketClient::GetFd(){
    return fd;
}

sockaddr_in* SocketClient::GetAddr() {
    return addr;
}

ssize_t SocketClient::Send(char *buf, size_t length){
    //this might be blocked
    if(!alive){
        return -1;
    }
    ssize_t result = send(fd, buf, length, 0);
    if(result == -1){
        alive = false;
    }
    return result;
}

ssize_t SocketClient::Recvieve(char *buf, size_t length){
    if(!alive){
        return -1;
    }
    ssize_t result = recv(fd, buf, length, 0);
    if(result == -1){
        alive = false;
    }
    return result;
}

void SocketClient::SendBadRequest() {
    const char *msg = "BAD REQUEST 400";
    Send((char *)msg, 16);
}

void SocketClient::SendServiceUnavalable() {
    const char *msg = "Service Unavalable 503";
    Send((char *)msg, 23);
}

void SocketClient::SendNotImplemented() {
    const char *msg = "Not Implemented 501";
    Send((char *)msg, 20);
}