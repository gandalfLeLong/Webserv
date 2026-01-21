#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <map>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <cstdio>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <sys/wait.h>
#include "ConfigParser.hpp"
#include "Data.hpp"

class Server;
class Request;
class Response;

class Client
{
    
public:
    Client(int clientFD, Data data);
    ~Client();

    int     getFD(void);
    void    handlePath(Request& request);
    void    handleRequest(std::string request);
    void    doGet(Request& request);
    void    doDelete(Request& request);
    void    doPost(Request& request);
    void    checkBodySize(Request& request);
    void    errorHandler(Request& request);
    void    openFileHTML(Request& request, std::ifstream& file);
    void    handleListingFile(Request& request);
    void    handleRedirect(Request& request);
    void    makeResponseListDir(std::vector<std::string> filesInDir, Request& request);
    void    setResponse(std::string newResponse);
    void    handleCGI(Request& request);
    bool    isMethodAllowed(std::string method);
    bool    getComplete(void);
    bool    checkFavIcon(Request request);
    bool    isADirectory(std::string path);
    bool    checkPerm(std::string path);
    std::string     removeExtraHeader(std::string requestBody);
    std::string     getResponse();
    std::string     getTimeHTTP(void);

private:
    int         fd;
    int         AllowMethods;
    bool        complete;
    bool        listDir;
    bool        redirect;
    bool        cgi;
    Data        serverData;
    size_t      bytesSent;
    std::string root;
    std::string index;
    std::string filePath;
    std::string query;
    std::string serverName;
    std::string requestBuffer;
    std::string responseBuffer;
};