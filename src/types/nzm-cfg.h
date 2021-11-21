#ifndef __NZM_CFG_H
#define __NZM_CFG_H

#define NZM_CFG_PATH ("./nzm.cfg")
#define NZM_CFG_MAX_SERVER 10
#define NZM_CFG_MAX_CGI_EXT 10
#define NZM_CFG_MAX_PATH_LENGTH 1024

//单个服务器配置
struct NzmServerConfig {
    int port;
    char ip[24];
    char *gci_ext[NZM_CFG_MAX_CGI_EXT];
    char *banned_ext[NZM_CFG_MAX_CGI_EXT];
    //是否允许cgi
    bool allow_gci;
    //是否允许目录穿透到web上级目录
    bool allow_path_cross;
    //根目录
    char *root_path;
    //每个请求的最长时间ms
    unsigned long timeout;
};

//全局配置
class NzmConfig {
public:
    NzmConfig();
    ~NzmConfig();
    bool ParseFromFile();

    int GetServerSize();

    NzmServerConfig* GetServerList();
private:
    NzmServerConfig *server_list;
    char *cfg_path;
    int cfg_max_server;
    int server_size;
};

#endif