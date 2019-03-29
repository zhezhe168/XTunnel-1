#include "server.h"
#include <map>
#include <string>
#include <signal.h>
#include "inifile.h"

Server *g_pServer = nullptr;

struct ConfigServer
{
    unsigned short serverPort;
    unsigned short proxyPort;
    std::string password;
} g_cfg;

void readConfig(const char *cfgFile)
{
    inifile::IniFile iniFile;
    int ret = iniFile.Load(cfgFile);
    if (ret == -1)
    {
        printf("can't open config file\n");
        exit(-1);
    }
    string common = "common";
    int serverPort, porxyPort;
    string password;
    iniFile.GetIntValue(common, "server_port", &serverPort);
    iniFile.GetIntValue(common, "proxy_port", &porxyPort);
    iniFile.GetStringValue(common, "password", &password);

    g_cfg.password = password;
    g_cfg.serverPort = serverPort;
    g_cfg.proxyPort = porxyPort;
    //printf("pw:%s\nsp: %d\npp: %d\n", g_cfg.password.c_str(), g_cfg.serverPort, g_cfg.proxyPort);
}

int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);

    readConfig("ts.ini");
    g_pServer = new Server(g_cfg.serverPort, g_cfg.proxyPort);
    if (g_pServer == nullptr)
    {
        printf("create server err\n");
        return -1;
    }
    g_pServer->setPassword(g_cfg.password.c_str());
    g_pServer->startEventLoop();
    delete g_pServer;
    return 0;
}