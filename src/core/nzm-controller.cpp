#include "./nzm-controller.h"
#include <cstring>

NzmController::NzmController(char *ip, short port){
    mempcpy(this->ip, ip, 23);
    this->port = port;
    NzmController();
}

NzmController::NzmController(){
    config = new NzmConfig();
}

bool NzmController::LoadConfig(){
    return config->ParseFromFile();
}

bool NzmController::Run(){
    pool = new ThreadPool(20);

    if(!sock->Start() || !sock->Listen()){
        //cout << sock->GetLastError() << endl;
        return false;
    }

    NzmServer *list = config->GetServerList();
    int server_size = config->GetServerSize();
    Task **server_tasks = new Task*[server_size];

    for(int i = 0; i < server_size; i++){
        Task *server_task = new Task(HandleServer, NULL);
        server_tasks[i] = server_task;
        server_task->SetData((void *)&list[i]);
        pool->AppendTask(server_task);
    }

    //wait

    return true;
}

void NzmController::HandleRequest(void *data){
    Task *task = (Task *)data;

    

    delete task;
}

void NzmController::HandleServer(void *data){
    NzmServer *server = (NzmServer *)data;
    NzmSocket *sock = new NzmSocket(server->ip, server->port);

    while(true){
        SocketClient *client = sock->Accept();
        SocketTask *task = new SocketTask(HandleRequest, NULL);
        task->SetData((void *)task);
        task->SetSocketClinet(client);

        pool->AppendTask(task);
    }

    delete task;
}