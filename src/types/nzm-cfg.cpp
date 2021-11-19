#include "./nzm-cfg.h"
#include <string.h>

NzmConfig::NzmConfig(){
    server_list = new NzmServer[cfg_max_server];
    cfg_path = (char *)NZM_CFG_PATH;
    cfg_max_server = NZM_CFG_MAX_SERVER;
    server_size = 0;
}

NzmConfig::~NzmConfig(){
    delete server_list;
}

bool NzmConfig::ParseFromFile(){
    strncpy(server_list[0].ip, "0.0.0.0", 7);
    server_list[0].port = 8099;
    server_size = 1;
    return true;
}

int NzmConfig::GetServerSize(){
    return server_size;
}

NzmServer* NzmConfig::GetServerList(){
    return server_list;
}