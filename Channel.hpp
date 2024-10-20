/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:50:19 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/21 00:22:43 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

class Client;

# include "Client.hpp"
# include <string>
# include <vector>

class Channel 
{
   public:
		bool				add_client_in_channel(const Client& client);		

   private:
	   	std::string			_name;
		std::vector<Client>	_client_list;
		std::vector<Client> _operator_list;
		bool				_i_mode;
		bool				_t_mode;
		bool				_k_mode;
		std::string			_password;
		bool				_l_mode;
		size_t				_client_limit;

   public:
		Channel(std::string name);
		Channel();
		~Channel();
		std::string			getName() const;
		bool				get_k_mode() const;
		bool 				getIMode() const;
		bool 				getTMode() const;
		std::string 		getPassword() const;
		bool 				getLMode() const;
		size_t 				getClientLimit() const;
		std::vector<Client> getClientList() const;
		std::vector<Client> getOperatorList() const;
};

#endif
