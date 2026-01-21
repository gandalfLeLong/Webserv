#pragma once

# include <iostream>
# include <string>
# include <csignal>
# include "ConfigParser.hpp"
# include "Header.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Data.hpp"

//Functions
void    init_signals(Server* server);