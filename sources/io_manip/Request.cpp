#include "Request.hpp"

static std::vector<std::string>	split(const std::string& str, char delimiter) 
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter)) 
		tokens.push_back(token);
	return (tokens);
}

std::map<std::string, std::string>	Request::getHeaders()
{
	return (_headers.headers);
}

Request::Request()
{
	std::string	statusLine;
	std::string	bodyLine;
	std::ifstream	file("requestTest.txt");

	_status = false;
	std::getline(file, statusLine);

	if (!statusLine.empty() && back(statusLine) == '\r')
		pop_back(statusLine);

	std::vector<std::string> statusSplit = split(statusLine, ' ');
	Header	headers(file);

	_headers = headers;
	_error = headers.getErrorN();
	if (_error != 0)
	{
		_errorMessage = headers.getErrorM();
		return ;
	}
	if (statusSplit.size() == 3)
	{
		_methode = statusSplit[0];
		_path = statusSplit[1];
		_httpVersion = statusSplit[2];
	}
	else
	{
		_error = 400;
		_errorMessage = "Bad request";
		return ;
	}
	while (std::getline(file, bodyLine))
		_body += bodyLine + "\r\n";
	if (_methode == "POST")
	{
		parsePost();
	}
	if (getBody().find(_boundary + "--\r\n\r\n"))
		_status = true;
	requestVerify();
}

std::string Request::deleteBoundary(const std::string &input)
{
	if (this->_boundary.empty())
        return input;
	
	std::istringstream stream(input);
	std::ostringstream result;
	std::string line;

	while (std::getline(stream, line)) 
	{
		if (line.find(this->_boundary) == std::string::npos)
			result << line + "\n";
	}
	return result.str();
}

void	Request::updatePath()
{
	size_t	start = this->_body.find("filename=\"");

	if (start == std::string::npos)
		return ;
	start += 10;
	size_t	end = this->_body.find("\"", start);

	if (end == std::string::npos)
		return ;
	this->_path = _path + this->_body.substr(start, end - start);
}

void	Request::printRequest()
{
	std::cout << this->getPath() << "\n\n\n";
	this->printHeader();
	std::cout << std::endl << this->getBody() << std::endl << std::endl << std::endl << std::endl << std::endl;
}

Request::Request(std::string brut)
{
	std::istringstream	file(brut);
	std::string	statusLine;
	std::string	bodyLine;

	_status = true;
	std::getline(file, statusLine);
	
	if (!statusLine.empty() && back(statusLine) == '\r')
		pop_back(statusLine);
	
	std::vector<std::string> statusSplit = split(statusLine, ' ');
	Header	headers(file);
	
	_headers = headers;
	_error = headers.getErrorN();
	if (_error != 0)
	{
		_errorMessage = headers.getErrorM();
		return ;
	}
	if (statusSplit.size() == 3)
	{
		_methode = statusSplit[0];
		_path = statusSplit[1];
		_httpVersion = statusSplit[2];
	}
	else
	{
		_error = 400;
		_errorMessage = "Bad request";
		return ;
	}
	while (std::getline(file, bodyLine))
	{
		_body += bodyLine;
		if (!file.eof())
			_body += "\n";
	}
	if (_methode == "POST")
	{
		parsePost();
		requestDone();
		//std::cout << "\n\n\nBODY SIZE : " << this->getBody().size() << "\n" << this->getBody() << "\n\n\n";
	}
	if (_status == true && _methode == "POST")
	{
		this->_body = deleteBoundary(this->_body);
		updatePath();
	}
	// this->printRequest();
	requestVerify();
}

void	Request::requestDone()
{
	if (!(this->getBody().size() == (size_t)atoi(this->_headers.headers["Content-Length"].c_str())))
		_status = false;
}

void	Request::parsePost()
{
	std::string contentType = this->_headers.headers["Content-Type"];
	size_t boundaryPos = contentType.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		_boundary = contentType.substr(boundaryPos + 9);
		contentType.erase(boundaryPos - 2);
		this->_headers.headers["Content-Type"] = contentType;
	}
	/*
	if (this->_headers.headers["Content-Type"].find("boundary="))
	{
		_boundary = this->_headers.headers["Content-Type"].substr(this->_headers.headers["Content-Type"].find("boundary=") + 9);
		this->_headers.headers["Content-Type"].erase(this->_headers.headers["Content-Type"].find("boundary=") - 2);
	}*/
}

std::string	Request::getVersion()
{
	if (this->_httpVersion.size() > 1)
		return (this->_httpVersion);
	return ("");
}

std::string	Request::getMethode()
{
	if (this->_methode.size() >= 1)
		return (this->_methode);
	return("");
}

std::string	Request::getPath()
{
	if (this->_path.size() >= 1)
		return (this->_path);
	return ("");
}

std::string	Request::getBody()
{
	if (this->_body.size() >= 1)
		return (this->_body);
	return ("");
}

void	Request::printHeader()
{
	this->_headers.printHeader();
}

static bool	isNum(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (!std::isdigit(str[i]))
			return (0);
	return (1);
}

// static bool	checkLength(std::string	body, std::string length)
// {
// 	std::cout << "SIZE : " << body.size() << " C-L : " << std::stoi(length) << "\n\n";
// 	if (body.size() != (size_t)std::stoi(length))
// 		return (0);
// 	return (1);
// }

void	Request::requestVerify()
{
	if (this->_error != 0)
			return ;
	if (this->_methode != "GET" && this->_methode != "DELETE" && this->_methode != "POST")
	{
		_error = 405;
		_errorMessage = "Methode Not Allowed";
		return ;
	}
	else if (this->_httpVersion != "HTTP/1.1")
	{
		_error = 505;
		_errorMessage = "HTTP Version Not Supported";
		return ;
	}
	else if (this->_httpVersion == "HTTP/1.1" && this->_headers.headers.find("Host") == this->_headers.headers.end())
	{
		_error = 400;
		_errorMessage = "Bad Request";
		return ;
	}
	else if (this->_headers.headers.find("Content-Length") != this->_headers.headers.end() && this->_methode == "POST")
	{
		if (!isNum(this->_headers.headers["Content-Length"])
			|| this->_headers.headers.find("Content-Type") == this->_headers.headers.end()/*
			|| !checkLength(this->_body, this->_headers.headers["Content-Length"])*/)
		{
			_error = 400;
			_errorMessage = "Bad Request";
			return ;
		}
	}
	else if (this->_headers.headers.find("Host") == this->_headers.headers.end())
	{
		_error = 400;
		_errorMessage = "Bad Request";
		return ;
	}
	else if (this->_methode == "POST" && this->_headers.headers.find("Content-Length") == this->_headers.headers.end())
	{
		_error = 411;
		_errorMessage = "Length Required";
		return ;
	}
	/*error with client_max_body_size when parts linked*/
	/*Parse-Body*/
	
}

int	Request::getError()
{
	return (this->_error);
}

std::string	Request::getErrorMessage()
{
	if (this->_errorMessage.size() > 1)
		return (this->_errorMessage);
	return ("");
}

Request::~Request()
{

}

void	Request::output(void)
{
	std::cout << "output Request" << std::endl;
}

void	Request::setError(int value)
{
	this->_error = value;
}

void	Request::setErrorMess(std::string value)
{
	this->_errorMessage = value;
}

bool	Request::getStatus(void)
{
	return (this->_status);
}

void	Request::setBody(std::string body)
{
	this->_body = body;
}

std::string	Request::getFilename(void)
{
	return (this->_filename);
}