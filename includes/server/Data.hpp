#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "ConfigParser.hpp"

class Data
{
	public :

	Data();
	Data(ConfigParser config, int i);
	Data(const Data& copy);
	~Data();

	Data&	operator=(const Data& copy);

	int					_maxRequestSize;
	std::string			_serverName;
	std::string			_root;
	std::string			_host;
	std::vector<int>	_ports;
	std::map<int, str>	_errorPages;
	std::vector<PrefixMatch_t>	_indexList;
};