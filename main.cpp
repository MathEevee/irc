/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:40:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/01 17:13:22 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <iomanip>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include "server.hpp"

void	handler(int num)
{}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Bad argument" << std::endl;
		return (0);
	}
	signal(SIGINT, handler);
	Server	serv(std::atoi(av[1]), av[2]);
    close(serv.getServSocket());
}

// int main()
// {
//     // creating socket
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     // specifying address
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     // sending connection request
//     connect(clientSocket, (struct sockaddr*)&serverAddress,
//             sizeof(serverAddress));

//     // sending data
// 	std::string	message;
// 	while (getline(std::cin, message))
// 	{
//     	send(clientSocket, message.c_str(), message.size(), 0);
// 		message = "";
// 	}
//     // closing socket
//     close(clientSocket);

//     return 0;
// }