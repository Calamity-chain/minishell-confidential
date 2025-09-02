# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 12:10:35 by rmei              #+#    #+#              #
#    Updated: 2025/06/02 17:06:07 by rmei             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC_DIR = src
OBJ_DIR = obj

SRCS = atoi bzero calloc error get_next_line haschar isalnum isalpha isascii isdigit \
	isprint isspace itoa lstnew lstadd_back lstadd_front lstclear lstdelone lstiter \
	lstlast lstmap lstsize memchr memcmp memcpy memmove memset numlen printf putchar_fd \
	putendl_fd putnbr_fd putnbr_base_fd putstr_fd realloc split substr strchr strdup \
	striteri strjoin strlcat strlcpy strlen strmapi strncmp strnstr strrchr strtrim \
	tolower toupper
SRCS := $(addsuffix .c, $(addprefix $(SRC_DIR)/ft_, ${SRCS}))
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Logger
LOG = "[libft]"
GREEN = \033[0;32m
BLUE = \033[0;34m
RESET = \033[0m

define log_info
	@echo "$(shell date +"%d-%m-%Y %T") $(GREEN)$(LOG) $(BLUE)INFO$(RESET)$(1)"
endef

# ---- RULES ---- #
.PHONY: all end clean fclean re

all: $(NAME) end

$(NAME): $(OBJS)
	@ar -rcs $@ $^
	@$(call log_info, "Archive ´$(NAME)´ created")

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(call log_info, "Compiled $< to $@")
	@cc -Wall -Wextra -Werror -Iinclude -c $< -o $@

end:
	@$(call log_info, "Everything is done!")

clean:
	@$(call log_info, "Deleting object files if any...")
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(call log_info, "Deleting archive ´$(NAME)´ if present...")
	@rm -f $(NAME)

re: fclean all
