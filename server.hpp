/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:25:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/17 00:05:37 by mbriand          ###   ########.fr       */
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

// Maybe have to delete ?
#include <csignal>
#include <cstdlib>
#include <errno.h>

class Server
{
	private :
		int						_server_socket;
		// int						_port;
		sockaddr_in 			_serverAddress;
		std::vector<Client>		_tab_client;
		std::string				_password;

		void					runtime();
		void					initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool					add_client();
		void					read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool					set_commands(Client &client);
		bool					check_password(Client &client, std::string password);
	public :
		Server(int port, std::string password);
		~Server();

		void					commands_parsing(Client &client, std::string commande);
		
		int						getServerSocket();
		std::string				getPassword(void);
		void					setPassword(std::string password);
};

#endif