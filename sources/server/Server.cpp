/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmathot <cmathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:07:48 by cmathot           #+#    #+#             */
/*   Updated: 2025/04/18 12:47:12 by cmathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ConfigParser.hpp"

Server* Server::_instance = NULL;

Server::Server(std::vector<Data> vData, ConfigParser& cpRef) : _vData(vData), _config(cpRef)
{
    this->_Status = 0;
    this->_Nbserver = _vData.size();
    this->_instance = this;
    std::signal(SIGINT, Server::sigHandler);
}

Server::~Server()
{
    this->dellAllSocket();
    if (this == _instance)
        _instance = NULL;
}

int Server::init(void)
{
    std::cout << "Inialisation began" << std::endl;

    for (int i = 0; i < this->_Nbserver; i++)
    {
        std::vector<int>    serverPorts = _vData[i]._ports;
        
        for (size_t j = 0; j < serverPorts.size(); j++)
        {
            int serverFD;
            struct sockaddr_in SocketInfo;
            
            memset(&SocketInfo, 0, sizeof(SocketInfo));
            SocketInfo.sin_family = AF_INET;
            SocketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            SocketInfo.sin_port = htons(serverPorts[j]);
            serverFD = socket(SocketInfo.sin_family,SOCK_STREAM, 0);
            if (serverFD == -1)
            {
                std::cout << "Error: socket of server " << serverPorts[j] << std::endl;
                return (-1);
            }
            
            int opt = 1;
            this->_Status = setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            if (this->_Status == -1)
            {
                std::cout << "Error: setsocketopt of server " << serverPorts[j] << std::endl;        
                return (-1);
            }
            
            this->_Status = bind(serverFD, (struct sockaddr *)&SocketInfo, sizeof(SocketInfo));
            if (this->_Status != 0)
            {
                std::cout << "Error: bind of server " << serverPorts[j] << std::endl;        
                return (-1);
            }
            std::cout << "[Server] bound socket to localhost port " << serverPorts[j] << std::endl;
            std::cout << "Server " << serverPorts[j] <<  " is now running" << std::endl;
            
            this->_Status = listen(serverFD, SOMAXCONN < 0);
            if (this->_Status != 0)
            {
                std::cout << "Error: listen of server " << serverPorts[j] << std::endl;
                return (-1);
            }
            
            this->_Status = fcntl(serverFD, F_SETFL, O_NONBLOCK);
            if (this->_Status == -1)
            {
                std::cout << "Error: fcntl of server " << serverPorts[j] << std::endl;        
                return (-1);
            }
            this->_Ports.push_back(serverPorts[j]);
            this->_ServerFDs.push_back(serverFD);
            this->addToPollFDs(serverFD);
        }
    }
    std::cout << "PolFDs are set up" << std::endl;
    return (0);
}

int    Server::run(void)
{
    while (1)
    {
        this->_Status = poll(this->_PollFDs.data(), this->_PollFDs.size(), 1000);
        if (this->_Status == -1)
        {
            this->dellAllSocket();
            return (-1);
        }
        else if (this->_Status == 0)
            std::cout << "Server waiting ..." << std::endl;

        std::vector<int>    fdsToRemove;
        for (int i = 0; i < static_cast<int>(this->_PollFDs.size()); i++)
        {
            if (!(this->_PollFDs[i].revents & POLLIN))
                continue;
            //std::cout << this->_PollFDs[i].fd << " is ready for I/O operations." << std::endl;
            
            if (isServerFD(this->_PollFDs[i].fd))
            {
                if (acceptNewConnection(this->_PollFDs[i].fd, this->_Ports[i]))
                {
                    //this->dellFromClientList(this->_PollFDs[i].fd);
                    //this->dellFromPollFDs(this->_PollFDs[i].fd);
                    fdsToRemove.push_back(this->_PollFDs[i].fd);
                }
            }
            else
            {
                if (this->readDataFromSocket(i))
                {
                    //this->dellFromClientList(this->_PollFDs[i].fd);
                    //this->dellFromPollFDs(this->_PollFDs[i].fd);
                    fdsToRemove.push_back(this->_PollFDs[i].fd);
                }
                for (size_t j = 0; j < this->_ClientList.size(); j++)
                {
                    if (this->_PollFDs[i].fd == this->_ClientList[j].getFD())
                    {
                        if (this->_ClientList[j].getComplete() == true)
                            this->_PollFDs[i].events |= POLLOUT;
                    }
                }
            }
        }
        for (int i = 0; i < static_cast<int>(this->_PollFDs.size()); i++)
        {
            if (this->_PollFDs[i].revents & POLLOUT)
            {
                for (size_t j = 0; j < this->_ClientList.size(); j++)
                {
                    if (this->_PollFDs[i].fd == this->_ClientList[j].getFD())
                    {
                        if (this->sendResponseClient(i, j))
                        {
                            fdsToRemove.push_back(this->_PollFDs[i].fd);
                            break ;
                            //this->dellFromClientList(this->_PollFDs[i].fd);
                            //this->dellFromPollFDs(this->_PollFDs[i].fd);
                        }
                    }
                }
            }
        }
        for (size_t i = 0; i < fdsToRemove.size(); i++)
        {
            this->dellFromClientList(fdsToRemove[i]);
            this->dellFromPollFDs(fdsToRemove[i]);
        }
    }
}

int    Server::acceptNewConnection(int FD, int portClient)
{
    int clientFD;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    clientFD = accept(FD, reinterpret_cast<struct sockaddr*>(&client_addr), &client_len);
    if (clientFD == -1)
    {
        std::cout << "Error: accept for client FD " << FD << std::endl;
        return (-1);
    }
    int opt = 1;
    if (setsockopt(clientFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(clientFD);
        std::cout << "Failed to set SO_REUSEADDR on client socket" << std::endl;
        return (-1);
    }
    this->_Status = fcntl(clientFD, F_SETFL, O_NONBLOCK);
    if (this->_Status == -1)
    {
        close (clientFD);
        std::cout << "Error: Couldn't pass the client FD to non-blocking mode. (" << FD << ")"  << std::endl;
        return (-1);
    }
    addToPollFDs(clientFD);
    addToClientList(clientFD, portClient);
    
    //std::cout << "Accepted connexion on " << clientFD << std::endl;
    return 0;
}

int    Server::readDataFromSocket(int i)
{
    ssize_t      bytesRead;
    int         senderFD;
    char        buffer[BUFFSIZE];

    senderFD = this->_PollFDs[i].fd;
    memset(buffer, 0, sizeof(buffer));
    bytesRead = 0;
    bytesRead = recv(senderFD, buffer, BUFFSIZE - 1, 0);
    if (bytesRead <= 0)
    {
        if (bytesRead == 0)
        {
            //std::cout << "Closed connection on " << senderFD << std::endl;
            return (-1);
        }
        else
        {
            std::cout << "Error in recv for Client " << senderFD << std::endl;
            return (-1);
        }
    }
    else
    {
        std::string request;
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            request.push_back(buffer[i]);
        }
        for (size_t i = 0; i < this->_ClientList.size(); i++)
        {
            if (senderFD == this->_ClientList[i].getFD())
                this->_ClientList[i].handleRequest(request);
        }
    }
    return 0;
}

int Server::sendResponseClient(int i, int j)
{
    std::string response = this->_ClientList[j].getResponse();
    std::string buffer;
    if (response.size() > BUFFSIZE)
    {
        std::string remain;
        buffer = response.substr(0, BUFFSIZE);
        remain = response.substr(BUFFSIZE);
        this->_ClientList[j].setResponse(remain);
    }
    else
    {
        buffer = response;
        this->_PollFDs[i].events &= ~POLLOUT;
    }
    this->_Status = send(_PollFDs[i].fd, buffer.c_str(), buffer.size(), 0);
    if (this->_Status == -1)
    {
        std::cout << "Error: send function issue for client " << this->_PollFDs[i].fd << std::endl;
        return (-1);
    }
    return (0);
}

void    Server::addToPollFDs(int clientFD)
{
    pollfd newFD = {clientFD, POLLIN, 0};
    this->_PollFDs.push_back(newFD);
}

void    Server::addToClientList(int clientFD, int portClient)
{
    for (size_t i = 0; i < this->_ClientList.size(); i++)
    {
        if (clientFD == this->_ClientList[i].getFD())
            return ;
    }
    for (size_t i = 0; i < this->_vData.size(); i++)
    {
        for (size_t j = 0; j < this->_vData[i]._ports.size(); j++)
        {
            if (portClient == this->_vData[i]._ports[j])
            {
                Client newClient(clientFD, this->_vData[i]);
    
                this->_ClientList.push_back(newClient);
                return ;
            }
        }
    }
}

void    Server::dellFromPollFDs(int FD)
{
    for (size_t i = 0; i < this->_PollFDs.size();)
    {
        if (FD == this->_PollFDs[i].fd)
        {
            close(this->_PollFDs[i].fd);
            this->_PollFDs.erase(this->_PollFDs.begin() + i);
        }
        else
            i++;
    }
}

void    Server::dellFromClientList(int FD)
{
    for (size_t i = 0; i < this->_ClientList.size();)
    {
        if (FD == this->_ClientList[i].getFD())
            this->_ClientList.erase(this->_ClientList.begin() + i);
        else
            i++;
    }
}

void    Server::dellAllSocket(void)
{
    for (size_t i = 0; i < this->_PollFDs.size(); i++)
    {
        close(this->_PollFDs[i].fd);
    }
    this->_ClientList.clear();
    this->_PollFDs.clear();
}

bool    Server::isServerFD(int FD)
{
    for (size_t i = 0; i < this->_ServerFDs.size(); i++)
    {
        if (FD == this->_ServerFDs[i])
            return (true);
    }
    return (false);
}

void Server::printOpenFDs()
{
    std::cout << "=== [DEBUG] Open File Descriptors ===" << std::endl;
    std::cout << "Tracked in _PollFDs:" << std::endl;

    for (size_t i = 0; i < this->_PollFDs.size(); ++i)
    {
        std::cout << "  FD: " << this->_PollFDs[i].fd;
        if (isServerFD(this->_PollFDs[i].fd))
            std::cout << " [Server FD]";
        else
            std::cout << " [Client FD]";
        std::cout << std::endl;
    }

    std::cout << "Tracked in _ClientList:" << std::endl;
    for (size_t i = 0; i < this->_ClientList.size(); ++i)
    {
        std::cout << "  Client FD: " << this->_ClientList[i].getFD() << std::endl;
    }

    std::cout << "Tracked in _ServerFDs:" << std::endl;
    for (size_t i = 0; i < this->_ServerFDs.size(); ++i)
    {
        std::cout << "  Server FD: " << this->_ServerFDs[i] << std::endl;
    }

    std::cout << "=====================================" << std::endl;
}

void    Server::sigHandler(int sig)
{
    if (Server::getInstance())
    {
        std::cout << "Catched a signal" << std::endl;
        Server::getInstance()->dellAllSocket();
        Server::getInstance()->_vData.clear();
        Server::getInstance()->_config.~ConfigParser();
        Server::getInstance()->_Ports.clear();
        Server::getInstance()->_ServerFDs.clear();
        Server::getInstance()->_PollFDs.clear();
        Server::getInstance()->_ClientList.clear();
    }
    exit (sig);
}

Server* Server::getInstance(void)
{
    return (_instance);
}