/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/16 18:39:55 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int socket_fd)
{
	this->set_socket_fd(socket_fd);
	this->set_status(0);
	// this->set_username("");
	// this->set_nickname("");
	// this->set_real_name("");
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

// std::string		Client::get_username(void)
// {
	// return (this->_username);
// }

// std::string		Client::get_real_name(void)
// {
	// return (this->_real_name);
// }

// std::string		Client::get_nickname(void)
// {
	// return (this->_nickname);
// }

// void		Client::set_username(std::string username)
// {
// 	this->_username = username;
// }

// void		Client::set_real_name(std::string real_name)
// {
// 	this->_real_name = real_name;
// }

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
