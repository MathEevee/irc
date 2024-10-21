/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/21 13:40:21 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::runtime()
{
	struct 	pollfd fds[NB_MAX_CLIENTS + 1];

	while (1)
	{
		bool	new_client = false;
		initialize_poll_fds(fds);
		int	nb_fd = poll(fds, _client_list.size() + 1, -1);
		if (nb_fd == -1 && errno == EINTR)
			return;
		if ((fds[0].revents & POLLIN) != 0)
		{
			new_client = add_client();
		}
		read_all_clients(fds, new_client);
	}
}

void	Server::initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServerSocket();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int	i = 1;
	for (std::vector<Client>::iterator it = _client_list.begin(); it != _client_list.end(); it++)
	{
		fds[i].fd = it->getSocketFd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
	// std::cout << i << std::endl;
}

// Add a client to the server list
bool	Server::add_client()
{
	int clientSocket = accept(this->getServerSocket(), NULL, NULL);
	if (clientSocket != -1)
	{
		if (this->_client_list.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			std::cout << "Too many clients" << std::endl;
		}
		else
		{
			Client	new_client(clientSocket);
			this->_client_list.push_back(new_client);
			std::cout << "New client connected." << std::endl;
			return (true);
		}
	}
	return (false);
}

// Looking for command / information from each server client
void	Server::read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int		i = 1;
	ssize_t size;
	char	buffer[1024];

	for (std::vector<Client>::iterator it = this->_client_list.begin(); it != this->_client_list.end() - new_client;)
	{
		std::string	message = it->getMessage();
		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (size == 0)
				{
					it->setDisconnected(true);
					it = this->_client_list.erase(it);
				}
				buffer[size] = 0;
				message = message + buffer;
				it->setMessage(message);
			} while (size == 1024);
			i++;
			if (size == 0)
				continue;
		}
		if ((*it).getDisconnected() == false)
			it++;
		else
			this->sendToAll(*it);
	}
}

// 
bool	Server::process_commands(Client &client)
{
	std::string message;
	
	message = client.getMessage();
	while (message.find("\n") != std::string::npos)
	{
		std::string command = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.setMessage(message);
		commands_parsing(client, command);
	}
	return (true);
}

// Parse and execute the command based on the first word
void	Server::commands_parsing(Client &client, std::string input)
{
	bool		result;
	std::string	command;
	// size_t		command_len;

	result = false;
	// command_len = input.find(' ');
	command = input.substr(0, input.find(' '));
	if (input.find(' ') ==  std::string::npos)
		return ;
	if (command == "PASS")
		result = checkPass(client, input.substr(input.find(' ') + 1));
	//add checkpoint connection for PASS, not connected send error & stop 
	if (command == "USER")
		result = checkUser(client, input.substr(input.find(' ') + 1));
	if (command == "NICK")
		result = checkNick(client, input.substr(input.find(' ') + 1));
	//add checkpoint to check user initialized, not initialized send error & stop
	if (command == "PRIVMSG")
		result = checkPrivmsg(client, input.substr(input.find(' ') + 1));
	if (command == "JOIN")
		result = checkJoin(client, input.substr(input.find(' ') + 1));

	(void) result; // how do u want to use this variable?
}

void	Server::sendToAll(Client &client)
{
	//for() loop all chann to send in all channel disconnected
	std::string all_message = client.getNickname() + ": " + "disconnected" + "\r\n";
	send(this->getServerSocket(), all_message.c_str(), all_message.size(), 0);
}


Server::Server(int port, std::string password)
{
	this->_name = "thewonderfulserver";
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
	for (std::vector<Client>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		close(it->getSocketFd());
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

