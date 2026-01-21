#include "Data.hpp"

Data::Data()
{

}

Data::Data(ConfigParser config, int i)
{   
    this->_maxRequestSize = config.server[i].max_body_size;
    this->_serverName = config.server[i].server_name;
    this->_root = config.server[i].root_;
    this->_errorPages = config.server[i].error_page;
    for (int j = 0; j < config.server[i].prefixCount; j++)
    {
        PrefixMatch_t copy;
        copy = config.server[i].prefix[j];
        this->_indexList.push_back(copy);
    }
	for (size_t k = 0; k < config.server[i].listen_port.size(); k++)
	{
		this->_ports.push_back(config.server[i].listen_port[k]);
	}
}

Data::Data(const Data& copy)
{
    *this = copy;
}

Data&	Data::operator=(const Data& copy)
{
    if (this == &copy)
        return (*this);
    _maxRequestSize = copy._maxRequestSize;
    _serverName = copy._serverName;
    _root = copy._root;
    _host = copy._host;
    _ports = copy._ports;
    _errorPages = copy._errorPages;
    _indexList = copy._indexList;
    return (*this);
}

Data::~Data()
{

}