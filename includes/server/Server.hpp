#pragma once

#include "Client.hpp"
#include "ConfigParser.hpp"
#include "Data.hpp"
#include <csignal>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#define BUFFSIZE 5096

class Server
{
public:
//const/dest
    Server(std::vector<Data> data, ConfigParser& cpRef);
    ~Server();

//functions
    int     init(void);
    int     run(void);
    int     acceptNewConnection(int FD, int portClient);
    int     readDataFromSocket(int i);
    int     sendResponseClient(int i, int j);
    void    addToPollFDs(int clientFD);
    void    addToClientList(int clientFD, int portClient);
    void    dellFromPollFDs(int index);
    void    dellFromClientList(int FD);
    void    dellAllSocket(void);
    bool    isServerFD(int FD);

    static Server*  getInstance(void);
    static void     sigHandler(int sig);
    void printOpenFDs();

    std::vector<Data>       _vData;
    ConfigParser            _config;
private:
    int                     _Nbserver;
    int                     _Status;
    std::vector<int>        _Ports;
    std::vector<int>        _ServerFDs;
    std::vector<pollfd>     _PollFDs;
    std::vector<Client>     _ClientList;
    static Server*          _instance;
};