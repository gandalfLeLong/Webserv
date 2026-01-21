#include "ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	this->serverCount = 0;
}

ConfigParser::~ConfigParser()
{
	
}

int	ConfigParser::fileIteration(std::ifstream& sourceFile, int index, str file, bool gb)
{
	str nextLine;
	int i = index;
	int a = 0;

	while (std::getline(sourceFile, nextLine))
	{
		i++;
		if (nextLine.empty())
			continue;
		if ((a = afterMainBracketCheck(nextLine)) != -1)
		{
			std::cout << "\033[31mWARNING: \033[0m Maximum amount of server you can have is 10 (Exceeding is an error!)" << std::endl;
			std::cout << "\033[31mFile name: " << "\""  
			  << file << "\"" << " -- line " 
			  << i << " -- character " << a
			  << " (Please review the file!)\033[0m" << std::endl;
			  std::cout << std::endl;
			return (1);
		}
	}
	if (gb == false)
		return (1);
	else
		return (0);
}

int	ConfigParser::afterMainBracketCheck(str line)
{
	int	a = 0;
	while (true)
	{
		if (line[a] == '\0' || line[a] == '#')
			break ;
		if (!isspace(line[a]))
		{
			std::cout << "\033[31mError: \033[0mRandom character(s) detected" << std::endl;
			return (a + 1);
		}
		a++;
	}
	return (-1);
}

int	ConfigParser::mainBracketsCheck(int index, ServerInfo& server)
{
	if (server.srvr == true && server.cBracket > 1 && server.oBracket == 1)
	{
		std::cout << "\033[31mError: \033[0mDetected excessive closing brackets \033[31m-- line -- " << index << "\033[0m" << std::endl;
		return (1);
	}
	else if (server.srvr == true && server.cBracket >= 1 && server.oBracket == 0)
	{
		std::cout << "\033[31mError: \033[0mDetected excessive closing brackets or random closing bracket \033[31m-- line -- " << index << "\033[0m" << std::endl;
		return (1);
	}
	// else if (server.srvr == true && server.cBracket == 1 && server.oBracket > 1)
	// {
	// 	std::cout << "\033[31mError: \033[0mDetected excessive opening brackets \033[31m-- line -- " << index << "\033[0m" << std::endl;
	// 	return (1);
	// }
	// else if (server.srvr == true && server.cBracket == 0 && server.oBracket > 1)
	// {
	// 	std::cout << "\033[31mError: \033[0mDetected excessive opening brackets \033[31m-- line -- " << index << "\033[0m" << std::endl;
	// 	return (1);
	// }
	else if (server.srvr == true && server.cBracket > 1 && server.oBracket > 1)//Problem might come from here
	{
		std::cout << "\033[31mError: \033[0mCheck curly brackets excess of brackets \033[31m-- line -- " << index << "\033[0m" << std::endl;
		return (1);
	}
	return (0);
}

int	ConfigParser::posPositionCheck(str line, size_t pos)
{
	// if (pos == 1)
	// {
	// 	if (line[0] != '\t')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mNot enough space or tab infront of a keyword" << std::endl;
	// 		return (1);
	// 	}
	// }
	if (pos == 4)
	{
		//std::cout << line[0] << "." << line[1] << "." << line[2] << "." << line[3] << "." << std::endl; 
		if (line[0] != ' ' || line[1] != ' ' || line[2] != ' ' || line[3] != ' ')
		{	
			std::cout << "\033[31mError: \033[0mMix of tabs and spaces or random character/s, else more than 1 tab infront of a keyword" << std::endl;
			return (1);
		}
	}
	else if (pos == 8)
	{
		if (line[0] != ' ' || line[1] != ' ' || line[2] != ' ' || line[3] != ' ' || line[4] != ' ' || line[5] != ' ' || line[6] != ' ' || line[7] != ' ')
		{	
			std::cout << "\033[31mError: \033[0mMix of tabs and spaces or random character/s, else more than 1 tab infront of a keyword" << std::endl;
			return (1);
		}
	}
	return (0);
}

int	ConfigParser::pCheck(str line, size_t tmp)
{
	if (tmp != 0 && tmp != std::string::npos)
	{
		for (size_t i = 0; i < tmp; i++)
		{
			if (!isspace(line[i]))
			{
				// if (line[i] == '}')
				// {
				// 	if (i == 4)
				// 		this->kwCBracket += 1;//will come back to it for kwlocation function!
				// 	else
				// 	{
				// 		std::cout << "\033[31mError: \033[0mCheck closing curly bracket positioning" << std::endl;
				// 		return (1);
				// 	}
				// }
				// else if (line[i] == '{')
				// {
				// 	if (i == 4)
				// 		this->kwOBracket += 1;
				// 	else
				// 	{
				// 		std::cout << "\033[31mError: \033[0mCheck closing curly bracket positioning" << std::endl;
				// 		return (1);
				// 	}
				// }
				// else
				// {

				// }
				std::cout << "\033[31mError: \033[0mNot known character(s) in line" << std::endl;
				return (1);
			}
		}
	}
	return (0);
}

int	ConfigParser::prevCheck(str line, size_t tmp, ServerInfo& server)
{
	if (tmp != 0 && tmp != std::string::npos)
	{
		for (size_t i = 0; i < tmp; i++)
		{
			if (!isspace(line[i]))
			{
				if (line[i] == '}')
					server.cBracket += 1;
				else if (line[i] == '{')
					server.oBracket += 1;
				else
				{
					std::cout << "\033[31mError: \033[0mNot known character(s) in line" << std::endl;
					return (1);
				}
			}
		}
	}
	return (0);
}

int	ConfigParser::spaceTabCheck(str line, ServerInfo& server)
{
	int i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ')
		{
			if (line[i] == '}')
				server.cBracket += 1;
			else if (line[i] == '{')
				server.oBracket += 1;
			else
			{
				std::cout << "\033[31mError: \033[0mRandom character in line" << std::endl;
				return (i + 1);
			}
		}
		i++;
	}
	return (-1);
}

int	ConfigParser::oBracketPosCheck(ServerInfo& server, size_t actualPos, int indexToCheck)
{
	if (indexToCheck == 0)
	{	
		if (server.oBracket == 0)
		{
			if (actualPos != 0)
				return (1);
		}
		else if (server.oBracket > 0)
		{
			if (actualPos != server.oBracket * 4)
				return (1);
		}
	}
	else if (indexToCheck == 1)
	{
		if (actualPos != 4)
			return (1);
		//TODO: 
	}
	else if (indexToCheck == 2)
	{
		if (server.oBracket != 0 && server.prefix[server.prefixCount - 1].kwOBracket != 0)
		{
			if (actualPos != server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4)
				return (1);
		}
		else if (server.oBracket != 0 && server.prefix[server.prefixCount - 1].kwOBracket == 0)
		{
			if (actualPos != server.oBracket * 4)
				return (1);
		}
		else
		{
			std::cout << "\033[31mError: \033[0mNot known behaviour" << std::endl;
			return (1);
		}
	}
	return (0);
}

int	ConfigParser::cBracketPosCheck(ServerInfo& server, size_t actualPos, int indexToCheck)
{
	if (indexToCheck == 0)
	{
		if (server.oBracket != 0 && server.cBracket == 0)
		{
			if (actualPos != server.oBracket * 4 - 4)
				return (1);

		}
		else if (server.oBracket != 0 && server.cBracket != 0)
		{
			if (actualPos != server.oBracket * 4 - 4 - server.cBracket * 4)
				return (1);
		}
	}
	else if (indexToCheck == 1)
	{
		if (actualPos != 4)
			return (1);
		//TODO: 
	}
	else if (indexToCheck == 2)
	{
		if (server.oBracket != 0 && server.prefix[server.prefixCount - 1].kwOBracket != 0 && server.prefix[server.prefixCount - 1].kwCBracket != 0)
		{
			// std::cout << "TEST: " << actualPos << std::endl;
			// std::cout << "oBracket: " << server.oBracket * 4 << std::endl;
			// std::cout << "kwOBracket: " << server.prefix[server.prefixCount - 1].kwOBracket * 4 << std::endl;
			// std::cout << "kwCBracket: " << server.prefix[server.prefixCount - 1].kwCBracket * 4 << std::endl;
			// std::cout << "VALUE: " << server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4 - 4 - server.prefix[server.prefixCount - 1].kwCBracket * 4 << std::endl;
			if (actualPos != server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4 - 4 - server.prefix[server.prefixCount - 1].kwCBracket * 4)
				return (1); 
		}
		else if (server.oBracket != 0 && server.prefix[server.prefixCount - 1].kwOBracket != 0 && server.prefix[server.prefixCount - 1].kwCBracket == 0)
		{
			if (actualPos != server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4 - 4)
				return (1);
		}
		else//Continue work from here!
		{
			std::cout << "\033[31mError: \033[0mNot known behaviour" << std::endl;
			return (1);
		}	
	}
	return (0);
}

int	ConfigParser::nextCheck(str line, size_t pos, ServerInfo& server)
{
	int	a = 0;
	int i = 0;
	while (true)
	{
		if (line[pos + a] == '\0' || line[pos + a] == '#')
			break ;
		if (!isspace(line[pos + a]))
		{
			if (line[pos + a] == '}')
			{
				if (cBracketPosCheck(server, pos + a, 0) != 0 && server.srvr == true)
				{
					std::cout << "\033[31mError: \033[0mCheck closing curly bracket positioning" << std::endl;
					return (pos + a + 1);
				}
				else if (server.srvr == false)
				{
					std::cout << "\033[31mError: \033[0mParser was not able to find keyword \033[31m->SERVER\033[0m (Cannot recognize curly bracket!)" << std::endl;
					return (pos + a + 1);
				}
				server.cBracket += 1;
			}
			else if (line[pos + a] == '{')
			{
				if (oBracketPosCheck(server, pos + a, 0) != 0 && server.srvr == true)
				{
					std::cout << "\033[31mError: \033[0mCheck opening curly bracket positioning" << std::endl;
					return (pos + a);
				}
				else if (server.srvr == false)
				{
					server.srvr = true;
					if ((i = nextCheck(line, pos + a + 1, server)) != -1)
						return (i);
				}
				server.oBracket += 1;
			}
			else
			{
				std::cout << "\033[31mError: \033[0mRandom character(s) detected" << std::endl;
				return (pos + a + 1);
			}
		}
		a++;
	}
	return (-1);
}

int	ConfigParser::emptySpaceCheck(str line, size_t pos, size_t index)
{
	if (pos < index)
	{
		for (size_t i = 0; i < index; i++)
		{
			if (!isspace(line[i]) && pos != 0)
			{
				std::cout << "\033[31mError: \033[0mCharacter detected infront of a keyword (Check well please!)" << std::endl;
				return (i);
			}
			else
			{
				std::cout << "\033[31mError: \033[0mSpaces missing infront of a keyword (One tab or 4 spaces please!)" << std::endl;
				return (pos);
			}
		}
	}
	else if (pos > index && pos != std::string::npos)
	{
		size_t i = 0;
		while (i < pos)
		{
			if (!isspace(line[i]))
			{
				std::cout << "\033[31mError: \033[0mCharacter detected infront of a keyword (Check well please!)" << std::endl;
				return (i);
			}
			else
			{
				std::cout << "\033[31mError: \033[0mExcessive spaces or tabs infront of a keyword (Check well please!)" << std::endl;
				return (pos);
			}
			i++;
		}
	}
	else
	{
		std::cout << "\033[31mError: \033[0mPositional issue" << std::endl;
		return (pos);
	}
	return (pos);
}

int	ConfigParser::kwServer_oB(str line, ServerInfo& server)
{
	std::size_t pos;
	std::size_t tmp;
	int			a;
	
	tmp = line.find('#');
	pos = line.find("server");
	if (tmp < pos)
	{
		pos = std::string::npos;
		if (prevCheck(line, tmp, server) != 0)
			return (1);
	}
	else if (tmp == std::string::npos && pos == std::string::npos)
	{
		if ((a = spaceTabCheck(line, server)) != -1)
			return (a);
	}
	else if (pos != 0 && pos != std::string::npos)
	{
		std::cout << "\033[31mError: \033[0mCharacter or empty space is infront of keyword server" << std::endl;
		return (1);
	}
	else if ((a = nextCheck(line, pos + 6, server)) != -1 && pos != std::string::npos)
	{
		std::cout << "\033[31mError: \033[0mUnrecognized character detected after a keyword" << std::endl;
		return (a);
	}
	else if (pos != std::string::npos)
		server.srvr = true;
	if (server.srvr == 0 && server.oBracket > 0)
	{
		std::cout << "\033[31mError: \033[0mKeyword \033[31m->\"server\"\033[0m not found in config file and random opening curly bracket(s) found" << std::endl;
		return (1);
	}
	if (server.srvr == 0 && server.cBracket > 0)
	{
		std::cout << "\033[31mError: \033[0mKeyword \033[31m->\"server\"\033[0m not found in config file and random closing curly bracket(s) found" << std::endl;
		return (1);
	}
	return (-1);
}

enum kwtypeMainBlock
{
	HOST,
	LISTEN,
	SERVER_NAME,
	BODY_SIZE,
	ROOT,
	ERROR_PAGE,
	LOCATION,
	UNKNOWN
};

std::map<str, kwtypeMainBlock> initializekwMap() 
{
    std::map<std::string, kwtypeMainBlock> kw;
    kw["host"] = HOST;
    kw["server_name"] = SERVER_NAME;
    kw["listen"] = LISTEN;
	kw["client_max_body_size"] = BODY_SIZE;
	kw["error_page"] = ERROR_PAGE;
	kw["root"] = ROOT;
	kw["location"] = LOCATION;
    return kw;
}

int	ConfigParser::numberfy_kw(str extract)
{
	static std::map<str, kwtypeMainBlock> kw = initializekwMap();
    
    std::map<str, kwtypeMainBlock>::iterator it = kw.find(extract);
    if (it != kw.end()) 
        return it->second;
    return UNKNOWN;
}

int	ConfigParser::check_kw_mainBlock(str line)
{
	str extract;
	int	i = 0;

	while (line[i] != '\0' && line[i] == ' ')
		i++;
	while (line[i] != '\0' && (isalnum(line[i]) || line[i] == '_'))
	{
		extract += line[i];
		i++;
	}
	switch (numberfy_kw(extract))
	{
		case HOST:
			return HOST;
		case LISTEN:
			return LISTEN;
		case SERVER_NAME:
			return SERVER_NAME;
		case BODY_SIZE:
			return BODY_SIZE;
		case ROOT:
			return ROOT;
		case ERROR_PAGE:
			return ERROR_PAGE;
		case LOCATION:
			return LOCATION;//for Location Block
		case UNKNOWN:
			return (UNKNOWN);
	}
	return (-1);
}

int	ConfigParser::kwListenPort(str line, ServerInfo& server)
{
	std::size_t pos;
	std::size_t tmp;
	int	int_extract;
	str	extract;
	int int_error_code = 0;
	str	error_code;
	int	s_or_l = 0;
	int g = 0;
	int y = 0;

	g = check_kw_mainBlock(line);
	if (g == LISTEN)
		pos = line.find("listen");
	else if (g == SERVER_NAME)
	{
		pos = line.find("server_name");
		s_or_l = 1;
	}
	else if (g == BODY_SIZE)
	{
		pos = line.find("client_max_body_size");
		s_or_l = 2;
	}
	else if (g == ROOT)
	{
		pos = line.find("root");
		s_or_l = 3;
	}
	else if (g == ERROR_PAGE)
	{
		pos = line.find("error_page");
		s_or_l = 4;
	}
	else if (g == HOST)
	{
		pos = line.find("host");
		s_or_l = 5;
	}
	else if (g == LOCATION)
	{
		g = kwLocation(line, server);
		if (g == -1)
			return (-1);
		else
			return (g);
	}
	else
		pos = std::string::npos;

	tmp = line.find('#');
	int a;
	if (pos == server.oBracket * 4)
	{
		if ((a = posPositionCheck(line, pos)) != 0)

			return (a);
	}
	a = 1;
	if (tmp < pos)
	{
		pos = std::string::npos;
		if (prevCheck(line, tmp, server) != 0)
			return (1);
	}
	else if (pos != server.oBracket * 4 && pos != std::string::npos)
		return (emptySpaceCheck(line, pos, server.oBracket * 4));
	else if (pos != std::string::npos)
	{
		if (s_or_l == 0)//listen_port

		{
			while (line[pos + 5 + a] != '\0')
			{
				while (line[pos + 5 + a] == ' ')
					a++;
				if (line[pos + 5 + a - 1] == 'n')
				{
					std::cout << "\033[31mError: \033[0mKeyword and value should be separated by a space(s)" << std::endl;
					return (pos + 5 + a + 1);
				}
				while (isdigit(line[pos + 5 + a]))
				{
					//Find a way to take all the character positions and create a string
					extract += line[pos + 5 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find value for variable \033[31m->listen\033[0m" << std::endl;
					return (pos + 9 + a);
				}

				if (!isdigit(line[pos + 5 + a]) && line[pos + 5 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mNon digit character detected as a port number" << std::endl;
					return (pos + 5 + a);
				}
				if (line[pos + 5 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value (e.g 8080;)" << std::endl;
					return (pos + 5 + a);
				}
				if ((a = (nextCheck(line, pos + 6 + a, server))) != -1)
					return (a);
				break ;
			}
			int_extract = atoi(extract.c_str());
			server.listen_b = true;
			server.listen_port.push_back(int_extract);
		}
		else if (s_or_l == 1)//server_name
		{
			if (server.srvr_name_b == true)
			{
				std::cout << "\033[31mError: \033[0mServer name is already registered (more than one server_name is unacceptable)" << std::endl;
				return (pos);
			}

			while (line[pos + 10 + a] != '\0')
			{
				while (line[pos + 10 + a] == ' ')
					a++;
				if (line[pos + 10 + a - 1] == 'e')
				{
					std::cout << "\033[31mError: \033[0mKeyword and value should be separated by a space(s)" << std::endl;
					return (pos + 10 + a + 1);
				}
				while (isalnum(line[pos + 10 + a]) || ispunct(line[pos + 10 + a]))
				{
					//Find a way to take all the character positions and create a string
					if (line[pos + 10 + a] == ';')
						break;
					extract += line[pos + 10 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find value for variable \033[31m->server_name\033[0m" << std::endl;
					return (pos + 9 + a);
				}
				if (!isalnum(line[pos + 10 + a]) && !ispunct(line[pos + 10 + a]))
				{
					std::cout << line[pos + 10 + a] << std::endl;
					std::cout << "\033[31mError: \033[0mDomain name cannot fulfill requirement" << std::endl;
					return (pos + 10 + a);
				}
				if (line[pos + 10 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after domain name (e.g mywebsite.com;)" << std::endl;
					return (pos + 10 + a);
				}
				if ((a = (nextCheck(line, pos + 11 + a, server))) != -1)
					return (a);
				break ;
			}
			server.srvr_name_b = true;
			server.server_name = extract;
		}
		else if (s_or_l == 2)//body size
		{
			if (server.body_size_b == true)
			{
				std::cout << "\033[31mError: \033[0mClient max body size is already registered (more than one client max body size is unacceptable)" << std::endl;
				return (pos);
			}
			while (line[pos + 19 + a] != '\0')
			{
				while (line[pos + 19 + a] == ' ')
					a++;
				while (line[pos + 19 + a] != ' ' && line[pos + 19 + a] != '\0')
				{
					if (line[pos + 19 + a] == ';')
						break;
					extract += line[pos + 19 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find value for variable \033[31m->client_max_body_size\033[0m" << std::endl;
					return (pos + 19 + a);
				}
				while (extract[y] != '\0')
				{
					if (!isdigit(extract[y]))
					{
						std::cout << "\033[31mError: \033[0mNon digit character found from max body size (Only accepted formet is numbers)" << std::endl;
						return (pos + 19);
					}
					y++;
				}
				if (y > 9)
				{
					std::cout << "\033[31mError: \033[0mPlease keep in mind that MAX body size cannot exceed 9 digit number" << std::endl;
					return (pos + 19 + a);
				}
				if (line[pos + 19 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value (e.g 8080;)" << std::endl;
					return (pos + 19 + a);
				}
				if ((a = (nextCheck(line, pos + 20 + a, server))) != -1)
					return (a);
				break ;
			}
			int_extract = atoi(extract.c_str());
			if (int_extract < 5000)
			{
				std::cout << "\033[31mError: \033[0mCannot be less than 5000 \033[31m->client_max_body_size\033[0m" << std::endl;
				return (pos + 19 + a);
			}
			server.body_size_b = true;
			server.max_body_size = int_extract;
		}
		else if (s_or_l == 3)//root (Can we receive more than one root?)
		{
			while (line[pos + 3 + a] != '\0')
			{
				while (line[pos + 3 + a] == ' ')
					a++;
				if (line[pos + 3 + a - 1] == 't')
				{
					std::cout << "\033[31mError: \033[0mKeyword and value should be separated by a space(s)" << std::endl;
					return (pos + 3 + a + 1);
				}
				while (isalnum(line[pos + 3 + a]) || ispunct(line[pos + 3 + a]) || line[pos + 3 + a] == '/')
				{
					//Find a way to take all the character positions and create a string
					if (line[pos + 3 + a] == ';')
						break;
					extract += line[pos + 3 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find value for variable \033[31m->root\033[0m" << std::endl;
					return (pos + 9 + a);
				}
				if (!isalnum(line[pos + 3 + a]) && !ispunct(line[pos + 3 + a]))
				{
					std::cout << line[pos + 3 + a] << std::endl;
					std::cout << "\033[31mError: \033[0mRoot directory cannot fulfill requirement" << std::endl;
					return (pos + 3 + a);
				}
				if (line[pos + 3 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value (e.g mywebsite.com;)" << std::endl;
					return (pos + 3 + a);
				}
				if ((a = (nextCheck(line, pos + 4 + a, server))) != -1)
					return (a);
				break ;
			}
			server.root_dir_b = true;
			server.root_ = extract;
		}
		else if (s_or_l == 4)//Error_Page
		{
			while (line[pos + 9 + a] != '\0')
			{
				while (line[pos + 9 + a] == ' ' && line[pos + 9 + a] != '\0')
					a++;
				while (isdigit(line[pos + 9 + a]) && line[pos + 9 + a] != '\0')
				{
					error_code += line[pos + 9 + a];
					a++;
				}
				if (error_code.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find error code from varibale \033[31m->error_page\033[0m" << std::endl;
					return (pos + 9 + a);
				}
				while (line[pos + 9 + a] == ' ' && line[pos + 9 + a] != '\0')
					a++;
				if (isdigit(line[pos + 9 + a - 1]))
				{
					std::cout << "\033[31mError: \033[0mVariable and value should be separated by a space(s)" << std::endl;
					return (pos + 9 + a + 1);
				}
				while (isalnum(line[pos + 9 + a]) || ispunct(line[pos + 9 + a]) || line[pos + 9 + a] == '/')//check what if extract is empty
				{
					if (line[pos + 9 + a] == ';')
						break;
					extract += line[pos + 9 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find html file path" << std::endl;
					return (pos + 9 + a);
				}
				if (!isalnum(line[pos + 9 + a]) && !ispunct(line[pos + 9 + a]) && line[pos + 9 + a] != '/')
				{
					std::cout << "\033[31mError: \033[0mError_page variale cannot fulfill requirement" << std::endl;
					return (pos + 9 + a);
				}
				if (line[pos + 9 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value (e.g mywebsite.com;)" << std::endl;
					return (pos + 9 + a);
				}
				if ((a = (nextCheck(line, pos + 10 + a, server))) != -1)
					return (a);
				break ;
			}
			server.error_b = true;
			int_error_code = atoi(error_code.c_str());
			server.error_page[int_error_code] = extract;
		}
		else if (s_or_l == 5)//host name
		{
			if (server.host_name_b == true)
			{
				std::cout << "\033[31mError: \033[0mHost IPv4 address already registered (more than one host address is unacceptable)" << std::endl;
				return (pos);
			}
			while (line[pos + 3 + a] != '\0')
			{
				while (line[pos + 3 + a] == ' ')
					a++;
				if (line[pos + 3 + a - 1] == 'n')
				{
					std::cout << "\033[31mError: \033[0mKeyword and value should be separated by a space(s)" << std::endl;
					return (pos + 3 + a + 1);
				}
				while (isdigit(line[pos + 3 + a]) || line[pos + 3 + a] == '.')
				{
					//Find a way to take all the character positions and create a string
					extract += line[pos + 3 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mCould not find IPv4 address for variable \033[31m->host\033[0m" << std::endl;
					return (pos + 9 + a);
				}
				if (!isdigit(line[pos + 3 + a]) && line[pos + 3 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mNon digit character detected" << std::endl;
					return (pos + 3 + a);
				}
				if (line[pos + 3 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value (e.g 8080;)" << std::endl;
					return (pos + 3 + a);
				}
				if ((a = (nextCheck(line, pos + 4 + a, server))) != -1)
					return (a);
				break ;
			}
			if (extract == "127.0.0.1")
			{
				server.host_name_b = true;
				server.host = extract;
			}
			else
			{
				std::cout << "\033[31mError: \033[0mAcceptable IPv4 address is only loopback address (127.0.0.1)" << std::endl;
				return (pos + 3 + a);
			}
		}
	}
	else if (pos == std::string::npos && tmp == std::string::npos)
	{
		if ((a = nextCheck(line, 0, server)) != -1)
			return (a);
	}
	return (-1);
}

int	ConfigParser::getLocMatchType(str line, ServerInfo& server, int index)
{
	str locBlockExtract;
	// if (line[index] == '=')
	// {
	// 	index = index + 1;
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 		index++;
	// 	if (line[index] != '/')
	// 		return (index + 1);
	// 	while (line[index] != ' ' && line[index] != '\0')
	// 	{
	// 		locBlockExtract += line[index];
	// 		index++;
	// 	}
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 		index++;
	// 	if (line[index] != '\0' && line[index] != '#')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mRandom character/s found" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	server.exactMatch[server.equalCount] = 2;
	// 	server.equal[server.equalCount].match = locBlockExtract;//after taking the root do not forget to iterate the equalCount!!!!
	// 	server.equalCount += 1;
	// 	return (0);
	// }
	if (line[index] == '/')
	{
		if (server.prefixCount == 10)
		{
			std::cout << "\033[31mError: \033[0mMaximum possible number of location directives are 10 (Exceeding is an error!)" << std::endl;
			return (index + 1);
		}
		if (line[index] == '/' && line[index + 1] != ' ' && line[index + 1] != '{' && line[index + 1] != '\0')
		{
			while (line[index] != ' ' && line[index] != '\0' && line[index] != '{')
			{
				locBlockExtract += line[index];
				index++;
			}
			while (line[index] != '\0' && (line[index] == ' ' || line[index] == '{'))
			{
				if (line[index] == '{')
					server.prefix[server.prefixCount].kwOBracket += 1;
				index++;
			}
			if (line[index] != '\0' && line[index] != '#' && line[index] != '{')
			{
				std::cout << "\033[31mError: \033[0mRandom character/s found" << std::endl;
				return (index + 1);
			}
			server.prefixMatch[server.prefixCount] = 2;
			server.prefix[server.prefixCount].match = locBlockExtract;
			server.prefixCount += 1;

			return (0);
		}
		else if (line[index] == '/' && (line[index + 1] == ' ' || line[index + 1] == '{' || line[index + 1] == '\0'))
		{
			if (server.prefixCount == 10)
			{
				std::cout << "\033[31mError: \033[0mMaximum possible number of location directives are 10 (Exceeding is an error!)" << std::endl;
				return (index + 1);
			}
			index += 1;
			while (line[index] != '\0' && (line[index] == ' ' || line[index] == '{'))
			{
				if (line[index] == '{')
					server.prefix[server.prefixCount].kwOBracket += 1;
				index++;
			}
			if (line[index] != '\0' && line[index] != '#' && line[index] != '{')
			{
				std::cout << "\033[31mError: \033[0mRandom character/s found" << std::endl;
				return (index + 1);
			}
			server.prefixMatch[server.prefixCount] = 2;
			server.prefix[server.prefixCount].match = "/";
			server.prefixCount += 1;
			return (0);
		}
	}
	// else if (line[index] == '~' && line[index + 1] == '*')//Case-Insensitive
	// {
	// 	index = index + 2;
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 		index++;
	// 	if (line[index - 1] == '*')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mCase-Insensitive match space missing!" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	if (line[index] != '\\' && line[index] != '/')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mCase-Insensitive match should be followed by a backslash or a slash" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	while (line[index] != ' ' && line[index] != '\0')
	// 	{
	// 		locBlockExtract += line[index];
	// 		index++;
	// 	}
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 			index++;
	// 	if (line[index] != '\0' && line[index] != '#')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mRandom character/s found" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	server.caseInsenMatch[server.caseInsenCount] = 2;//2 means that it needs its values now after it will be 1
	// 	server.caseInsen[server.caseInsenCount].match = locBlockExtract;
	// 	server.caseInsenCount += 1;
	// 	return (0);
	// }
	// else if (line[index] == '~' && line[index + 1] == ' ')
	// {
	// 	index += 1;
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 		index++;
	// 	if (line[index - 1] == '~')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mRegex/Case-Sensitive match space missing!" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	if (line[index] != '\\' && line[index] != '/')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mRegex/Case-Sensitive match should be followed by a backslash or a slash" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	while (line[index] != ' ' && line[index] != '\0')
	// 	{
	// 		locBlockExtract += line[index];
	// 		index++;
	// 	}
	// 	while (line[index] != '\0' && line[index] == ' ')
	// 			index++;
	// 	if (line[index] != '\0' && line[index] != '#')
	// 	{
	// 		std::cout << "\033[31mError: \033[0mRandom character/s found" << std::endl;
	// 		return (index + 1);
	// 	}
	// 	server.regexMatch[server.regexCount] = 2;
	// 	server.regex[server.regexCount].match = locBlockExtract;
	// 	server.regexCount += 1;
	// 	std::cout << server.regex[server.regexCount].match << std::endl;
	// 	return (0);
	// }
	// else if (line[index] == '!' && line[index] == '~')
	// {

	// }
	std::cout << "\033[31mError: \033[0mMatch type issue" << std::endl;
	return (index + 1);
}

enum kwtypeLocBlock
{
	INDEX_L,
	ALLOW_METHOD_L,
	ROOT_L,
	AUTOINDEX_L,
	RETURN_L,
	UNKNOWN_L
};

std::map<str, kwtypeLocBlock> initializekwLMap() 
{
    std::map<std::string, kwtypeLocBlock> kwL;
    kwL["index"] = INDEX_L;
    kwL["allow_methods"] = ALLOW_METHOD_L;
    kwL["root"] = ROOT_L;
	kwL["autoindex"] = AUTOINDEX_L;
	kwL["return"] = RETURN_L;
    return kwL;
}

int	ConfigParser::numberfy_kwL(str extract)
{
	static std::map<str, kwtypeLocBlock> kwL = initializekwLMap();
    
    std::map<str, kwtypeLocBlock>::iterator it = kwL.find(extract);
    if (it != kwL.end()) 
        return it->second;
    return UNKNOWN;
}

int	ConfigParser::check_kw_locBlock(str line)
{
	str extract;
	int i = 0;

	while (line[i] != '\0' && line[i] == ' ')
		i++;
	while (line[i] != '\0' && (isalnum(line[i]) || line[i] == '_'))
	{
		extract += line[i];
		i++;
	}
	switch (numberfy_kwL(extract))
	{
		case INDEX_L:
			return INDEX_L;
		case ALLOW_METHOD_L:
			return ALLOW_METHOD_L;
		case ROOT_L:
			return ROOT_L;
		case AUTOINDEX_L:
			return AUTOINDEX_L;
		case RETURN_L:
			return RETURN_L;
		case UNKNOWN:
			return (UNKNOWN);
	}
	return (-1);
}

int	ConfigParser::kwEqual(str line, ServerInfo& server)
{
	size_t	pos;
	size_t	tmp;
	str		extract;

	pos = line.find("root");
	tmp = line.find("#");
	int a = 1;

	if (server.exactMatch[server.equalCount - 1] == 2 && server.equal[server.equalCount - 1].kwOBracket == 0)
	{
		if ((a = (nCheck(line, 0, server))) != -1)
			return (a);
		if (server.exactMatch[server.equalCount - 1] == 2 && server.equal[server.equalCount - 1].kwOBracket == 0)
		{
			std::cout << "\033[31mError: \033[0mMissing opening curly bracket after a keyword" << std::endl;
			return (1);
		}
	}
	else if (server.exactMatch[server.equalCount - 1] == 2 && server.equal[server.equalCount - 1].kwOBracket == 1 && server.equal[server.equalCount - 1].kwCBracket == 0)
	{
		if (tmp < pos)
		{
			pos = std::string::npos;
			if (pCheck(line, tmp) != 0)
				return (1);
		}
		else if (pos == 8)
		{
			if ((a = posPositionCheck(line, pos)) != 0)
				return (a);
		}
		if (pos != 8 && pos != std::string::npos)
		{
			return (emptySpaceCheck(line, pos, 8));
		}
		else if (pos != std::string::npos)
		{
			a = 1;
			while (line[pos + 3 + a] == ' ' && line[pos + 3 + a] != '\0')
				a++;
			if (line[pos + 3 + a - 1] == 'x')

			{
				std::cout << "\033[31mError: \033[0mKeyword and directory should be separated by a space(s)" << std::endl;
				return (pos + 3 + a + 1);
			}
			while (line[pos + 3 + a] != ' ' && line[pos + 3 + a] != '\0')
			{
				if (line[pos + 3 + a] == ';')
					break;
				extract += line[pos + 3 + a];
				a++;
			}
			if (line[pos + 3 + a] != ';')
			{
				std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
				return (pos + 3 + a);
			}
			if (extract.empty())
			{
				std::cout << "\033[31mError: \033[0mNo value found for root exact (=) type match" << std::endl;
				return (pos + 3 + a);
			}
			if ((a = (nCheck(line, pos + 4 + a, server))) != -1)
				return (a);
			server.equal[server.equalCount - 1].directory = extract;
			// std::cout << server.equal[server.equalCount - 1].directory << std::endl;
		}
		else if (pos == std::string::npos)
		{
			if ((a = (nCheck(line, 0, server))) != -1)
				return (a);
		}
	}
	return (-1);
}

int	ConfigParser::allowMethod_ExtractCheck(str e1, str e2, str e3)
{
	//check if methods are duplicated
	int q = 0;
	int w = 0;
	int e = 0;
	if (!e2.empty() && e3.empty())//2 methods
	{
		if (e1 == "GET")//1
			q = 1;
		else if (e1 == "POST")//2
			q = 2;
		else if (e1 == "DELETE")//4
			q = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given for the 1st value (Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}

		if (e2 == "GET")//1
			w = 1;
		else if (e2 == "POST")//2
			w = 2;
		else if (e2 == "DELETE")//4
			w = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given for the 2nd value(Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}
		if (q == w)
		{
			std::cout << "\033[31mError: \033[0mMethod duplication detected (Please ensure that GET/POST/DELETE method is used only once as a value)" << std::endl;
			return (-2);
		}
		return (q + w);
	}
	else if (!e2.empty() && !e3.empty())//3 methods
	{
		if (e1 == "GET")//1
			q = 1;
		else if (e1 == "POST")//2
			q = 2;
		else if (e1 == "DELETE")//4
			q = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given for the 1st value (Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}

		if (e2 == "GET")//1
			w = 1;
		else if (e2 == "POST")//2
			w = 2;
		else if (e2 == "DELETE")//4
			w = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given for the 2nd value(Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}
		
		if (e3 == "GET")//1
			e = 1;
		else if (e3 == "POST")//2
			e = 2;
		else if (e3 == "DELETE")//4
			e = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given for the 3rd value(Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}
		if (q == w || q == e || e == w)
		{
			std::cout << q << w << e << std::endl;
			std::cout << "\033[31mError: \033[0mMethod duplication detected (Please ensure that GET/POST/DELETE method is used only once as a value)" << std::endl;
			return (-2);
		}
		return (q + w + e);
	}
	else if (e2.empty() && e3.empty())// 1 method
	{
		if (e1 == "GET")//1
			q = 1;
		else if (e1 == "POST")//2
			q = 2;
		else if (e1 == "DELETE")//4
			q = 4;
		else
		{
			std::cout << "\033[31mError: \033[0mUnrecognized method given (Acceptable methods: GET/POST/DELETE)" << std::endl;
			return (-2);
		}
		return (q);
	}
	else
	{
		std::cout << "\033[31mError: \033[0mNot known behaviour" << std::endl;
		return (-2);
	}
}

int	ConfigParser::kwPrefix(str line, ServerInfo& server)
{
	size_t	pos;
	size_t	tmp;
	int		u = 0;
	int		q = 0;
	int		int_extract;
	str		extract;
	str		extract2;
	str		extract3;

	int	s_or_l = 0;
	int g = 0;

	g = check_kw_locBlock(line);
	if (g == INDEX_L)
		pos = line.find("index");
	else if (g == ALLOW_METHOD_L)
	{
		pos = line.find("allow_methods");
		s_or_l = 1;
	}
	else if (g == ROOT_L)
	{
		pos = line.find("root");
		s_or_l = 2;
	}
	else if (g == AUTOINDEX_L)
	{
		pos = line.find("autoindex");
		s_or_l = 3;
	}
	else if (g == RETURN_L)
	{
		pos = line.find("return");
		s_or_l = 4;
	}
	else
		pos = std::string::npos;

	tmp = line.find("#");
	int a = 1;

	if (server.prefixMatch[server.prefixCount - 1] == 2 && server.prefix[server.prefixCount - 1].kwOBracket == 0)
	{
		if ((a = (nCheck(line, 0, server))) != -1)
			return (a);
		if (server.prefixMatch[server.prefixCount - 1] == 2 && server.prefix[server.prefixCount - 1].kwOBracket == 0)
		{
			std::cout << "\033[31mError: \033[0mMissing opening curly bracket after a keyword" << std::endl;
			return (1);
		}
	}
	else if (server.prefixMatch[server.prefixCount - 1] == 2 && server.prefix[server.prefixCount - 1].kwOBracket > 0 && server.prefix[server.prefixCount - 1].kwCBracket < server.prefix[server.prefixCount - 1].kwOBracket)
	{
		if (tmp < pos)
		{
			pos = std::string::npos;
			if (pCheck(line, tmp) != 0)
				return (1);
		}
		else if (pos == server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4)
		{
			if ((a = posPositionCheck(line, pos)) != 0)
				return (a);
		}
		if (pos != server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4 && pos != std::string::npos)
		{
			return (emptySpaceCheck(line, pos, server.oBracket * 4 + server.prefix[server.prefixCount - 1].kwOBracket * 4));
		}
		else if (pos != std::string::npos)
		{
			a = 1;
			if (s_or_l == 0)//index
			{
				if (server.prefix[server.prefixCount - 1].index_b == true)
				{
					std::cout << "\033[31mError: \033[0mIndex variable already registered previously (Location block cannot have more than one index variable)" << std::endl;
					return (pos + 4);
				}
				while (line[pos + 4 + a] != '\0')
				{
					extract = "";
					while (line[pos + 4 + a] == ' ' && line[pos + 4 + a] != '\0')
						a++;
					if (line[pos + 4 + a - 1] == 'x')
					{
						std::cout << "\033[31mError: \033[0mIndex directive within location block should be separated by space(s)/tab(s) from default files directory" << std::endl;
						return (pos + 4 + a + 1);
					}
					while (line[pos + 4 + a] != ' ' && line[pos + 4 + a] != '\0')
					{
						if (line[pos + 4 + a] == ';')
							break;
						extract += line[pos + 4 + a];
						a++;
					}
					if (extract.empty() && server.prefix[server.prefixCount - 1].index.empty())
					{
						std::cout << "\033[31mError: \033[0mNo value found for variable \033[31m->index\033[0m" << std::endl;
						return (pos + 4 + a);
					}
					if (extract.empty() && !server.prefix[server.prefixCount - 1].index.empty())
					{
						if (line[pos + 4 + a] != ';')
						{
							std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
							return (pos + 4 + a);
						}
					}
					server.prefix[server.prefixCount - 1].index.push_back(extract);
					if (!extract.empty() && line[pos + 4 + a] != ';')
						continue;
					if ((a = (nCheck(line, pos + 5 + a, server))) != -1)
						return (a);
					break;
				}
				server.prefix[server.prefixCount - 1].index_b = true;
			}
			else if (s_or_l == 1)//allow method
			{
				if (server.prefix[server.prefixCount - 1].allowmethod_b == true)
				{
					std::cout << "\033[31mError: \033[0mFor this location block allowmethod variable value is already registered (Accepted format: Maximum one allowmethod variable per location block)" << std::endl;
					return (pos + 12);
				}
				while (line[pos + 12 + a] == ' ' && line[pos + 12 + a] != '\0')
					a++;
				if (line[pos + 12 + a - 1] == 's')
				{
					std::cout << "\033[31mError: \033[0mKeyword and directory should be separated by a space(s)" << std::endl;
					return (pos + 12 + a + 1);
				}
				while (line[pos + 12 + a] != ' ' && line[pos + 12 + a] != '\0')
				{
					if (line[pos + 12 + a] == ';')
						break;
					extract += line[pos + 12 + a];
					a++;
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mNo value found for variable \033[31m->allow_methods\033[0m" << std::endl;
					return (pos + 12 + a);
				}
				if (line[pos + 12 + a] != ';')
				{
					while (line[pos + 12 + a] == ' ' && line[pos + 12 + a] != '\0')
						a++;
					while (line[pos + 12 + a] != ' ' && line[pos + 12 + a] != '\0')
					{
						if (line[pos + 12 + a] == ';')
							break;
						extract2 += line[pos + 12 + a];
						a++;
					}
					if (extract2.empty())
					{
						std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
						return (pos + 12 + a);
					}
					if (line[pos + 12 + a] != ';')
					{
						while (line[pos + 12 + a] == ' ' && line[pos + 12 + a] != '\0')
							a++;
						while (line[pos + 12 + a] != ' ' && line[pos + 12 + a] != '\0')
						{
							if (line[pos + 12 + a] == ';')
								break;
							extract3 += line[pos + 12 + a];
							a++;
						}
						if (extract3.empty())
						{
							std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
							return (pos + 12 + a);
						}
						if (line[pos + 12 + a] != ';')
						{
							std::cout << "\033[31mError: \033[0mSemicolon missing after value/There are only 3 possible methods (more than 3 methods is unacceptable)" << std::endl;
							return (pos + 12 + a);
						}
					}
				}
				if ((a = (nCheck(line, pos + 13 + a, server))) != -1)
					return (a);
				
				q = allowMethod_ExtractCheck(extract, extract2, extract3);
				if (q == -2)
					return (pos + 12);
				else
				{
					server.prefix[server.prefixCount - 1].allowMethod = q;
					server.prefix[server.prefixCount - 1].allowmethod_b = true;
				}
			}
			else if (s_or_l == 2)//root
			{
				if (server.prefix[server.prefixCount - 1].root_b == true)
				{
					std::cout << "\033[31mError: \033[0mFor this location block root is already registered (Accepted format: Maximum one root directive per location block)" << std::endl;
					return (pos + 3);
				}
				while (line[pos + 3 + a] == ' ' && line[pos + 3 + a] != '\0')
					a++;
				if (line[pos + 3 + a - 1] == 't')
				{
					std::cout << "\033[31mError: \033[0mKeyword and directory should be separated by a space(s)" << std::endl;
					return (pos + 3 + a + 1);
				}
				while (line[pos + 3 + a] != ' ' && line[pos + 3 + a] != '\0')
				{
					if (line[pos + 3 + a] == ';')
						break;
					extract += line[pos + 3 + a];
					a++;
				}
				if (line[pos + 3 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
					return (pos + 3 + a);
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mNo value found for variable \033[31m->root\033[0m" << std::endl;
					return (pos + 3 + a);
				}
				if ((a = (nCheck(line, pos + 4 + a, server))) != -1)
					return (a);
				server.prefix[server.prefixCount - 1].root = extract;
				server.prefix[server.prefixCount - 1].root_b = true;//Do not forget to initialize it to false!!!
			}
			else if (s_or_l == 3)//autoindex
			{
				if (server.prefix[server.prefixCount - 1].autoindex_b == true)
				{
					std::cout << "\033[31mError: \033[0mFor this location block autoindex is already registered (Accepted format: Maximum one autoindex variable per location block)" << std::endl;
					return (pos + 8);
				}
				while (line[pos + 8 + a] == ' ' && line[pos + 8 + a] != '\0')
					a++;
				if (line[pos + 8 + a - 1] == 'x')
				{
					std::cout << "\033[31mError: \033[0mKeyword and directory should be separated by a space(s)" << std::endl;
					return (pos + 8 + a + 1);
				}
				while (line[pos + 8 + a] != ' ' && line[pos + 8 + a] != '\0')
				{
					if (line[pos + 8 + a] == ';')
						break;
					extract += line[pos + 8 + a];
					a++;
				}
				if (line[pos + 8 + a] != ';')
				{
					std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
					return (pos + 8 + a);
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0mNo value found for variable \033[31m->root\033[0m" << std::endl;
					return (pos + 8 + a);
				}
				if ((a = (nCheck(line, pos + 9 + a, server))) != -1)
					return (a);
				if (extract == "on")
				{
					server.prefix[server.prefixCount - 1].autoindex = true;
					server.prefix[server.prefixCount - 1].autoindex_b = true;
				}
				else if (extract == "off")
				{
					server.prefix[server.prefixCount - 1].autoindex = false;
					server.prefix[server.prefixCount - 1].autoindex_b = true;
				}
				else
				{
					std::cout << "\033[31mError: \033[0mUnrecognized value for autoindex (accepted values \033[31m->on\033[0m or \033[31m->off\033[0m)" << std::endl;
					return (pos + 8);
				}
			}
			else if (s_or_l == 4)//return
			{
				if (server.prefix[server.prefixCount - 1].return_b == true)
				{
					std::cout << "\033[31mError: \033[0mFor this location block return directive is already registered (Accepted format: Maximum one return directive per location block)" << std::endl;
					return (pos + 5);
				}
				while (line[pos + 5 + a] == ' ' && line[pos + 5 + a] != '\0')
					a++;
				while (line[pos + 5 + a] != ' ' && line[pos + 5 + a] != '\0')
				{
					if (line[pos + 5 + a] == ';')
						break;
					extract += line[pos + 5 + a];
					a++;
					while (extract[u] != '\0')
					{
						if (!isdigit(extract[u]))
						{
							std::cout << "\033[31mError: \033[0mReturn directive code should only consist of positive numbers" << std::endl;
							return (pos + 5 + a);
						}
						u++;
					}
					if (u > 3)
					{
						std::cout << "\033[31mError: \033[0mReturn directive code should not exceed 3 digits" << std::endl;
						return (pos + 5 + a);
					}
				}
				if (extract.empty())
				{
					std::cout << "\033[31mError: \033[0m(CODE) No value found for directive \033[31m->return\033[0m" << std::endl;
					return (pos + 5 + a);
				}
				if (line[pos + 5 + a] != ';')
				{
					while (line[pos + 5 + a] == ' ' && line[pos + 5 + a] != '\0')
						a++;
					while (line[pos + 5 + a] != ' ' && line[pos + 5 + a] != '\0')
					{
						if (line[pos + 5 + a] == ';')
							break;
						extract2 += line[pos + 5 + a];
						a++;
					}
					if (extract2.empty())
					{
						std::cout << "\033[31mError: \033[0m(URL) No value found for directive \033[31m->return\033[0m" << std::endl;
						return (pos + 5 + a);
					}
					if (line[pos + 5 + a] != ';')
					{
						std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
						return (pos + 5 + a);
					}
					if ((a = (nCheck(line, pos + 6 + a, server))) != -1)
						return (a);
					int_extract = atoi(extract.c_str());
					server.prefix[server.prefixCount - 1].return_[int_extract] = extract2;
					server.prefix[server.prefixCount - 1].return_b = true;
				}
				else if (line[pos + 5 + a] == ';')
				{
					std::cout << "\033[31mError: \033[0mMissing pair (return directive should consist of pairs <CODE|URL>)" << std::endl;
					return (pos + 5 + a);
				}
			}
		}
		else if (pos == std::string::npos)
		{
			if ((a = (nCheck(line, 0, server))) != -1)
				return (a);
		}
	}
	return (-1);
}

int	ConfigParser::kwCaseInsen(str line, ServerInfo& server)
{
	size_t	pos;
	size_t	tmp;
	str		extract;

	pos = line.find("root");
	tmp = line.find("#");
	int a = 1;

	if (server.caseInsenMatch[server.caseInsenCount - 1] == 2 && server.caseInsen[server.caseInsenCount - 1].kwOBracket == 0)
	{
		if ((a = (nCheck(line, 0, server))) != -1)
			return (a);
		if (server.caseInsenMatch[server.caseInsenCount - 1] == 2 && server.caseInsen[server.caseInsenCount - 1].kwOBracket == 0)
		{
			std::cout << "\033[31mError: \033[0mMissing opening curly bracket after a keyword" << std::endl;
			return (1);
		}	
	}
	else if (server.caseInsenMatch[server.caseInsenCount - 1] == 2 && server.caseInsen[server.caseInsenCount - 1].kwOBracket == 1 && server.caseInsen[server.caseInsenCount - 1].kwCBracket == 0)
	{
		if (tmp < pos)
		{
			pos = std::string::npos;
			if (pCheck(line, tmp) != 0)
				return (1);
		}
		else if (pos == 8)
		{
			if ((a = posPositionCheck(line, pos)) != 0)
				return (a);
		}
		if (pos != 8 && pos != std::string::npos)
		{
			return (emptySpaceCheck(line, pos, 8));
		}
		else if (pos != std::string::npos)
		{
			a = 1;

			while (line[pos + 3 + a] == ' ' && line[pos + 3 + a] != '\0')
				a++;
			if (line[pos + 3 + a - 1] == 'n')
			{
				std::cout << "\033[31mError: \033[0mKeyword and directory should be separated by a space(s)" << std::endl;
				return (pos + 3 + a + 1);
			}
			while (line[pos + 3 + a] != ' ' && line[pos + 3 + a] != '\0')
			{
				if (line[pos + 3 + a] == ';')
					break;
				extract += line[pos + 3 + a];
				a++;
			}
			if (line[pos + 3 + a] != ';')
			{
				std::cout << "\033[31mError: \033[0mSemicolon missing after value" << std::endl;
				return (pos + 3 + a);
			}
			if (extract.empty())
			{
				std::cout << "\033[31mError: \033[0mNo value found for root exact (=) type match" << std::endl;
				return (pos + 3 + a);
			}
			if ((a = (nCheck(line, pos + 4 + a, server))) != -1)
				return (a);
			server.caseInsen[server.caseInsenCount - 1].directory = extract;
			// std::cout << server.equal[server.equalCount - 1].directory << std::endl;
		}
		else if (pos == std::string::npos)
		{
			if ((a = (nCheck(line, 0, server))) != -1)
				return (a);
		}
	}
	return (-1);
}

// int	ConfigParser::kwRegex(str line, ServerInfo& server)
// {
// 	return (0);
// }

int	ConfigParser::nCheck(str line, size_t pos, ServerInfo& server)
{
	int	a = 0;
	while (true)
	{
		if (line[pos + a] == '\0' || line[pos + a] == '#')
			break ;
		if (!isspace(line[pos + a]))
		{
			if (line[pos + a] == '}')
			{
				if ((server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2 && server.equal[server.equalCount - 1].kwOBracket > 0) || (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2 && server.prefix[server.prefixCount - 1].kwOBracket > 0) || (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2 && server.caseInsen[server.caseInsenCount - 1].kwOBracket > 0) || (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 2 && server.regex[server.regexCount - 1].kwOBracket > 0))
				{
					if (cBracketPosCheck(server, pos + a, 2) != 0)
					{
						std::cout << "\033[31mError: \033[0mCheck closing curly bracket positioning" << std::endl;
						return (pos + a + 1);
					}
					if (server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2)
					{	
						server.equal[server.equalCount - 1].kwCBracket += 1;
						server.exactMatch[server.equalCount - 1] = 1;
					}
					else if (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2)
					{
						server.prefix[server.prefixCount - 1].kwCBracket += 1;
						if (server.prefix[server.prefixCount - 1].kwCBracket == server.prefix[server.prefixCount - 1].kwOBracket)//one change
							server.prefixMatch[server.prefixCount - 1] = 1;
					}
					else if (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2)
					{
						server.caseInsen[server.caseInsenCount - 1].kwCBracket += 1;
						server.caseInsenMatch[server.caseInsenCount - 1] = 1;
					}
					else if (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 2)
					{
						server.regex[server.regexCount - 1].kwCBracket += 1;
						server.regexMatch[server.regexCount - 1] = 1;
					}
					else
					{
						std::cout << "\033[31mError: \033[0mClosing curly bracket should be used only to close open curly bracket" << std::endl;
						return (pos + a);
					}
				}
				else
				{
					std::cout << "\033[31mError: \033[0mClosing curly bracket has to be used together with the opening curly bracket" << std::endl;
					return (pos + a);
				}
			}
			else if (line[pos + a] == '{')
			{
				if ((server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2) || (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2) || (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2) || (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 2))
				{
					if (oBracketPosCheck(server, pos + a, 2) != 0)
					{
						std::cout << "\033[31mError: \033[0mCheck opening curly bracket positioning" << std::endl;
						return (pos + a);
					}
					if (server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2)
						server.equal[server.equalCount - 1].kwOBracket += 1;
					else if (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2)
						server.prefix[server.prefixCount - 1].kwOBracket += 1;
					else if (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2)
						server.caseInsen[server.caseInsenCount - 1].kwOBracket += 1;
					else if (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 2)
						server.regex[server.regexCount - 1].kwOBracket += 1;
					else
					{
						std::cout << "\033[31mError: \033[0mOpening curly bracket should be used only after a keyword" << std::endl;
						return (pos + a);
					}
				}
			}
			else
			{
				std::cout << "\033[31mError: \033[0mRandom character(s) detected" << std::endl;
				return (pos + a + 1);
			}
		}
		a++;
	}
	return (-1);
}

int	ConfigParser::kwLocation(str line, ServerInfo& server)
{
	size_t pos;
	size_t tmp;

	pos = line.find("location");
	tmp = line.find('#');
	int a;
	int b = 0;
	if (pos == 4)
	{
		if ((a = posPositionCheck(line, pos)) != 0)
			return (a);
	}
	a = 1;
	if (tmp < pos)
	{
		pos = std::string::npos;
		if (prevCheck(line, tmp, server) != 0)
			return (1);
	}
	else if (pos != server.oBracket * 4 && pos != std::string::npos)
	{
		return (emptySpaceCheck(line, pos, server.oBracket * 4));
	}
	else if (pos != std::string::npos)
	{
		while (line[pos + 7 + a] == ' ' && line[pos + 7 + a] != '\0')
			a++;
		if (line[pos + 7 + a - 1] == 'n')
		{
			std::cout << "\033[31mError: \033[0mKeyword and type match should be separated by a space(s)" << std::endl;
			return (pos + 7 + a + 1);
		}
		if ((b = getLocMatchType(line, server, pos + 7 + a)) != 0)
			return (b);
	}
	else if (pos == std::string::npos && tmp == std::string::npos)
	{
		// if (server.equalCount == 0 && server.prefixCount == 0 && server.caseInsenCount == 0 && server.regexCount == 0)
		// {
		// 	if ((a = nextCheck(line, 0)) != -1)
		// 	return (a);
		// }
		// else if ((a = nCheck(line, 0, server)) != -1)
		// 	return (a);
		if ((a = nextCheck(line, 0, server)) != -1)
			return (a);
	}
	return (-1);
}

int	ConfigParser::pullInfo(str line, ServerInfo& server)
{
	int a;
	if (server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2)
	{	
		if ((a = kwEqual(line, server)) != -1)
			return (a);
	}
	else if (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2)
	{
		if ((a = kwPrefix(line, server)) != -1)
			return (a);
	}
	else if (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2)
	{
		if ((a = kwCaseInsen(line, server)) != -1)
			return (a);
	}
	// else if (server.regexCount > 0 && regexMatch[server.regexCount - 1] == 2)
	// {
	// 	kwRegex(line, server);
	// }
	return (-1);
}

int ConfigParser::lineCheck(str line, ServerInfo& server)
{
	int a = 0;
	if (server.srvr == false)
		return (a = kwServer_oB(line, server));
	else if (server.srvr == true && server.oBracket == 0)
	{
		a = nextCheck(line, 0, server);
		if (a != -1)
			return (a);
	}
	else if ((server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 2) || (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 2) || (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 2) || (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 2))
	{	
		if ((a = pullInfo(line, server)) != -1)
			return (a);
	}
	else if ((server.equalCount == 0 || (server.equalCount > 0 && server.exactMatch[server.equalCount - 1] == 1)) || (server.prefixCount == 0 || (server.prefixCount > 0 && server.prefixMatch[server.prefixCount - 1] == 1)) || (server.caseInsenCount == 0 || (server.caseInsenCount > 0 && server.caseInsenMatch[server.caseInsenCount - 1] == 1)) || (server.regexCount == 0 || (server.regexCount > 0 && server.regexMatch[server.regexCount - 1] == 1)))
		return (a = kwListenPort(line, server));

	return (-1);
}

str	stringChange(str line, str s1, str s2)
{
	str newLine;
	size_t position = line.find(s1);
	size_t i = 0;
	while (position != std::string::npos)
	{
		newLine += line.substr(i, position - i) + s2;
		i = position + s1.length();
		position = line.find(s1, i);
	}
	newLine += line.substr(i);
	return (newLine);
}

int	ConfigParser::parsing(str av)
{
	int a = 0;
	int ch_pos;

	for (int d = 0; d <= 9; d++)
	{
		server[d].equalCount = 0;//Count how many exact type matches
		server[d].prefixCount = 0;
		server[d].caseInsenCount = 0;
		server[d].regexCount = 0;
		for (int i = 0; i <= 9; i++)
		{
			server[d].equal[i].kwCBracket = 0;
			server[d].equal[i].kwOBracket = 0;

			server[d].prefix[i].kwCBracket = 0;
			server[d].prefix[i].kwOBracket = 0;
			server[d].prefix[i].root_b = false;
			server[d].prefix[i].allowmethod_b = false;
			server[d].prefix[i].autoindex_b = false;
			server[d].prefix[i].index_b = false;
			server[d].prefix[i].return_b = false;
			server[d].prefix[i].autoindex = false;//by default



			server[d].caseInsen[i].kwCBracket = 0;
			server[d].caseInsen[i].kwOBracket = 0;
			server[d].regex[i].kwCBracket = 0;
			server[d].regex[i].kwOBracket = 0;
		}
		server[d].cBracket = 0;
		server[d].oBracket = 0;
		server[d].srvr = false;
		server[d].listen_b = false;
		server[d].srvr_name_b = false;
		server[d].host_name_b = false;
		server[d].error_b = false;
		server[d].root_dir_b = false;
		server[d].body_size_b = false;
	}
	// bool gb = false;

	str line;
	str	file;
	file = av;
	str	newFile = file + ".replace";
	std::ifstream sourceFile(file.c_str());
	std::cout << std::endl;
	if (!sourceFile)
	{
		std::cerr << "\033[31mError opening config file!\033[0m\n"
				  << "\033[31mfile named: \033[0m" << file << " \033[31mDOES NOT EXIST!\033[0m" << std::endl;
		return (1);
	}
	std::ofstream destFile(newFile.c_str());
	if (!destFile)
	{
		std::cerr << "Error creating destination file!" << std::endl;
		return (1);
	}
	while (std::getline(sourceFile, line))
	{
		destFile << stringChange(line, "\t", "    ");
		if (!sourceFile.eof())
			destFile << std::endl;
	}
	destFile.close();
	sourceFile.close();
	std::ifstream sourceF(newFile.c_str());
	if (!sourceF)
	{
		std::cerr << "\033[31mError opening config file! \033[0m\n"
				  << "\033[31mfile named: \033[0m" << file << " \033[31mDOES NOT EXIST!\033[0m" << std::endl;
		return (1);
	}
	while (std::getline(sourceF, line))
	{	
		a++;
		if (line.empty())
			continue;//if its empty line continue with the loop
		ch_pos = lineCheck(line, server[this->serverCount]);
		if (ch_pos != -1)
		{
			std::cout << "\033[31mFile name: " << "\""  
					  << file << "\"" << " -- line " 
					  << a << " -- character " << ch_pos 
					  << " (Please review the file!)\033[0m" << std::endl;// TODO: make it a separate function!
			std::cout << std::endl;

			return (1);
		}
		// if (mainBracketsCheck(a, server[this->serverCount]) == 1)
		// {
		// 	return (fileIteration(sourceF, a, file, gb));
		// }
		if (server[this->serverCount].srvr == true && server[this->serverCount].cBracket > 0 && server[this->serverCount].oBracket > 0 && server[this->serverCount].oBracket == server[this->serverCount].cBracket)
		{
			// gb = true;
			// if ((a = fileIteration(sourceF, a, file, gb)) != 0)//create another function to get other servers
			// 	return (1);
			// break; 
			this->serverCount += 1;
			if (serverCount == 10)
				break;
			continue;
		}
	}
	bool gb = true;
	if (fileIteration(sourceF, a, file, gb) != 0)
		return (1);
	if (serverCount == 0)
	{
		std::cout << "\033[31mError: \033[0mIn the given configuration file, server is missing" << std::endl;
		std::cout << std::endl;
		return (1);
	}
	for (int d = 0; d < this->serverCount; d++)
	{
		for (int y = 0; y < server[d].equalCount; y++)
		{
			if (server[d].exactMatch[y] == 2)
			{
				std::cout << "\033[31mError: \033[0mCould not find closing curly bracket for type match" << std::endl;
				return (1);
			}
			if (server[d].prefixMatch[y] == 2)
			{
				std::cout << "\033[31mError: \033[0mCould not find closing curly bracket for type match" << std::endl;
				return (1);
			}
			if (server[d].caseInsenMatch[y] == 2)
			{
				std::cout << "\033[31mError: \033[0mCould not find closing curly bracket for type match" << std::endl;
				return (1);
			}
			if (server[d].regexMatch[y] == 2)
			{
				std::cout << "\033[31mError: \033[0mCould not find closing curly bracket for type match" << std::endl;
				return (1);
			}
		}
	}

	for (int c = 0; c < this->serverCount; c++)
	{
		size_t k = 0;
		int ab = 0;
		if (server[c].server_name.empty())
			server[c].server_name = "default.com";

		if (server[c].cBracket != server[c].oBracket)
		{
			std::cout << "\033[31mError: \033[0mCould not find closing curly bracket to match the opening curly bracket" << std::endl;
			std::cout << std::endl;
			return (1);
		}
		if (server[c].body_size_b == false)
			server[c].max_body_size = 10000;

		if (server[c].listen_port.empty())
		{
			std::cout << "\033[31mError: \033[0mFor " << c + 1 << "st/nd/rd/th server, port to listen is missing (Please review!)" << std::endl;
			std::cout << std::endl;
			return (1);
		}
		k = server[c].listen_port.size();
		for (size_t f = 0; f < k; f++)
		{
			int fg = 0;
			ab = server[c].listen_port[f];
			for (size_t d = 0; d < server[c].listen_port.size(); d++)
			{
				if (ab == server[c].listen_port[d])
					fg++;
			}
			if (fg > 1)
			{
				std::cout << "\033[31mError: \033[0mDetected same port numbers from " << c + 1  << "st/nd/rd/th server (Please review!)" << std::endl;
				std::cout << std::endl;
				return (1);
			}
		}
	}
	
	for (int d = 0; d < this->serverCount; d++)
	{
		// for (int f = 0; f < server[d].equalCount; f++)
		// {
		// 	std::cout << "kwCBracket: " << server[d].equal[f].kwCBracket << std::endl;
		// 	std::cout << "kwOBracket: " << server[d].equal[f].kwOBracket << std::endl;
		// 	std::cout << "directory: " << server[d].equal[f].directory << std::endl;
		// 	std::cout << "match: " << server[d].equal[f].match << std::endl;
		// 	std::cout << std::endl;
		// }

		for (int f = 0; f <= serverCount; f++)
		{
			if (server[f].srvr == true && server[f].oBracket > 0 && server[f].cBracket < server[f].oBracket)
			{
				std::cout << "\033[31mError: \033[0mDetected server with insufficient configuration information" << std::endl;
				std::cout << std::endl;
				return (1);
			}
			else if (server[f].srvr == true && server[f].cBracket > 0 && server[f].oBracket < server[f].cBracket)
			{
				std::cout << "\033[31mError: \033[0mDetected server with insufficient configuration information" << std::endl;
				std::cout << std::endl;
				return (1);
			}
			else if (server[f].srvr == true && server[f].cBracket == 0 && server[f].oBracket == 0)
			{
				std::cout << "\033[31mError: \033[0mDetected server with insufficient configuration information" << std::endl;
				std::cout << std::endl;
				return (1);
			}
		}

		for (int f = 0; f < server[d].prefixCount; f++)
		{
			std::cout << "match: " << server[d].prefix[f].match << std::endl;
			std::cout << "root: " << server[d].prefix[f].root << std::endl;
			for (size_t x = 0; x < server[d].prefix[f].index.size(); x++)
				std::cout << "index: " << server[d].prefix[f].index[x] << std::endl;
			std::cout << "allowmethods: " << server[d].prefix[f].allowMethod << std::endl;
			std::cout << "autoIndex: " << server[d].prefix[f].autoindex << std::endl;
			for (std::map<int, str>::const_iterator i = server[d].prefix[f].return_.begin(); i != server[d].prefix[f].return_.end(); ++i)
				std::cout << "return: " << i->first << " -> " << i->second << std::endl;

			std::cout << "kwOBracket: " << server[d].prefix[f].kwOBracket << std::endl;
			std::cout << "kwCBracket: " << server[d].prefix[f].kwCBracket << std::endl;
			std::cout << std::endl;
		}

	
		std::cout << "prefixCount: " << server[d].prefixCount << std::endl;
		std::cout << "cBracket: " << server[d].cBracket << std::endl;
		std::cout << "oBracket: " << server[d].oBracket << std::endl;
		std::cout << "maxBodySize: " << server[d].max_body_size << std::endl;
		std::cout << "root: " << server[d].root_ << std::endl;
		std::cout << "host: " << server[d].host << std::endl;
		std::cout << "root: " << server[d].root_ << std::endl;
		for (size_t c = 0; c < server[d].listen_port.size(); c++)
			std::cout << "listenPort: " << server[d].listen_port[c] << std::endl;
		for (std::map<int, str>::iterator i = server[d].error_page.begin(); i != server[d].error_page.end(); i++)
			std::cout << "errorPage: " << i->first << " -> " << i->second << std::endl;
		std::cout << "serverName: " << server[d].server_name << std::endl;
	}
	std::cout << "ServerCount: " << this->serverCount << std::endl;
	std::cout << std::endl;
	return (0);
}

// // Example of a function that checks the location match type
// std::string check_location_type(const std::string& location) {
//     if (location[0] == '=') {
//         return "exact"; // `=` for exact match
//     }
//     else if (location[0] == '~') {
//         return "regex"; // `~` for regular expression match
//     }
//     else if (location[0] == '/') {
//         return "prefix"; // `/` for prefix match
//     }
//     return "unknown"; // Return unknown if not matched
// }

// Summary of Location Match Types:
// Exact match (=): Matches the exact path.
// Prefix match (/): Matches any path starting with the given prefix (default).
// Case-sensitive regular expression match (~): Matches paths based on a case-sensitive regular expression.
// Case-insensitive regular expression match (~*): Matches paths based on a case-insensitive regular expression.
// Negated regular expression match (!~): Matches paths not matching the given regular expression.


/*
Things to do:
- Create protections so that whenever it goes over the size of the array it is protected (cannot exceed array number) TEST IT!
- Do not forget to make body size to INT and add check (same as return CODE)
- Overall check and TEST the parser!!!
*/