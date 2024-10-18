/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/18 01:30:46 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Check is the provide password is the good password
bool	Server::checkPass(Client &client, std::string password)
{
	if (this->getPassword() == password && client.getStatus() == 0)
	{
		std::cout << "Good password" << std::endl;
		client.setStatus(1);
		return (true);
	}
	else if (client.getStatus() == 1)
	{
		client.send_error(462, ":You may not reregister");
		return (true);
	}
	client.send_error(464, ":Password incorrect");
	return (false);
}

// modify function using client informations
bool	Server::checkUser(Client& client, std::string data)
{
	std::string username;
	std::string real_name;
	std::string tmp = data;
	int i = 0;

	std::cout << "data = " << data << std::endl;
	if (client.getStatus() == 0)
	{
		std::cout << "Client not connected, use command 'PASS'" << std::endl;
		return (false);
	}

	if (!client.getUsername().size())
	{
		if (data.find(' ') == std::string::npos)
			return (false);

		while (i != 4)
		{
			if (i == 0)	
				username = data.substr(0, data.find(' '));
			if (i == 3)
				real_name = data.substr(data.find_last_of(' ') + 1);
			tmp = tmp.substr(tmp.find(' ') + 1);
			i++;
		}

		if (tmp != real_name)
			return (false);

		client.setUsername(username);
		client.setRealName(real_name);
		return (true);
	}
	return (false);
}

// NICK <nickname>
// Start by a letter
// Can include: letter, digits,  - | _ \  ` ^ { }
// Cannot include: space, ',', '~', '.'...
// Limit lenght: 13 char
// special name: admin, root, server, operator
// Have to be unique
bool	Server::checkNick(Client &client, std::string data) // recheck this function
{
	if (data.length() > 13)
	{
		client.send_error(432, data + " :Erroneous Nickname");
		return (false);
	}
	if (!isalpha(data[0]))
	{
		client.send_error(432, data + " :Erroneous Nickname");
		return (false);
	}
	for (size_t i = 1; i < data.length(); ++i)
	{
		if (!isalnum(data[i]) && data[i] != '-' && data[i] != '|' && data[i] != '_' &&
			data[i] != '\\' && data[i] != '`' && data[i] != '^' && data[i] != '{' && data[i] != '}')
		{
			client.send_error(432, data + " :Erroneous Nickname");
			return (false);
		}
	}
	std::string special_names[] = {"admin", "root", "server", "operator"};
	for (size_t i = 0; i < sizeof(special_names) / sizeof(special_names[0]); ++i)
	{
		if (special_names[i] == data)
		{
			client.send_error(432, data + " :Erroneous Nickname");
			return (false);
		}
	}
	if (this->findClientByNick(data) != NULL)
	{
		client.send_error(433, data + " :Nickname is already in use");
		return (false);
	}
	client.setNickname(data);
	return (true);
}

// recheck this function
Client* Server::findClientByNick(const std::string& nick)
{
	for (std::vector<Client>::iterator it = _client_list.begin(); it != _client_list.end(); ++it)
	{
		if (it->getNickname() == nick)
			return &(*it);
	}
	return (NULL);
}

// PRIV <nickname> <message>
bool	checkPrivmsg(Client &client, std::string data)
{
	(void) data;
	(void) client;
	return (false);
}