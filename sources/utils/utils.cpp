/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmathot <cmathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:52:10 by cmathot           #+#    #+#             */
/*   Updated: 2025/01/31 15:16:27 by cmathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void    handle_signal(int signal, siginfo_t* info, void* context)
{
    Server* server = static_cast<Server*>(info->si_value.sival_ptr);
    (void)context;
    
    std::cout << "\nReceived signal " << signal << std::endl;
    if (server != NULL)
    {
        server->dellAllSocket();
        std::cout << "All socket closed" << std::endl;
    }
    exit (0);
}

void    init_signals(Server* server)
{
    struct sigaction    sa;
    sigval              value;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);
    value.sival_ptr = server;
    sa.sa_sigaction = handle_signal;
    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        std::cerr << "Error: signals." << std::endl;
        exit (1);
    }
    (void)value;
}