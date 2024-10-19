/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/20 01:41:19 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Check is the provide password is the good password
bool	Server::checkPass(Client &client, std::string password)
{
	if (this->getPassword() == password && client.getStatus() == 0)
	{
		std::cout << "Client authentificated." << std::endl;
		std::string client_msg = ":" + _name + ": Your are authentificated." + "\n";
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);		
		client.setStatus(1);
		return (true);
	}
	else if (client.getStatus() == 1)
	{
		client.send_error(462, ":You may not reregister");
		return (true);
	}
	client.send_error(464, "Failed password attempt.");
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
		//add error_send
		std::cout << "Client not connected, use command 'PASS'" << std::endl;
		return (false);
	}
	
	// checkData for #, @ 
	
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
			if (isalpha(tmp[0]) < 0)
			{
				//check_len
				//bad letter for arg
			}
			i++;
		}

		if (tmp != real_name)
		{
			//error to many args
			return (false);
		}
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
	std::cout << "Client set a valid nickname: " << data << std::endl;
	std::string client_msg = ":" + _name + " " + client.getNickname() + " :Your Nickname is set." + "\n";
	send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
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
bool	Server::checkPrivmsg(Client &client, std::string data)
{
	std::string recipient = data.substr(0, data.find(' '));
	
	if (recipient[0] == '#')
	{
		std::cout << "send in channel : " << recipient << " : " << data.substr(data.find(' ') + 1) << std::endl; 
		//checkChannel & send message or error, channel doesn't exist
	}
	else
	{
		Client *interlocutor = this->findClientByNick(recipient);
		if (interlocutor == NULL)
		{
			std::cout << "976 can't send message to user :" << recipient << std::endl;
			//send error interlocutor doesn't exist
		}
		else
		{
			client.send_private_message(*interlocutor, data.substr(data.find(' ') + 1));
		}
			
	}
	return (false);
}

// QUIT :jdjdjjd (with a reason)
// QUIT (without reason)
// bool Server::checkQuit(Client &client, std::string data)
// {
// 	if (data.empty())
// 	{
// 		std::cout << "Client quit without a reason." << std::endl;
// 		// client.disconnect();
// 		return (true);
// 	}
// 	if (data[0] == ':' && data.size() < 100)
// 	{
// 		std::cout << "Client quit with reason: " << data.substr(1) << std::endl;
// 		// client.disconnect(data.substr(1));
// 		return (true);
// 	}
// 	return (false);
// }

/*
	1. Join or create a non-protected channel: "JOIN #publicChannel"
	2. Password-protected: "JOIN #privateChannel password123"
	3. Join several channel?
*/
bool 	Server::checkJoin(Client &client, std::string data)
{
	std::string client_msg;
	std::string	channel;

	std::cout << "'" << data << "'" << std::endl;

	// if (data == std::string::npos)
	// {
	// 	std::cout << "Usage: JOIN <channel>, joins the channel" << std::endl;
	// 	return (false);
	// }

	if (data[0] != '#')
	{
		client_msg = ":" + _name + " 403 " + client.getNickname() + " :No such channel \n"; // add param
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);

		// client.send_error(433, data + " :No such channel");		
// >> @time=2024-10-19T22:56:24.371Z :zinc.libera.chat 403 djjd d :No such channel		
		return (false);
	}
	
	return (true);

}
