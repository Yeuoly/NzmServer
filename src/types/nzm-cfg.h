#ifndef __NZM_CFG_H
#define __NZM_CFG_H

#define NZM_CFG_PATH ("./nzm.cfg")
#define NZM_CFG_MAX_SERVER 10
#define NZM_CFG_MAX_CGI_EXT 10

struct NzmServer {
    int port;
    char ip[24];
    char* gci_ext[NZM_CFG_MAX_CGI_EXT];
    bool allow_gci;
};

class NzmConfig {
public:
    NzmConfig();
    ~NzmConfig();
    bool ParseFromFile();

    int GetServerSize();

    NzmServer* GetServerList();
private:
    NzmServer *server_list;
    char *cfg_path = (char *)NZM_CFG_PATH;
    int cfg_max_server = NZM_CFG_MAX_SERVER;
    int server_size = 0;
};

#endif