#include "./nzm-client.h"
#include "../types/nzm-cfg.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

SocketClient::SocketClient() {
    addr = new sockaddr_in();
    alive = true;
}

SocketClient::SocketClient(int fd) : SocketClient() {
    this->fd = fd;
    SocketClient();
}

SocketClient::~SocketClient() {
    Close();
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
    const char *msg = "400 BAD REQUEST\n\n";
    Send((char *)msg, 18);
}

void SocketClient::SendServiceUnavalable() {
    const char *msg = "503 Service Unavalable\n\n";
    Send((char *)msg, 25);
}

void SocketClient::SendNotImplemented() {
    const char *msg = "501 Not Implemented\n\n";
    Send((char *)msg, 22);
}

void SocketClient::SendForbidden() {
    const char *msg = "403 Forbidden\n\n";
    Send((char *)msg, 16);
}

void SocketClient::SendNotFound() {
    const char *msg = "404 NotFound\n\n";
    Send((char *)msg, 15);
}

void SocketClient::SendOk() {
    const char *msg = "200 OK\n\n";
    Send((char *)msg, 9);
}

int SocketClient::SendNormalFile(char *relative_path, char *root){
    char path[NZM_CFG_MAX_PATH_LENGTH] = { 0 };
    if(strlen(relative_path) + strlen(root) + 1 >= NZM_CFG_MAX_PATH_LENGTH){
        return HTTP_FILE_PATH_ERR;
    }

    strcpy(path, root);
    strcat(path, relative_path);

    printf("%s", path);

    char buf[1024] = { 0 };
    int file_fd = open(path, 0);
    if(file_fd == -1){
        return HTTP_FILE_NOT_FOUND;
    }

    SendOk();

    int length;
    while((length = read(file_fd, buf, 1023)) > 0){
        Send(buf, length);
    }

    return 0;
}

void SocketClient::Close() {
    shutdown(fd, SHUT_RDWR);
}