/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:50:19 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/18 19:09:24 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

# include <string>
# include <vector>
# include <Client.hpp>

class Channel 
{
   public:
		// send messages to channel members

   private:
	   	std::string			_name;
		std::vector<Client>	_user_list;
		std::vector<Client> _operator_list;
		bool				_i_mode;
		bool				_t_mode;
		bool				_k_mode;
		std::string			_password;
		bool				_l_mode;
		size_t				_user_limit;

   public:
		Channel();
		~Channel();
};

#endif
