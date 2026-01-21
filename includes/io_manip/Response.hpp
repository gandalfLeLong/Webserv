#pragma once

#include <iostream>
#include <string>
#include "Request.hpp"

class Response
{
public:
	std::string	httpVersion;
	int	statusCode;
	std::string	StatusMess;
	std::map<std::string, std::string> headers;
	std::string	body;
	void	addHeader(std::string name, std::string value);
	void	addBodyHeader(std::string body);
	void	pathHandler(std::string	path);
	std::string	generateResponse();
	Response(Request);
	~Response();
	void output(void);
};