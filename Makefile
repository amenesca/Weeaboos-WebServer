NAME = webserver

CPP	= g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

CLASSES =	srcs/Client/Client.cpp \
			srcs/cgi/cgiHandler.cpp\
			srcs/Socket/Socket.cpp\
			srcs/WebServer/WebServer.cpp\
			srcs/ConfigParser/ConfigParser.cpp\
			srcs/Utils/Utils.cpp\
			srcs/VirtualServer/VirtualServer.cpp\
			srcs/RequestParser/RequestParser.cpp\
			srcs/Response/Response.cpp

SRCS = $(CLASSES) srcs/main.cpp

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
