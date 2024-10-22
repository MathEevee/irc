/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/21 19:44:32 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Check is the provide password is the good password
bool	Server::checkPass(Client &client, std::string password)
{
	std::cout << "|" << password << "|"  << std::endl;
	if (this->getPassword() == password && client.getStatus() == 0)
	{
		std::cout << "Client authentificated." << std::endl;
		std::string client_msg = ":" + _name + ": Your are authentificated." + "\r\n";
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

	if (client.getStatus() == 0)
	{
		//add error_send
		std::cout << "Client not connected, use command 'PASS'" << std::endl;
		return (false);
	}
	
	// checkData for #, @
	std::cout << data << std::endl;
	for (std::string::iterator it = data.begin(); it != data.end(); it++)
	{
		if (isalnum(*it) == 0 && *it != ' ' && *it != '-' && *it != '_' && *it != '`' && *it != '^' && *it != '{' && *it != '}')
		{
			client.send_error(421, "USER :Unknown command");
			return (false);
		}
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
		{
			//error to many args
			client.send_error(461, "USER :Not enough parameters");
			return (false);
		}
		client.setUsername(username);
		client.setRealName(real_name);
		//client set Username
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

	for (size_t i = 1; i < data.length() && data[i] != ' '; ++i)
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
bool Server::checkQuit(Client &client, std::string data)
{
	(void) client;
	std::cout << data << std::endl;
	return (true);
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
// 	}
//	return (true);
}

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
		// << JOIN #newchan
		// >> @time=2024-10-20T22:30:42.454Z :mbriand_!~mbriand@2001:861:5301:fd60:a93f:b017:6736:b44e JOIN #newchan * :mbriand
		// << MODE #newchan
		// << WHO #newchan %chtsunfra,152
		// >> @time=2024-10-20T22:30:42.454Z :tungsten.libera.chat 353 mbriand_ @ #newchan :mbriand_ @mbriand
		// >> @time=2024-10-20T22:30:42.454Z :tungsten.libera.chat 366 mbriand_ #newchan :End of /NAMES list.
		// >> @time=2024-10-20T22:30:42.558Z :tungsten.libera.chat 324 mbriand_ #newchan +Cnst
		// >> @time=2024-10-20T22:30:42.558Z :tungsten.libera.chat 329 mbriand_ #newchan 1729463320
		// >> @time=2024-10-20T22:30:42.621Z :tungsten.libera.chat 354 mbriand_ 152 #newchan ~mbriand 2001:861:5301:fd60:a93f:b017:6736:b44e tungsten.libera.chat mbriand_ H 0 :mbriand
		// >> @time=2024-10-20T22:30:42.621Z :tungsten.libera.chat 354 mbriand_ 152 #newchan ~mbriand 2001:861:5301:fd60:a93f:b017:6736:b44e mercury.libera.chat mbriand H@ 0 :mbriand
		// >> @time=2024-10-20T22:30:42.621Z :tungsten.libera.chat 315 mbriand_ #newchan :End of /WHO list.
	
	
	}
	// create the channel
	else
	{
		// If the channel does not exist, create it and add the client
		Channel new_channel(channel);
		_channel_list.push_back(new_channel);
		new_channel.add_client_in_channel(client);
		client_msg = ":" + _name + " JOIN " + channel + "\n";
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
	
	// << JOIN #newchan
	// >> @time=2024-10-20T22:28:40.836Z :mbriand!~mbriand@2001:861:5301:fd60:a93f:b017:6736:b44e JOIN #newchan * :mbriand
	// << MODE #newchan
	// << WHO #newchan %chtsunfra,152
	// >> @time=2024-10-20T22:28:40.837Z :mercury.libera.chat MODE #newchan +Cnst
	// >> @time=2024-10-20T22:28:40.837Z :mercury.libera.chat 353 mbriand @ #newchan :@mbriand
	// >> @time=2024-10-20T22:28:40.837Z :mercury.libera.chat 366 mbriand #newchan :End of /NAMES list.
	// >> @time=2024-10-20T22:28:40.887Z :mercury.libera.chat 324 mbriand #newchan +Cnst
	// >> @time=2024-10-20T22:28:40.887Z :mercury.libera.chat 329 mbriand #newchan 1729463320
	// >> @time=2024-10-20T22:28:40.911Z :mercury.libera.chat 354 mbriand 152 #newchan ~mbriand 2001:861:5301:fd60:a93f:b017:6736:b44e mercury.libera.chat mbriand H@ 0 :mbriand
	// >> @time=2024-10-20T22:28:40.911Z :mercury.libera.chat 315 mbriand #newchan :End of /WHO list.

	// std::cout << "<< JOIN #" << channel << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.836Z :" << client.getNickname() << "!~" << client.getUsername() << "@localhost JOIN #" << channel << " * :" << client.getNickname() << std::endl;
	// std::cout << "<< MODE #" << channel << std::endl;
	// std::cout << "<< WHO #" << channel << " %chtsunfra,152" << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.837Z :" << _name << " MODE #" << channel << " +Cnst" << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.837Z :" << _name << " 353 " << client.getNickname() << " @ #" << channel << " :@" << client.getNickname() << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.837Z :" << _name << " 366 " << client.getNickname() << " #" << channel << " :End of /NAMES list." << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.887Z :" << _name << " 324 " << client.getNickname() << " #" << channel << " +Cnst" << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.887Z :" << _name << " 329 " << client.getNickname() << " #" << channel << " 1729463320" << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.911Z :" << _name << " 354 " << client.getNickname() << " 152 #" << channel << " ~" << client.getUsername() << " localhost " << _name << " " << client.getNickname() << " H@ 0 :" << client.getRealName() << std::endl;
	// std::cout << ">> @time=2024-10-20T22:28:40.911Z :" << _name << " 315 " << client.getNickname() << " #" << channel << " :End of /WHO list." << std::endl;

	std::string nickname = client.getNickname();
    std::string username = client.getUsername();
    std::string realname = client.getRealName();
    std::string timestamp = find_current_time();
    
	send_log_message(client.getSocketFd(), "<< JOIN #" + channel + "\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + nickname + "!~" + username + "@localhost JOIN #" + channel + " * :" + nickname + "\n");
    send_log_message(client.getSocketFd(), "<< MODE #" + channel + "\n");
    send_log_message(client.getSocketFd(), "<< WHO #" + channel + " %chtsunfra,152\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " MODE #" + channel + " +Cnst\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 353 " + nickname + " @ #" + channel + " :@" + nickname + "\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 366 " + nickname + " #" + channel + " :End of /NAMES list.\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 324 " + nickname + " #" + channel + " +Cnst\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 329 " + nickname + " #" + channel + " 1729463320\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 354 " + nickname + " 152 #" + channel + " ~" + username + " localhost " + _name + " " + nickname + " H@ 0 :" + realname + "\n");
    send_log_message(client.getSocketFd(), ">> @time=" + timestamp + " :" + _name + " 315 " + nickname + " #" + channel + " :End of /WHO list.\n");
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
