/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:36:54 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/17 16:58:20 by matde-ol         ###   ########.fr       */
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
		std::string	_real_name;
		std::string	_nickname;
		// std::vector<Channel> _channel_list;	
		int			_socket_fd;
		int			_status; // is the client is connected to the server - if the password
		std::string	_message_buffer; // data send to the server
	
	public :
		bool			checkUser(std::string data); // We can do create this same system of checking for each command
		// bool checkNick ....  

		void			set_socket_fd(int socket_fd);
		void			set_status(int status);
		void			set_message(std::string message);
		void			set_username(std::string username);
		void			set_real_name(std::string real_name);
		void			setNickname(std::string nickname);

		int				get_socket_fd(void);
		int				get_status(void);
		// int				get_count_connexion(void);
		std::string		get_message(void);
		std::string		get_username(void);
		std::string		get_real_name(void);
		// std::string		get_nickname(void);

		void			send_error(int error, std::string msg_error);


		Client(int socket_fd);
		~Client();
};

#endif