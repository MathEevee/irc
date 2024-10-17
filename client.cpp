/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/17 17:00:04 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int socket_fd)
{
	this->set_socket_fd(socket_fd);
	this->set_status(0);
	this->set_username("");
	this->setNickname("");
	this->set_real_name("");
}

int	Client::get_status()
{
	return (this->_status);
}

int	Client::get_socket_fd()
{
	return (this->_socket_fd);
}

std::string	Client::get_message()
{
	return (this->_message_buffer);
}

void	Client::send_error(int error, std::string msg_error)
{
	std::stringstream out;
	out << error;
	std::string msg = ":127.0.0.1 " + out.str() + " " + msg_error;
	std::cout << msg << std::endl;
	std::cout << msg_error << std::endl;
}

bool		Client::checkUser(std::string data)
{
	std::string username;
	std::string real_name;
	std::string tmp = data;
	int i = 0;

	std::cout << "data = " << data << std::endl;
	if (this->get_status() == 0)
	{
		std::cout << "Client not connected, use command 'PASS'" << std::endl;
		return (false);
	}

	if (!this->_username.size())
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

		this->set_username(username);
		this->set_real_name(real_name);
		return (true);
	}
	return (false);
}


std::string		Client::get_username(void)
{
	return (this->_username);
}

std::string		Client::get_real_name(void)
{
	return (this->_real_name);
}

// std::string		Client::get_nickname(void)
// {
	// return (this->_nickname);
// }

void		Client::set_username(std::string username)
{
	this->_username = username;
}

void		Client::set_real_name(std::string real_name)
{
	this->_real_name = real_name;
}

void		Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::set_socket_fd(int socket_fd)
{
	this->_socket_fd = socket_fd;
}

void	Client::set_message(std::string message)
{
	this->_message_buffer = message;
}

void	Client::set_status(int status)
{
	this->_status = status;
}

Client::~Client()
{}
