Protocol: System who declare and set how to part would communication. Example: https, is an internet protocol to send a request to a server.

IRC Protocol: define how IRC clients and IRC server communicate with each others like:
- How clients connect to the server: authentication with password, setting nicknames
- How clients interact with each other: joining channels, sending messages
- How the server should handle multiple client

IRC server:
- Communication types:
  - Client to client (private messages)
  - Client to channel (global messages)

IRC client:
Context: Using Hexchat I can create a client and connect it to a network (group of server).
Components:
- Username:
- Realname:
- Nickname: Unique client id that other clients can use to communicate with it.
- Status:
- SocketFd:
Commands:
- /nick <nickname>: change nickname
- /join <# channel> [password]: create or join a channel ?channel password?
- /pass: 
- /user <username> <hostname> <servername> <realname> 
- /msg <nickname> <private messsage>.
Examples:
- Via terminal using: nc localhost <port>
- HexChat (most popular)
- WeeChat
- irssi

Step 1: Launch the server with a port and a passpord
Step 2: Create a client and connection him to a port server
Step 3: Unlock command sending the password to the server
Step 4: 


