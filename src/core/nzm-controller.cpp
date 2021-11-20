#include "./nzm-controller.h"
#include "../utils/parser.h"
#include <cstring>
#include <iostream>

using namespace std;

NzmController::NzmController(char *ip, short port) : NzmController() {
    mempcpy(this->ip, ip, 23);
    this->port = port;
}

NzmController::NzmController(){
    config = new NzmConfig();
    controller_mutex = PTHREAD_MUTEX_INITIALIZER;
    controller_cond = PTHREAD_COND_INITIALIZER;
}

bool NzmController::LoadConfig(){
    return config->ParseFromFile();
}

bool NzmController::Run(){
    pool = new ThreadPool(20);

    int server_size = config->GetServerSize();
    NzmServerConfig *list = config->GetServerList();
    ServerTask **server_tasks = new ServerTask*[server_size];

    for(int i = 0; i < server_size; i++){
        ServerTask *server_task = new ServerTask(HandleServer, NULL);
        server_tasks[i] = server_task;
        server_task->pool = (void *)pool;
        server_task->server = (void *)&list[i];
        server_task->SetData(server_task);
        pool->AppendTask(server_task);
    }

    //wait
    pthread_mutex_lock(&controller_mutex);
    pthread_cond_wait(&controller_cond, &controller_mutex);

    for(int i = 0; i < server_size; i++){
        delete server_tasks[i];
    }

    delete[] server_tasks;
    return true;
}

void NzmController::HandleRequest(void *data){
    SocketTask *task = (SocketTask *)data;
    SocketClient *client = task->GetSocketClient();
    HttpClient *http = new HttpClient();
    NzmSocket *sock = (NzmSocket *)task->parent_sock;
    NzmServerConfig *server_config = sock->GetServerConfig();
    char *path, *params;
    int ext_status;

    //接收HTTP协议头与URI
    char *buf = new char[1024];
    int buf_pos = 0, current_length;
    if((current_length = client->Recvieve(buf, 1023)) == -1){
        goto end;
    }else if(current_length <= 14){
        client->SendNotImplemented();
        goto end;
    }

    if(!strncasecmp(buf, "GET", 3)){
        http->method = HTTP_METHOD_GET;
        buf_pos += 3;
    }else if(!strncasecmp(buf, "POST", 4)){
        http->method = HTTP_METHOD_POST;
        buf_pos += 4;
    }else if(!strncasecmp(buf, "OPTIONS", 7)){
        http->method = HTTP_METHOD_OPTIONS;
        buf_pos += 7;
    }else if(!strncasecmp(buf, "PUT", 3)){
        http->method = HTTP_METHOD_PUT;
        buf_pos += 3;
    }else if(!strncasecmp(buf, "DELETE", 6)){
        http->method = HTTP_METHOD_DELETE;
        buf_pos += 6;
    }else{
        client->SendNotImplemented();
        goto end;
    }

    if(buf[buf_pos] != ' '){
        client->SendNotImplemented();
        goto end;
    }
    
    while(buf_pos < 1023 && buf[++buf_pos] == ' '){
        if(buf[buf_pos] == '\n'){
            client->SendNotImplemented();
            goto end;
        }
    }

    if(buf_pos == 1024) {
        client->SendNotImplemented();
        goto end;
    }
    
    //开始匹配path
    path = buf + buf_pos;

    while(buf_pos < 1023 && buf[++buf_pos] != '?' && buf[buf_pos] != ' '){
        if(buf[buf_pos] == '\n'){
            client->SendNotImplemented();
            goto end;
        }
    }

    if(buf_pos == 1024){
        client->SendNotImplemented();
        goto end;
    }

    if(buf[buf_pos] == ' ') {
        buf[buf_pos] = '\x00';
        params = NULL;
    }else{
        buf[buf_pos] = '\x00';
        params = buf + buf_pos + 1;
        while(buf_pos < 1023 && buf[++buf_pos] != ' '){
            if(buf[buf_pos] == '\n'){
                client->SendNotImplemented();
                goto end;
            }
        }
    }

    if(buf_pos == 1024){
        client->SendNotImplemented();
        goto end;
    }

    buf[buf_pos] = '\x00';

    http->params = params;
    http->path = path;

    if(!strncasecmp(buf + buf_pos + 1, "HTTP/1.0", 8)){
        http->version = HTTP_VERSION_1_0;
    }else if(!strncasecmp(buf + buf_pos + 1, "HTTP/1.1", 8)){
        http->version = HTTP_VERSION_1_1;
    }else if(!strncasecmp(buf + buf_pos + 1, "HTTP/2.0", 8)){
        http->version = HTTP_VERSION_2_0;
    }

    if(buf[buf_pos + 9] != '\n'){
        client->SendNotImplemented();
        goto end;
    }

    buf_pos += 10;

    //analysis path and
    if(path[0] != '/'){
        client->SendNotImplemented();
        goto end;
    }

    //检测是否允许目录穿越
    if(!server_config->allow_path_cross && !IsPathInSize(path, strlen(path))){
        client->SendForbidden();
        goto end;
    }

    //检测当前后缀是否在允许范围内
    if(HTTP_PATH_EXT_BANNED == (ext_status = sock->IsExtAvaliable(path))){
        client->SendForbidden();
        goto end;
    }

    //如果不是GCI后缀的话就直接读文件
    switch(ext_status){
    case HTTP_PATH_EXT_CGI:

        break;
    case HTTP_PATH_EXT_NORMAL:
        if(client->SendNormalFile(path, server_config->root_path) == HTTP_FILE_NOT_FOUND){
            client->SendNotFound();
            goto end;
        }
        break;
    }
end:
    //执行完任务以后清理资源
    sock->ClearClient(client->GetFd());
    delete[] buf;
    delete task; //this will delete socket-client also
}

void NzmController::HandleServer(void *data){
    ServerTask *task = (ServerTask *)data;
    NzmServerConfig *server_config = (NzmServerConfig *)task->server;
    ThreadPool *pool = (ThreadPool *)task->pool;
    NzmSocket *sock = new NzmSocket(server_config->ip, server_config->port);
    sock->SetServerConfig(server_config);

    if(!sock->Start() || !sock->Listen()){
        cout << "开启监听失败" << endl;
    }

    while(true){
        SocketTask *task = new SocketTask(HandleRequest, NULL);
        SocketClient *client = sock->Accept();
        task->parent_sock = sock;
        task->SetData((void *)task);
        task->SetSocketClinet(client);
        
        if(!pool->AppendTask(task)){
            task->Fail();
            sock->ClearClient(client->GetFd());
            delete task;
        }
    }

    delete sock;
}