
#include "Header.hpp"


char	&back(std::string &str)
{
    return str[str.size() - 1];
}

void	pop_back(std::string &str)
{
    if (!str.empty())
        str.erase(str.size() - 1, 1);
}

Header::Header()
{
	std::ifstream	file("requestTest.txt");
	std::string	line;
	size_t	colPos;

	std::getline(file, line);
	int	i;

	i = 0;
	_errorH = 0;
	while (std::getline(file, line))
	{
		if (line.size() == 1)
			break ;
		if (line.size() >= HEADER_SIZE || i >= HEADER_MAX)
		{
			setError(431, "Request Header Fields Too Large");
			return ;
		}
		if (!line.empty() && back(line) == '\r')
			pop_back(line);
		colPos = line.find(": ");
		if (colPos != std::string::npos)
		{
			std::string	name = line.substr(0, colPos);
			if (headers.find(name) != headers.end())
			{
				setError(400, "Bad Request");
				return ;
			}
			std::string	value = line.substr(colPos + 2);
			this->headers[name] = value;
		}
		else
		{
			setError(400, "Bad Request");
			return ;
		}
		i++;
	}
}


Header::Header(std::istringstream &file)
{
	std::string	line;
	size_t	colPos;
	int	i;

	i = 0;
	_errorH = 0;
	while (std::getline(file, line))
	{
		if (line.size() == 1)
			break ;
		if (line.size() >= HEADER_SIZE || i >= HEADER_MAX)
		{
			setError(431, "Request Header Fields Too Large");
			return ;
		}
		if (!line.empty() && back(line) == '\r')
			pop_back(line);
		colPos = line.find(": ");
		if (colPos != std::string::npos)
		{
			std::string	name = line.substr(0, colPos);
			if (this->headers.find(name) != this->headers.end())
			{
				setError(400, "Bad Request");
				return ;
			}
			std::string	value = line.substr(colPos + 2);
			this->headers[name] = value;
		}
		else
		{
			setError(400, "Bad Request");
			return ;
		}
		i++;
	}
}

Header::Header(std::ifstream &file)
{
	std::string	line;
	size_t	colPos;
	int	i;

	i = 0;
	_errorH = 0;
	while (std::getline(file, line))
	{
		if (line.size() == 1)
			break ;
		if (line.size() >= HEADER_SIZE || i >= HEADER_MAX)
		{
			setError(431, "Request Header Fields Too Large");
			return ;
		}
		if (!line.empty() && back(line) == '\r')
			pop_back(line);
		colPos = line.find(": ");
		if (colPos != std::string::npos)
		{
			std::string	name = line.substr(0, colPos);
			if (this->headers.find(name) != this->headers.end())
			{
				setError(400, "Bad Request");
				return ;
			}
			std::string	value = line.substr(colPos + 2);
			this->headers[name] = value;
		}
		else
		{
			setError(400, "Bad Request");
			return ;
		}
		i++;
	}
}

void	Header::setError(int err, std::string errM)
{
	_errorH = err;
	_errorMessageH = errM;
	return ;
}

void	Header::printHeader()
{
	for (std::map<std::string, std::string>::iterator i = this->headers.begin(); i != this->headers.end(); i++)
		std::cout << i->first << ": " << i->second << "\n";
}

int	Header::getErrorN()
{
	return (this->_errorH);
}

std::string	Header::getErrorM()
{
	if (this->_errorMessageH.size() > 1)
		return (this->_errorMessageH);
	return ("");
}

std::string	Header::getValue(std::string name)
{
	if (this->headers.find(name) != this->headers.end())
		return (this->headers[name]);
	return ("");
}

void	Header::addHeader(std::string name, std::string value)
{
	this->headers[name] = value;
}

void	Header::deleteHeader(std::string name)
{
	this->headers.erase(name);
}

std::string	Header::formatedHeader()
{
	std::string	formatedHeader;

	for (std::map<std::string, std::string>::iterator i = this->headers.begin(); i != this->headers.end(); i++)
		formatedHeader += i->first + ": " + i->second + "\r\n";
	return (formatedHeader);
}

Header::~Header()
{

}

void	Header::output(void)
{
	std::cout << "output Header" << std::endl;
}