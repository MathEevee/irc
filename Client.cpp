/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/18 11:30:10 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


void	Client::send_error(int error, std::string msg_error)
{
	std::stringstream out;
	
	out << error;
	std::string msg = ":127.0.0.1 " + out.str() + " " + msg_error;
	send(this->getSocketFd(), msg.c_str(), msg.size(), 0);
	// std::cout << msg << std::endl;
	std::cout << msg_error << std::endl;
}

Client::Client(int socket_fd)
{
	this->setUsername("");
	this->setRealName("");
	this->setNickname("");
	this->setStatus(0);
	this->setSocketFd(socket_fd);
	// set other elements?
}

Client::~Client()
{}

std::string	Client::getUsername(void)
{
	return (this->_username);
}

std::string	Client::getRealName(void)
{
	return (this->_real_name);
}

std::string	Client::getNickname(void)
{
	return (this->_nickname);
}

int	Client::getStatus()
{
	return (this->_status);
}

int	Client::getSocketFd()
{
	return (this->_socket_fd);
}

std::string	Client::getMessage()
{
	return (this->_message_buffer);
}

void	Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::setRealName(std::string real_name)
{
	this->_real_name = real_name;
}

void	Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::setStatus(int status)
{
	this->_status = status;
}

void	Client::setSocketFd(int socket_fd)
{
	this->_socket_fd = socket_fd;
}

void	Client::setMessage(std::string message)
{
	this->_message_buffer = message;
}
