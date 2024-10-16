/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/17 00:55:05 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// Add a client to the server list
bool	Server::add_client()
{
	int clientSocket = accept(this->getServerSocket(), NULL, NULL);
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
			std::cout << "Client connected" << std::endl;
			return (true);
		}
	}
	return (false);
}

// Check is the provide password is the good password
bool	Server::check_password(Client &client, std::string password)
{
	std::cout << "message |"<< password<<"|" << std::endl;
	// std::cout << this->getPassword() << std::endl;
	if (this->getPassword() == password)
	{
		std::cout << "Good password" << std::endl;
		client.set_status(1);
		return (true);
	}
	return (false);
}

// Parse and execute the command based on the first word
void	Server::commands_parsing(Client &client, std::string command)
{
	bool	cmd_result = false; // the purpose of the var is to get the result exe of the command?
	
	std::string	first_word = command.substr(0, command.find(' '));
	std::cout << "First word: " << first_word << std::endl;
	if (first_word == "USER")
		cmd_result = client.checkUser(command.substr(command.find(' ') + 1));
	// else if (first_word == "NICK")
	// 	cmd_result = client.setNickname(command.substr(command.find(' ') + 1));
	(void) cmd_result; // line to delete
	// else if ()
	// else if ()
	// else if ()
	// else if ()
	// else if ()
	// else if ()
}

// 
bool	Server::set_commands(Client &client)
{
	std::string message = client.get_message();
	while (message.find("\n") != std::string::npos)
	{
		std::string command = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.set_message(message);
		if (client.get_status() == 0)
			return (check_password(client, command));
		else
		{
			// std::cout << "f" << command << std::endl;
			// if (command == "USER")
			commands_parsing(client, command);
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
					std::cout << "Client disconnected." << std::endl;
					it = this->_tab_client.erase(it);
				}
				buffer[size] = 0;
				message = message + buffer;
				it->set_message(message);
			} while (size == 1024);

			connexion = set_commands(*it);
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
	struct 	pollfd fds[NB_MAX_CLIENTS + 1];

	while (1)
	{
		bool	new_client = false;
		initialize_poll_fds(fds);
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

int	Server::getServerSocket()
{
	return (this->_server_socket);
}

void	Server::setPassword(std::string password)
{
	this->_password = password;
}

std::string		Server::getPassword(void)
{
	return (this->_password);
}

void	Server::initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServerSocket();
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

Server::Server(int port, std::string password)
{
	this->_password = password;
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);

	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_server_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServerSocket(), 5);
	runtime();
}

Server::~Server()
{
	for (std::vector<Client>::iterator it = this->_tab_client.begin(); it != this->_tab_client.end(); it++)
	{
		close(it->get_socket_fd());
	}
}
