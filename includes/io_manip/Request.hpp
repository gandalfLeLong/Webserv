#pragma once

#include <iostream>
#include <string>

#include "Header.hpp"
#include <cctype>
#include <fstream>
#include <cstdlib>

class Request
{
private:
	std::string	_methode;
	std::string	_path;
	std::string	_httpVersion;
	std::string	_body;
	std::string	_errorMessage;
	int			_error;
	bool		_status;
	std::string	_filename;
	std::string	_boundary;
public:
	Header	_headers;
	Request();
	Request(std::string	brut);
	~Request();
	int	getError();
	bool	getStatus();
	std::string	getMethode();
	std::string	getVersion();
	std::string	getPath();
	std::string	getBody();
	std::string	getFilename();
	std::string getErrorMessage();
	std::map<std::string, std::string>	getHeaders();
	void	setError(int value);
	void	setErrorMess(std::string value);
	void	setBody(std::string body);
	std::string	deleteBoundary(const std::string &input);
	void	updatePath();
	void	parsePost();
	void	requestDone();
	void	printRequest();
	
	
	void	printHeader();
	void	requestVerify();
	void	output(void);
};
