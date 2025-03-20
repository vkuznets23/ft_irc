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
OBJ_DIR = obj

SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Client.cpp $(SRC_DIR)/runServer.cpp $(SRC_DIR)/Server.cpp $(SRC_DIR)/handleMessage.cpp

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.SILENT:

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	echo "ircserv compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	echo "🧹 Cleaned object files."

fclean: clean
	rm -f $(NAME)
	echo "🧹 Fully cleaned."

re: fclean all

.PHONY: all clean fclean re