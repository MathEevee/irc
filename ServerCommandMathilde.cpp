/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommandMathilde.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/22 00:27:57 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
