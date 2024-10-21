NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98 -g
CPP = c++

SRC =	main.cpp \
		Client.cpp \
		Server.cpp \
		ServerCommand.cpp \
		Channel.cpp

OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CPP) $(FLAGS) -c $< -o $@ 

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean