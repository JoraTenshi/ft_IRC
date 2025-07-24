NAME = IrcServer

CC = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror -Iinc
REMOVE = rm -rf
 
SRC = src/main.cpp\
	src/Server.cpp\
	src/User.cpp\
	src/Message.cpp\
	src/Channel.cpp\
	src/Commands/ExecuteCommand.cpp\
	src/Commands/JoinCmd.cpp\
	src/Commands/NickCmd.cpp\
	src/Commands/PassCmd.cpp\
	src/Commands/UserCmd.cpp\
	src/Commands/PrivMsgCmd.cpp\
	src/Commands/PartCmd.cpp\
	src/Commands/PingCmd.cpp\
	src/Commands/QuitCmd.cpp

OBJS = $(addprefix obj/, $(SRC:.cpp=.o))

BLUE = \033[1;38;5;153m
RED = \033[0;31m
RESET = \033[0m
U_LINE = \033[4m
YELLOW = \033[1;38;5;226m

all: $(NAME)
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\n$(BLUE)$(U_LINE)🥳🎊 $(NAME): Compiled 🥳🎊$(RESET) \n"

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(REMOVE) $(OBJS) obj/
	@echo "\n$(RED)$(U_LINE)$(NAME): Cleaned$(RESET) \n"

fclean: clean
	@$(REMOVE) $(NAME)

re: fclean all

.PHONY: all clean fclean re
