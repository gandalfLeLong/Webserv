
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"


Client::Client(int clientFD, Data data) : fd(clientFD), serverData(data)
{
    this->complete = false;
    this->redirect = false;
    this->listDir = false;
    this->bytesSent = 0;
    this->serverName = serverData._serverName;
    this->requestBuffer.clear();
    this->AllowMethods = 7;
}

Client::~Client()
{

}

void    Client::handleRequest(std::string req)
{
    this->requestBuffer += req;
    Request request(this->requestBuffer);
    if (request.getStatus() == false)
    {
        this->complete = false;
        return ;
    }
    this->complete = true;
    this->cgi = false;
    this->index.clear();
    this->root.clear();
    this->filePath.clear();
    this->query.clear();
    this->redirect = false;
    this->listDir = false;
    this->AllowMethods = 7;

    this->checkBodySize(request);
    this->handlePath(request);

    if (request.getError())
    {
        this->errorHandler(request);
        return ;
    }
    if (request.getMethode() == "GET")
        this->doGet(request);
    else if (request.getMethode() == "DELETE")
        this->doDelete(request);
    else if (request.getMethode() == "POST")
        this->doPost(request);
    if (static_cast<int>(this->requestBuffer.size()) > this->serverData._maxRequestSize)
    {
        request.setError(413);
        request.setErrorMess("Payload Too Large");
        this->errorHandler(request);
    }
    this->requestBuffer.clear();
}

void    Client::doGet(Request& request)
{
    bool dl = false;
    if (!isMethodAllowed(request.getMethode()))
    {
        request.setError(405);
        request.setErrorMess("Method Not Allowed");
        this->errorHandler(request);
        return ;
    }
    if (this->cgi == true)
        return (this->handleCGI(request));
    if (this->checkFavIcon(request))
        return ;
    if (this->redirect)
        return (this->handleRedirect(request));
    if (this->listDir)
        return (this->handleListingFile(request));
    if (this->query == "?download")
        dl = true;
    std::ifstream  file;
    this->openFileHTML(request, file);
    if (!file)
    {
        this->errorHandler(request);
        return;
    }
    std::stringstream   fileContent;
    fileContent << file.rdbuf();

    Response    response(request);

    response.addHeader("Date", this->getTimeHTTP());
    response.addHeader("Server", this->serverName);
    if (dl == true)
    {
        std::string filename = filePath.substr(root.size() + 1);
        response.addHeader("Content-Disposition", "attachment; filename=\"" + filename + "\"");
    }
    response.addBodyHeader(fileContent.str());
    this->responseBuffer = response.generateResponse();
    this->requestBuffer.clear();
}

void    Client::doPost(Request& request)
{
    if (!isMethodAllowed(request.getMethode()))
    {
        request.setError(405);
        request.setErrorMess("Method Not Allowed");
        this->errorHandler(request);
        return ;
    }
    if (this->cgi == true)
        return (this->handleCGI(request));
    std::string     filepath = this->filePath;
    std::ofstream   file(filepath.c_str(), std::ios::binary);
    if (!file)
    {
        request.setError(403);
        request.setErrorMess("Forbidden");
        this->errorHandler(request);
        return ;
    }
    else
    {
        request.setBody(this->removeExtraHeader(request.getBody()));
        file.write(request.getBody().c_str(), request.getBody().size());
        request.setError(204);
        request.setErrorMess("No Content");
        Response    response(request);
        this->responseBuffer = response.generateResponse();
    }
    this->requestBuffer.clear();
}

void    Client::doDelete(Request& request)
{
    if (!isMethodAllowed(request.getMethode()))
    {
        request.setError(405);
        request.setErrorMess("Method Not Allowed");
        this->errorHandler(request);
        return ;
    }
    std::string location;

    location += this->filePath;
    if (this->isADirectory(location))
    {
        request.setError(403);
        request.setErrorMess("forbidden");
        this->errorHandler(request);
    }
    else if (!this->checkPerm(location))
    {
        request.setError(403);
        request.setErrorMess("Forbidden");
        this->errorHandler(request);
    }
    else
    {
        if (!remove(location.c_str()))
        {
            request.setError(204);
            request.setErrorMess("No Content");
            this->errorHandler(request);
        }
        else
        {
            request.setError(500);
            request.setErrorMess("Internal Server Error");
            this->errorHandler(request);
        }
    }
}

void    Client::errorHandler(Request& request)
{
    int                 errorCode = request.getError();
    Response            response(request);
    std::ifstream       file;
    std::string         filepath;
    std::stringstream   fileContent;
    std::string         errorStr;
    std::stringstream   errorStream;

    
    response.addHeader("Date", this->getTimeHTTP());
    response.addHeader("Server", this->serverName);
    if (this->serverData._errorPages.find(errorCode) != this->serverData._errorPages.end())
    {
        filepath = this->serverData._root + this->serverData._errorPages[errorCode];
        file.open(filepath.c_str());
    }
    if (file.is_open())
    {
        fileContent << file.rdbuf();
        response.addBodyHeader(fileContent.str());
        this->requestBuffer.clear();
        this->responseBuffer = response.generateResponse();
    }
    else
    {
        errorStream << request.getError();
        errorStr = errorStream.str();
        fileContent << 
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head><title>Error " + errorStr + "</title></head>\n"
            "<body>\n"
            "<h1>Error " + errorStr + "</h1>\n"
            "<p>" + request.getErrorMessage() + "</p>\n"
            "<hr>\n"
            "<p>Server: " + this->serverData._serverName + "</p>\n"
            "</body>\n"
            "</html>\n";
        
        response.addBodyHeader(fileContent.str());
        this->requestBuffer.clear();
        this->responseBuffer = response.generateResponse();
    }
    
}

void Client::handlePath(Request& request)
{
    std::string uri = request.getPath();
    int bestMatchSize = 0;
    bool found = false;
    PrefixMatch_t bestMatch;
    
    if (uri.find('?') != uri.npos)
    {
        this->query = uri.substr(uri.find('?'));
        uri = uri.substr(0, uri.find('?'));
    }

    for (size_t i = 0; i < this->serverData._indexList.size(); i++)
    {
        std::string location = this->serverData._indexList[i].match;
        if (uri.substr(0, location.size()) == location)
        {
            if (uri.size() == location.size() || uri[location.size()] == '/')
            {
                if ((int)location.size() > bestMatchSize)
                {
                    bestMatch = this->serverData._indexList[i];
                    bestMatchSize = location.size();
                    found = true;
                }
            }
        }
    }
    if (found)
    {
        if (bestMatch.allowmethod_b)
            this->AllowMethods = bestMatch.allowMethod;
        else
            this->AllowMethods = 7;
    }
    else
        this->AllowMethods = 7;
    if (found && bestMatch.return_b)
    {
        this->redirect = true;
        request.setError(bestMatch.return_.begin()->first);
        request.setErrorMess("Moved Permanently");
        this->filePath = bestMatch.return_.begin()->second;
        return ;
    }
    std::string relativePath;
    if (found && bestMatchSize > 1)
    {
        relativePath = uri.substr(bestMatchSize);
        if (relativePath.empty())
            relativePath = "/";
    }
    else
        relativePath = uri;
    if (found && !bestMatch.root.empty())
        this->root = bestMatch.root;
    else
        this->root = this->serverData._root;
    this->filePath = this->root;
    
    if (!this->filePath.empty() && this->filePath[this->filePath.size() - 1] != '/' && !relativePath.empty() && relativePath[0] != '/')
        this->filePath += "/";
    this->filePath += relativePath;
    if (this->filePath == "cgi/generator.py")
        this->cgi = true;
    if (relativePath.empty() || relativePath[relativePath.length() - 1] == '/')
    {
        bool indexFound = false;
        if (found && !bestMatch.index.empty())
        {
            this->index = bestMatch.index[0];
            
            for (size_t i = 0; i < bestMatch.index.size(); i++)
            {
                std::string testPath = this->filePath + bestMatch.index[i];
                struct stat buffer;
                if (stat(testPath.c_str(), &buffer) == 0 && !(buffer.st_mode & S_IFDIR))
                {
                    this->filePath = testPath;
                    indexFound = true;
                    break;
                }
            }
        }
        if (!indexFound && found && bestMatch.autoindex_b && bestMatch.autoindex)
            this->listDir = true;
        else if (!indexFound && this->isADirectory(this->filePath))
        {
            request.setError(404);
            request.setErrorMess("Not found");
        }
    }
}

void    Client::handleRedirect(Request& request)
{
    Response response(request);

    response.addHeader("Content-Length", "0");
    response.addHeader("Location", this->filePath);
    this->responseBuffer = response.generateResponse();
    this->requestBuffer.clear();
    return ;
}

void    Client::handleCGI(Request& request)
{
    int input_pipe[2];
    int output_pipe[2];
    
    if (pipe(input_pipe) < 0 || pipe(output_pipe) < 0)
    {
        request.setError(500);
        request.setErrorMess("Internal Server Error: Pipe creation failed");
        this->errorHandler(request);
        return;
    }
    
    std::ostringstream oss;
    oss << request.getBody().length();
    std::string contentLength = oss.str();
    setenv("SCRIPT_FILENAME", this->filePath.c_str(), 1);
    setenv("REQUEST_METHOD", request.getMethode().c_str(), 1);
    setenv("CONTENT_LENGTH", contentLength.c_str(), 1);
    if (!this->query.empty())
        setenv("QUERY_STRING", this->query.substr(1).c_str(), 1);
    else
        setenv("QUERY_STRING", "", 1);
    pid_t pid = fork();
    
    if (pid < 0)
    {
        request.setError(500);
        request.setErrorMess("Internal Server Error: Fork failed");
        this->errorHandler(request);
        close(input_pipe[0]); close(input_pipe[1]);
        close(output_pipe[0]); close(output_pipe[1]);
        return;
    }
    else if (pid == 0)
    {
        close(input_pipe[1]);
        dup2(input_pipe[0], STDIN_FILENO);
        close(input_pipe[0]);
        
        close(output_pipe[0]);
        dup2(output_pipe[1], STDOUT_FILENO);
        close(output_pipe[1]);

        execl("/usr/bin/python3", "python3", this->filePath.c_str(), NULL);
        
        exit(1);
    }
    else {
        close(input_pipe[0]);
        close(output_pipe[1]);
        
        if (request.getMethode() == "POST")
            write(input_pipe[1], request.getBody().c_str(), request.getBody().length());
        close(input_pipe[1]);
        
        time_t start_time = time(NULL);
        const int timeout_seconds = 5;
        bool timed_out = false;
        
        int flags = fcntl(output_pipe[0], F_GETFL, 0);
        fcntl(output_pipe[0], F_SETFL, flags | O_NONBLOCK);
        
        std::string cgi_output;
        char buffer[4096];
        ssize_t bytes_read;
        bool process_finished = false;
        int status = 0;
        
        while (!process_finished && !timed_out)
        {
            pid_t result = waitpid(pid, &status, WNOHANG);
            
            if (result == pid)
            {
                while ((bytes_read = read(output_pipe[0], buffer, sizeof(buffer) - 1)) > 0)
                {
                    buffer[bytes_read] = '\0';
                    cgi_output += buffer;
                }
                process_finished = true;
                break;
            }
            
            bytes_read = read(output_pipe[0], buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0';
                cgi_output += buffer;
            }
            
            if (time(NULL) - start_time >= timeout_seconds)
            {
                timed_out = true;
                kill(pid, SIGTERM);
                usleep(100000);
                kill(pid, SIGKILL);
                waitpid(pid, NULL, 0); 
            }
            
            if (!process_finished && !timed_out)
                usleep(10000);
        }
        
        close(output_pipe[0]);
        
        if (timed_out)
        {
            request.setError(504);
            request.setErrorMess("CGI Process Timeout");
            this->errorHandler(request);
        }
        else if (process_finished && WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            Response response(request);
            response.addHeader("Content-Type", "text/html");
            response.addBodyHeader(cgi_output);
            this->responseBuffer = response.generateResponse();
        } 
        else
        {
            request.setError(500);
            request.setErrorMess("Internal Server Error: CGI execution failed");
            this->errorHandler(request);
        }
    }
}

void    Client::handleListingFile(Request& request)
{
    std::vector<std::string> filesInDir;
    struct dirent* entry;
    DIR*    directory = opendir(this->filePath.c_str());
    if (directory == NULL)
    {
        request.setError(403);
        request.setErrorMess("Forbidden");
        this->errorHandler(request);
        return ;
    }
    while ((entry = readdir(directory)) != NULL)
    {
        std::string file;

        file = entry->d_name;
        if (file == "." || file == "..")
            continue ;
        filesInDir.push_back(file); 
    }
    closedir(directory);
    this->makeResponseListDir(filesInDir, request);
}

void    Client::makeResponseListDir(std::vector<std::string> filesInDir, Request& request)
{
    std::string body;
    Response response(request);

    body = 
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
    <title>" + this->filePath + "</title>\n\
    </head>\n\
    <body>\n\
    <p>Index of " + this->filePath + "</p>\n\
    <p>----------------------------------------------------------------------------------------</p>\n";

    for (size_t i = 0; i < filesInDir.size(); i++)
    {
        body += "\t\t<p><a href=\"http://" + this->serverName + ":" + filesInDir[i] + "\">" + filesInDir[i] + "</a></p>\n";
    }
    body += "\
    <p>----------------------------------------------------------------------------------------</p>\n\
    </body>\n\
    </html>\n";
    response.addBodyHeader(body);
    this->responseBuffer = response.generateResponse();
}

std::string Client::getTimeHTTP(void)
{
    std::time_t now = std::time(NULL);
    std::tm*    gmtTime = std::gmtime(&now);

    char    buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);

    return (std::string(buffer));
}

void    Client::openFileHTML(Request& request, std::ifstream& file)
{
    std::string     path = this->filePath;

    file.open(path.c_str());
    if (!file)
    {
        if (errno == ENOENT)
        {
            request.setError(404);
            request.setErrorMess("Not Found");
        }
        else if (errno == EACCES)
        {
            request.setError(403);
            request.setErrorMess("Forbidden");
        }
    }
}

std::string Client::removeExtraHeader(std::string requestBody)
{
    size_t start = requestBody.find("\r\n\r\n");
    if (start == std::string::npos)
        return requestBody;
    start += 4;

    return (requestBody.substr(start));
}

bool Client::isMethodAllowed(std::string method) 
{
    if (method == "GET") {
        return (this->AllowMethods & 1) != 0;
    }
    else if (method == "POST") {
        return (this->AllowMethods & 2) != 0;
    }
    else if (method == "DELETE") {
        return (this->AllowMethods & 4) != 0;
    }
    return false;
}

void    Client::checkBodySize(Request& request)
{
    if (this->serverData._maxRequestSize < static_cast<int>(this->requestBuffer.size()))
    {
        request.setError(413);
        request.setErrorMess("Payload Too Large");
    }
}

int Client::getFD(void)
{
    return (this->fd);
}

std::string Client::getResponse(void)
{
    return (this->responseBuffer);
}

bool    Client::checkFavIcon(Request request)
{
    std::string path = request.getPath();
    if (path.size() >= 11 && path.substr(path.length() - 11) == "/favicon.ico")
        return true;
    else
        return false;
}

bool    Client::getComplete(void)
{
    return (this->complete);
}

bool    Client::isADirectory(std::string path)
{
    struct stat pathStat;

    if (stat(path.c_str(), &pathStat) != 0)
        return (false);
    return (S_ISDIR(pathStat.st_mode));
}

bool    Client::checkPerm(std::string path)
{
    return (access(path.c_str(), W_OK) == 0);
}

void    Client::setResponse(std::string newResponse)
{
    this->responseBuffer = newResponse;
}