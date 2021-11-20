#ifndef __NZM_CLIENT_H
#define __NZM_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define HTTP_VERSION_1_0 0x0
#define HTTP_VERSION_1_1 0x1
#define HTTP_VERSION_2_0 0x2

#define HTTP_METHOD_OPTIONS 0x0
#define HTTP_METHOD_GET 0x1
#define HTTP_METHOD_POST 0x2
#define HTTP_METHOD_PUT 0x3
#define HTTP_METHOD_DELETE 0x4

#define HTTP_FILE_NOT_FOUND 404
#define HTTP_FILE_PATH_ERR 1
#define HTTP_FILE_OK 

struct HttpClient {
    char version;
    char method;
    char *path;
    char *params;
    char *headers;
    char *body;
};

class SocketClient {
public:
    SocketClient();
    SocketClient(int fd);
    ~SocketClient();

    void SetFd(int fd);

    int GetFd();

    sockaddr_in* GetAddr();

    ssize_t Send(char *buf, size_t length);

    ssize_t Recvieve(char *buf, size_t length);

    //错误的请求格式
    void SendBadRequest();

    //服务繁忙
    void SendServiceUnavalable();

    //请求方法不允许
    void SendNotImplemented();

    //403
    void SendForbidden();

    //404
    void SendNotFound();

    //200
    void SendOk();

    //发送普通文件，返回码
    int SendNormalFile(char *relative_path, char *root);

    void Close();

    char* GetClientIp();
private:
    

    sockaddr_in *addr;

    int fd;

    int alive;
};

#endif