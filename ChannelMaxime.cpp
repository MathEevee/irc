/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMaxime.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:50:42 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/22 00:24:49 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

bool Channel::add_client_in_channel(const Client& client)
{
	if (_l_mode && _client_list.size() >= _client_limit) 
		return (false);
	_client_list.push_back(client);
	return (true);
}

Channel::Channel(std::string name): _name(name){}

Channel::Channel(){}

Channel::~Channel(){}

bool Channel::getIMode() const
{
	return (_i_mode);
}

bool Channel::getTMode() const
{
	return (_t_mode);
}

std::string Channel::getPassword() const
{
	return (_password);
}

bool Channel::getLMode() const
{
	return (_l_mode);
}

size_t Channel::getClientLimit() const
{
	return (_client_limit);
}

std::vector<Client> Channel::getClientList() const
{
	return (_client_list);
}

std::vector<Client> Channel::getOperatorList() const
{
	return (_operator_list);
}

bool Channel::get_k_mode() const
{
	return (_k_mode);
}

std::string Channel::getName() const
{
	return (_name);
}
