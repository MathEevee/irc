/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/16 20:58:34 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int socket_fd)
{
	this->set_socket_fd(socket_fd);
	this->set_status(0);
	this->set_username("");
	this->set_nickname("");
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

bool		Client::set_user(std::string data)
{
	std::string username;
	std::string real_name;

	std::cout << "data = " << data << std::endl;
	if (!this->_username.size())
	{
		if (data.find(' ') == std::string::npos)
			return (false);
		username = data.substr(0, data.find(' '));
		real_name = data.substr(data.find(' ') + 1);
		if (real_name.find(' ') != std::string::npos)
			return (false);
		this->set_username(username);
		this->set_real_name(real_name);
	}
	return (true);
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

// void		Client::set_nickname(std::string nickname)
// {
// 	this->_nickname = nickname;
// }

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
