/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriand <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:25:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/18 18:53:35 by mbriand          ###   ########.fr       */
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
# include "Client.hpp"
#include <algorithm>
# define NB_MAX_CLIENTS 999

// Maybe have to delete ?
#include <csignal>
#include <cstdlib>
#include <errno.h>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <cstdlib>

class Server
{
	private :
		std::string			_name;
		int					_server_socket;
		sockaddr_in 		_serverAddress;
		std::vector<Client>	_client_list;
		std::string			_password;
		// int					_port;

		void	runtime();
		void	initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool	add_client();
		void	read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool	process_commands(Client &client);
		
		bool	checkPass(Client &client, std::string password);
		bool	checkUser(Client &client, std::string data);
		bool	checkNick(Client &client, std::string data);
		bool	checkPrivmsg(Client &client, std::string data);
		bool 	checkQuit(Client &client, std::string data);
		// PRIVMSG
		// Ping
		// pong

	// channel management
		// JOIN msg
		// privmsg
		// PART

	// operators commands
		// MODE
		// TOPIC?
		// INVIT
		// KICK
		Client* findClientByNick(const std::string& nick);


	public :
		void		commands_parsing(Client &client, std::string commande);
		
		Server(int port, std::string password);
		~Server();
		
		int			getServerSocket();
		std::string	getPassword(void);
		void		setPassword(std::string password);
};

#endif