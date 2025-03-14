# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/26 19:12:41 by ekeinan           #+#    #+#              #
#    Updated: 2025/03/14 12:44:29 by ekeinan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo

SRC_DIR := src
SRC_FILES := main.c \
			 launch_feast.c \
			 philo.c \
			 grim_reaper.c \
			 time.c \
			 utils.c \
			 cleanup.c

INCLUDE_DIR := include
INCLUDE_FILES := philo.h color.h

COMPILE_FLAGS := -Wall -Wextra -Werror -I$(INCLUDE_DIR)
DEBUG_FLAGS := -g

OBJ := $(SRC_FILES:%.c=$(SRC_DIR)/%.o)
HEADERS := $(INCLUDE_FILES:%=$(INCLUDE_DIR)/%)

all: $(NAME)

%.o: %.c
	cc $(COMPILE_FLAGS) -D COLOR=$(if $(COLOR),1,0) -c $< -o $@

$(NAME): $(OBJ) $(HEADERS)
	cc $(COMPILE_FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ) $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

neat: $(NAME) clean
	clear

color: export COLOR = 1
color: re

debug: COMPILE_FLAGS += $(DEBUGFLAGS)
debug: re

.PHONY: all bonus clean fclean re color debug