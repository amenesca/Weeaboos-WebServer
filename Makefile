NAME = webserver

CPP	= g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

CLASSES =	srcs/Socket/Socket.cpp\
			srcs/WebServer/WebServer.cpp\
			srcs/ConfigParser/ConfigParser.cpp\
			srcs/Utils/Utils.cpp\
			srcs/VirtualServer/VirtualServer.cpp\
			srcs/cgi/cgiHandler.cpp\
			srcs/Client/Client.cpp

SRCS = srcs/main.cpp $(CLASSES)

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CPP) -c $< $(CPPFLAGS) -o $@

$(NAME): $(OBJS)
	$(CPP) $^ $(CPPFLAGS) -o $@
	printf "COMPILATION SUCCESSFUL!\n"

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	printf "FULLY CLEANED!\n"

re: fclean all

.SILENT:

.PHONY: all clean fclean re
