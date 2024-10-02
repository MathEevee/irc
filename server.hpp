/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:25:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/01 17:47:17 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include <poll.h>
# include "client.hpp"
# define NB_MAX_CLIENTS 999

class Server
{
	private :
		int						_port;
		int						_serv_socket;
		sockaddr_in 			_serverAddress;
		std::vector<Client>		_tab_client;
		std::string				_password;

		void					runtime();
		void					set_fds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool					add_client();
		void					read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool					set_commande(Client &client);
		bool					check_password(Client &client, std::string password);
	public :
		Server(int port, std::string password);
		~Server();
		
		int				getServSocket();
		std::string		get_password(void);
		void			set_password(std::string password);
		
};

#endif