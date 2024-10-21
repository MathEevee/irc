/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/21 23:47:08 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::send_private_message(Client &recipient, std::string msg)
{
	std::string all_message = this->getNickname() + ": " + msg + "\r\n";	
	send(recipient.getSocketFd(), all_message.c_str(), all_message.size(), 0);
}

void	Client::send_error(int error, std::string msg_error)
{
	std::stringstream out;
	
	std::cout << error << std::endl;
	out << error;
	std::string msg = ":127.0.0.1 " + out.str() + " " + msg_error + "\r\n";
	send(this->getSocketFd(), msg.c_str(), msg.size(), 0);
	std::cerr << msg_error << std::endl;
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

std::string	Client::getUsername(void) const
{
	return (this->_username);
}

std::string	Client::getRealName(void) const
{
	return (this->_real_name);
}

std::string	Client::getNickname(void) const
{
	return (this->_nickname);
}

int	Client::getStatus() const
{
	return (this->_status);
}

int	Client::getSocketFd() const
{
	return (this->_socket_fd);
}

std::string	Client::getMessage() const
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
