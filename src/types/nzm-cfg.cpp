#include "./nzm-cfg.h"

NzmConfig::NzmConfig(){
    server_list = new NzmServer[cfg_max_server];
}

NzmConfig::~NzmConfig(){
    delete server_list;
}

bool NzmConfig::ParseFromFile(){
    return true;
}

int NzmConfig::GetServerSize(){
    return server_size;
}

NzmServer* NzmConfig::GetServerList(){
    return server_list;
}