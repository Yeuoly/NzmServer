#include "./nzm-task.h"

Task::Task(void (* func)(void *), void *data){
    cb = func;
    this->data = data;
}

void Task::Run(){
    if(this->cb){
        this->cb(this->data);
    }
}

void Task::Fail() {
    if(this->fail_cb){
        this->fail_cb();
    }
}

void Task::SetFailCallback(void (* func)()){
    fail_cb = func;
}

void Task::SetData(void *data){
    this->data = data;
}

void SocketTask::SetSocketClinet(SocketClient *clinet){
    this->socket_client = clinet;
}

SocketClient* SocketTask::GetSocketClient(){
    return socket_client;
}

SocketTask::~SocketTask(){
    if(socket_client){
        delete socket_client;
    }
}

void SocketTask::Fail(){
    socket_client->SendServiceUnavalable();
}