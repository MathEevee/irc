/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:36:54 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/01 17:06:44 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iomanip>
# include <iostream>

class Client
{
	private :
		int			_socket_fd;
		int			_status;
		int			_count_connexion;
		std::string	_message;
	public :
		int				get_socket_fd(void);
		int				get_status(void);
		int				get_count_connexion(void);
		std::string		get_message(void);
		void			set_socket_fd(int socket_fd);
		void			set_status(int status);
		void			set_count_connexion(int status);
		void			set_message(std::string message);

		Client(int socket_fd);
		~Client();
};

#endif