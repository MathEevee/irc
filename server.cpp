/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/01 18:45:22 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <csignal>
#include <cstdlib>
#include <errno.h>

int	Server::getServSocket()
{
	return (this->_serv_socket);
}

std::string		Server::get_password(void)
{
	return (this->_password);
}

void	Server::set_password(std::string password)
{
	this->_password = password;
}

void	Server::set_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServSocket();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int	i = 1;
	for (std::vector<Client>::iterator it = _tab_client.begin(); it != _tab_client.end(); it++)
	{
		fds[i].fd = it->get_socket_fd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
	// std::cout << i << std::endl;
}

bool	Server::add_client()
{
	int clientSocket = accept(this->getServSocket(), NULL, NULL);
	if (clientSocket != -1)
	{
		if (this->_tab_client.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			std::cout << "Too many clients" << std::endl;
		}
		else
		{
			Client	new_client(clientSocket);
			this->_tab_client.push_back(new_client);
			return (true);
		}
	}
	return (false);
}

bool	Server::check_password(Client &client, std::string password)
{
	std::cout << "message |"<< password<<"|" << std::endl;
	// std::cout << this->get_password() << std::endl;
	if (client.get_count_connexion() == 2 && this->get_password() != password)
	{
		//add convert hexchat
		std::cout << "Bad password 3 times" << std::endl;
		return (false);
	}
	if (this->get_password() == password)
	{
		std::cout << "Good password" << std::endl;
		client.set_status(1);
	}
	else
	{
		if (this->get_password() != password)
			client.set_count_connexion(client.get_count_connexion() + 1);
	}
	return (true);
}


bool	Server::set_commande(Client &client)
{
	std::string message = client.get_message();
	while (message.find("\n") != std::string::npos)
	{
		std::string commande = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.set_message(message);
		if (client.get_status() == 0)
			return (check_password(client, commande));
		else
		{
			//parsing commande
			std::cout << commande << std::endl;
		}
	}
	return (true);
}

void	Server::read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int	i = 1;
	ssize_t size;
	char	buffer[1024];

	for (std::vector<Client>::iterator it = this->_tab_client.begin(); it != this->_tab_client.end() - new_client;)
	{
		std::string	message = it->get_message();
		bool	connexion = true;
		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (size == 0)
				{
					close(it->get_socket_fd());
					std::cout << "disconnected" << std::endl;
					it = this->_tab_client.erase(it);
				}
				buffer[size] = 0;
				message = message + buffer;
				it->set_message(message);
			} while (size == 1024);

			connexion = set_commande(*it);
			if (connexion == false)
			{
				close(it->get_socket_fd());
				it = this->_tab_client.erase(it);
			}
			i++;
			if (size == 0)
				continue;
		}
		if (connexion == true)
			it++;
	}
}

void	Server::runtime()
{
	struct pollfd fds[NB_MAX_CLIENTS + 1];
	char buffer[1024] = { 0 };

	while (1)
	{
		bool	new_client = false;
		set_fds(fds);
		int	nb_fd = poll(fds, _tab_client.size() + 1, -1);

		if (nb_fd == -1 && errno == EINTR)
			return;
		if ((fds[0].revents & POLLIN) != 0)
		{
			new_client = add_client();
		}
		read_all_clients(fds, new_client);
	}
}

Server::Server(int port, std::string password)
{
	this->_password = password;
	this->_serv_socket = socket(AF_INET, SOCK_STREAM, 0);

	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_serv_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServSocket(), 5);
	runtime();
}


Server::~Server()
{
	for (std::vector<Client>::iterator it = this->_tab_client.begin(); it != this->_tab_client.end(); it++)
	{
		close(it->get_socket_fd());
	}
}
