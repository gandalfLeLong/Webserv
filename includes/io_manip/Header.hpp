
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>

const int	HEADER_SIZE = 8192;
const int	HEADER_MAX = 100;

class Header
{
private:
	int	_errorH;
	std::string	_errorMessageH;
public:
	std::map<std::string, std::string> headers;
	Header();
	Header(std::istringstream &file);
	Header(std::ifstream &file);
	~Header();
	int	getErrorN();
	std::string	getErrorM();
	std::string	getValue(std::string name);
	std::string	formatedHeader();
	void	setError(int err, std::string errM);
	void	addHeader(std::string name, std::string value);
	void	printHeader();
	void	deleteHeader(std::string name);
	void	output(void);
};

char	&back(std::string &str);
void	pop_back(std::string &str);
