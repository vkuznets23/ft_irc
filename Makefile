# **************************************************************************************** #
#                                                                                          #
#                                                        ::::::::::: :::::::::   ::::::::  #
#                                                           :+:     :+:    :+: :+:    :+:  #
#                                                          :+:     :+:    :+: :+:          #
#                                                         :+:     :+:+:+:+:  :+:           #
#  By: Viktoriia Kuznetsova<vkuznets@student.hive.fi>    :+:     :+:    :+: :+:            #
#      Juliette Mouette<jmouette@student.hive.fi>,      :+:     :+:    :+: :+:    :+:      #
#      									   	       ::::::::::: :::    :::  ::::::::        #
#                                                                                          #
# **************************************************************************************** #


NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++11

SRC_DIR = src
CMD_DIR = src/commands
OBJ_DIR = obj
CMD_OBJ_DIR = $(OBJ_DIR)/commands

SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Client.cpp $(SRC_DIR)/runServer.cpp $(SRC_DIR)/Server.cpp\
		$(SRC_DIR)/handleMessage.cpp \
		$(CMD_DIR)/Pass.cpp $(CMD_DIR)/Nick.cpp $(CMD_DIR)/UserName.cpp $(CMD_DIR)/Cap.cpp

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.SILENT:

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	echo "ircserv compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(CMD_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	echo "🧹 Cleaned object files."

fclean: clean
	rm -f $(NAME)
	echo "🧹 Fully cleaned."

re: fclean all

.PHONY: all clean fclean re
