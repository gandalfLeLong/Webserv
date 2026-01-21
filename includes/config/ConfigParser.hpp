
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>

typedef std::string str;

typedef struct PrefixMatch_s
{
	bool root_b;
	bool allowmethod_b;
	bool autoindex_b;
	bool index_b;
	bool return_b;// do not forget to initialize it to false

	int	kwOBracket;
	int	kwCBracket;

	str	match;

	str	root;
	std::vector<str> index;
	int	allowMethod;
	bool autoindex;
	std::map<int, str> return_;

	struct PrefixMatch_s& operator=(const struct PrefixMatch_s& copy) {
		if (this != &copy)
		{
			this->root_b = copy.root_b;
			this->allowmethod_b = copy.allowmethod_b;
			this->autoindex_b = copy.autoindex_b;
			this->index_b = copy.index_b;
			this->return_b = copy.return_b;
			this->match = copy.match;
			this->root = copy.root;
			this->index = copy.index;
			this-> allowMethod = copy.allowMethod;
			this-> autoindex = copy.autoindex;
			this-> return_ = copy.return_;
		}
		return (*this);
	};

	PrefixMatch_s() {};
	PrefixMatch_s(const PrefixMatch_s& copy) {
		*this = copy;
	};


} PrefixMatch_t;

class ConfigParser
{
	public:
		struct ExactMatch
		{
			int	kwOBracket;
			int	kwCBracket;
			str	match;
			str	directory;
		};

		struct CaseInsenMatch
		{
			int	kwOBracket;
			int	kwCBracket;
			str	match;
			str	directory;
		};

		struct RegexMatch
		{
			size_t kwOBracket;
			size_t kwCBracket;
			str	match;
			str	directory;
		};
		
		struct ServerInfo 
		{
			bool	srvr;
			bool	listen_b;//at least one so vector
			bool	srvr_name_b;//one needed
			bool	body_size_b;//one needed
			bool	root_dir_b;
			bool	error_b;
			bool	host_name_b;

			size_t	oBracket;
			size_t	cBracket;

			int		prefixMatch[10];//just this one Charlie
			int		exactMatch[10];
			int		caseInsenMatch[10];
			int 	regexMatch[10];

			int	equalCount;
			int prefixCount;
			int	caseInsenCount;
			int regexCount;
			
    		str server_name;
			std::vector<int> listen_port; //int
			int max_body_size;
			str	root_;
			std::map<int, str> error_page;// (int) key pair (map) for error pages
			str	host;

			PrefixMatch_t prefix[10];//(/)
			ExactMatch equal[10];//(=)

			CaseInsenMatch caseInsen[10];//~*
			RegexMatch	regex[10];//~
			//DEFAULT ERROR!
			//Client body size!
			//Might add more!
		};

		int		serverCount;
		ServerInfo server[10];

		ConfigParser();
		~ConfigParser();

		void    output(void);
		int		parsing(str av);
		int		lineCheck(str line, ServerInfo& server);
		int		kwServer_oB(str line, ServerInfo& server);
		int		kwListenPort(str line, ServerInfo& server);

		//Utils
		int		nextCheck(str line, size_t pos, ServerInfo& server);
		int		prevCheck(str line, size_t tmp, ServerInfo& server);
		int		emptySpaceCheck(str line, size_t pos, size_t index);
		int		spaceTabCheck(str line, ServerInfo& server);
		int		posPositionCheck(str line, size_t pos);
		int		mainBracketsCheck(int index, ServerInfo& server);
		int		afterMainBracketCheck(str line);
		int		fileIteration(std::ifstream& sourceFile, int index, str file, bool gb);
		int		oBracketPosCheck(ServerInfo& server, size_t actualPos, int indexToCheck);
		int		cBracketPosCheck(ServerInfo& server, size_t actualPos, int indexToCheck);
		int		check_kw_mainBlock(str line);
		int		numberfy_kw(str	extract);

		
		//Location
		int		kwLocation(str line, ServerInfo& server);
		int		pullInfo(str line, ServerInfo& server);
		int		kwEqual(str line, ServerInfo& server);
		int		kwPrefix(str line, ServerInfo& server);
		int		kwCaseInsen(str line, ServerInfo& server);
		int		kwRegex(str line, ServerInfo& server);


		//Utils for Location
		int		getLocMatchType(str line, ServerInfo& server, int index);
		int		nCheck(str line, size_t pos, ServerInfo& server);
		int		pCheck(str line, size_t tmp);
		int		check_kw_locBlock(str line);
		int		numberfy_kwL(str extract);
		int		allowMethod_ExtractCheck(str e1, str e2, str e3);


};	