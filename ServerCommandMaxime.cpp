/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommandMaxime.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/22 01:37:02 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::checkPass(Client &client, std::string param)
{
	if (param.empty() || param.find_first_not_of(" \t\n") == std::string::npos)	
	{
		send_msg_to_client(client, "461", "PASS :Not enough parameters");
		return (false);
	}	
	else if (client.getStatus() == 1)
	{
		send_msg_to_client(client, "462", ":You may not reregister");
		return (false);		
	}
	else if (client.getStatus() == 0 && param == _password)
	{
		// PRINT msg on server?
		client.setStatus(1);
		return (true);
	}
	return (false);
}

// NICK <nickname>
// Start by a letter
// Can include: letter, digits,  - | _ \  ` ^ { }
// Cannot include: space, ',', '~', '.'...
// Limit lenght: 9 char
// special name: admin, root, server, operator
// Have to be unique
// recheck this function
bool	Server::checkNick(Client &client, std::string param) 
{
	if (param.empty() || param.find_first_not_of(" \t\n") == std::string::npos)	
	{
		send_msg_to_client(client, "431", ":No nickname given");
		return (false);
	}
	else if (param.length() > 9)
	{
		send_msg_to_client(client, "432", param + " :Erroneous Nickname");
		return (false);
	}
	else if (!isalpha(param[0]) || std::string(";[]\\`_^{}|").find(param[0]) == std::string::npos)
	{
		send_msg_to_client(client, "432", param + " :Erroneous Nickname");
		return (false);
	}
	for (size_t i = 1; i < param.length(); ++i)
	{
		if (!isalnum(param[i]) && std::string(";[]\\`_^{}|-").find(param[i]) == std::string::npos)
		{
			send_msg_to_client(client, "432", param + " :Erroneous Nickname");
			return (false);
		}
	}
	for (std::vector<Client>::iterator it = _client_list.begin(); it != _client_list.end(); ++it)
	{
		if (it->getNickname() == param)
		{
			send_msg_to_client(client, "433", param + " :Nickname is already in use");
			return (false);	
		}
	}
	client.setNickname(param);
	return (true);
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
	SHould I handle WHO?
*/
bool 	Server::checkJoin(Client &client, std::string param)
{
	std::string client_msg;
	std::string	channel;
	size_t 		pos;

	if (param[0] != '#')
	{
		client_msg = ":" + _name + " 403 " + client.getNickname() + " :No such channel \n"; // add param
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
		// client.send_error(433, param + " :No such channel");		
		// >> @time=2024-10-19T22:56:24.371Z :zinc.libera.chat 403 djjd d :No such channel		
		return (false);
	}
	pos = param.find(' ');
	if (pos != std::string::npos)
		channel = param.substr(1, pos);
	else
		channel = param;

	// check if the channel already exists
	Channel* existing_channel = find_channel_by_name(channel);
	if (existing_channel != NULL)
	{
		// If the channel exists, check if it requires a password
		if (existing_channel->get_k_mode())
		{
			std::string provided_password = param.substr(pos + 1);
			if (existing_channel->getPassword() != provided_password)
			{
				client_msg = ":" + _name + " 475 " + client.getNickname() + " " + channel + " :Cannot join channel (+k) - bad key\n";
				send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
				return (false);
			}
		}
		existing_channel->add_client_in_channel(client);
		// CHECK if client is added or if channel is full // what's the error?

		client_msg = ":" + _name + " JOIN " + channel + "\n";
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
	}
	// create the channel
	else
	{
		// If the channel does not exist, create it and add the client
		Channel new_channel(channel);
		_channel_list.push_back(new_channel);
		new_channel.add_client_in_channel(client);
		// client_msg = ":" + _name + " JOIN " + channel + "\n";
		// send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
		std::cout << "hello" << std::endl;
		client_msg = "@time=" + find_current_time() + " :" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN #" + channel + " * :" + client.getNickname() + "\n";
		// >> @time=2024-10-21T14:17:12.000Z :!~@localhost JOIN ##hdhdj * :
		// >> @time=2024-10-21T14:17:32.470Z :mbriand!~mbriand@rtr.23.90.210.21.unyc.it JOIN #hehe * :mbriand
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
	}
	return (true);
}

std::string	Server::find_current_time()
{
    std::time_t now = std::time(NULL);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&now));
    return std::string(buffer);
}

void Server::send_log_message(int clientSocket, const std::string& message) 
{
    send(clientSocket, message.c_str(), message.size(), 0);
}

Channel*	Server::find_channel_by_name(std::string& channel_name)
{
	for (std::vector<Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); ++it)
	{
		if (it->getName() == channel_name)
			return &(*it);
	}
	return (NULL);
}

    // send_log_message(client.getSocketFd(), "<< MODE #" + channel + "\n");
    // send_log_message(client.getSocketFd(), "<< WHO #" + channel + " %chtsunfra,152\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " MODE #" + channel + " +Cnst\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 353 " + nickname + " @ #" + channel + " :@" + nickname + "\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 366 " + nickname + " #" + channel + " :End of /NAMES list.\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 324 " + nickname + " #" + channel + " +Cnst\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 329 " + nickname + " #" + channel + " 1729463320\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 354 " + nickname + " 152 #" + channel + " ~" + username + " localhost " + _name + " " + nickname + " H@ 0 :" + realname + "\n");
    // send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 315 " + nickname + " #" + channel + " :End of /WHO list.\n");
