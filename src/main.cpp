#include <iostream>
#include "./core/nzm-controller.h"

using namespace std;

int main(){
    NzmController *con = new NzmController((char *)"0.0.0.0", 8099);
    if(!con->LoadConfig()){
        cout << "加载配置文件失败" << endl;
    }
    if(!con->Run()){
        cout << "服务器启动失败" << endl;
    }
    return 0;
}