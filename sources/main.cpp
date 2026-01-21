/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmathot <cmathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:24:15 by cmathot           #+#    #+#             */
/*   Updated: 2025/04/18 12:36:07 by cmathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Error: Webserv: Wrong numbers of arguments. (expected 1)" << std::endl;
        return (-1); 
    }
    
    ConfigParser        parser;
    std::vector<Data>   vData;
    if (parser.parsing(av[1]) != 0)
        return (-1);
    for (int i = 0; i < parser.serverCount; i++)
    {
        Data    instance(parser, i);

        vData.push_back(instance);
    }
    
    Server  server(vData, parser);
    
    server.init();
    server.run();
}