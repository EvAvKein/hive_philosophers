# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/26 19:12:41 by ekeinan           #+#    #+#              #
#    Updated: 2025/03/03 15:57:29 by ekeinan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo

SRC_DIR := src
SRC_FILES := main.c \
			 launch_feast.c \
			 routine.c \
			 utils.c \
			 cleanup.c

INCLUDE_DIR := include
INCLUDE_FILES := philo.h

COMPILE_FLAGS := -Wall -Wextra -Werror -I$(INCLUDE_DIR)

OBJ := $(SRC_FILES:%.c=$(SRC_DIR)/%.o)
HEADERS := $(INCLUDE_FILES:%=$(INCLUDE_DIR)/%)

all: $(NAME)

%.o: %.c
	cc $(COMPILE_FLAGS) -c $< -o $@

$(NAME): $(OBJ) $(HEADERS)
	cc $(COMPILE_FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ) $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

neat: $(NAME) clean
	clear

.PHONY: all bonus clean fclean re