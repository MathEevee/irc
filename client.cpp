/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/01 17:07:44 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int socket_fd)
{
	this->set_socket_fd(socket_fd);
	this->set_status(0);
	this->set_count_connexion(0);
}

int	Client::get_status()
{
	return (this->_status);
}

int	Client::get_socket_fd()
{
	return (this->_socket_fd);
}

int	Client::get_count_connexion()
{
	return (this->_count_connexion);
}

std::string	Client::get_message()
{
	return (this->_message);
}

void	Client::set_socket_fd(int socket_fd)
{
	this->_socket_fd = socket_fd;
}

void	Client::set_count_connexion(int count_connexion)
{
	this->_count_connexion = count_connexion;
}

void	Client::set_message(std::string message)
{
	this->_message = message;
}

void	Client::set_status(int status)
{
	this->_status = status;
}

Client::~Client()
{}
