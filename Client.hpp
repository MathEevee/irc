/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:36:54 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/18 00:53:35 by mbriand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iomanip>
# include <iostream>
# include <sstream>

class Client
{
	private :
		std::string	_username;
		// hostname ??
		// servername ??
		std::string	_real_name;
		std::string	_nickname;
		int			_status;
		int			_socket_fd;
		std::string	_message_buffer; // data send to the server
		// std::vector<Channel> _channel_list;	
		// std::vector<bool> _user_mode; // only operators OR basic user
		// Last Activity Timestamp???
	
	public :
		void		send_error(int error, std::string msg_error);

		Client(int socket_fd);
		~Client();

		void		setUsername(std::string username);
		void		setRealName(std::string real_name);
		void		setNickname(std::string nickname);
		void		setStatus(int status);
		void		setSocketFd(int socket_fd);
		void		setMessage(std::string message);

		std::string	getUsername(void);
		std::string	getRealName(void);
		std::string	getNickname(void);
		int			getStatus(void);
		int			getSocketFd(void);
		std::string	getMessage(void);
};

#endif