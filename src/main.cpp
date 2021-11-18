#include <iostream>
#include "./core/nzm-socket.h"
#include "./core/nzm-thread.h"
#include "./core/nzm-thread.h"

using namespace std;

void HandleRequest(void *data){
    Task *task = (Task *)data;
    
    cout << "new task started" << endl;

    delete task;
}

int main(){
    NzmScoket *sock = new NzmScoket((char *)"0.0.0.0", 8099);

    ThreadPool *pool = new ThreadPool(10);

    if(!sock->Start() || !sock->Listen()){
        cout << sock->GetLastError() << endl;
        return 1;
    }

    while(true){
        SocketClient *client = sock->Accept();
        SocketTask *task = new SocketTask(HandleRequest, NULL);
        task->SetData((void *)task);
        task->SetSocketClinet(client);

        pool->AppendTask(task);
    }
    
    return 0;
}