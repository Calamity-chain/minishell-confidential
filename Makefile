# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/01 11:50:45 by rmei              #+#    #+#              #
#    Updated: 2025/06/02 17:05:17 by rmei             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

SRC_DIR = src
OBJ_DIR = obj
DIRS = $(sort $(dir $(OBJS)))
LIBFT_DIR = lib/libft

SRCS = lexer main signals commands/pwd commands/echo commands/exit
SRCS := $(addsuffix .c, $(addprefix $(SRC_DIR)/, ${SRCS}))
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Logger
LOG = "[minishell]"
PURPLE = \033[0;35m
BLUE = \033[0;34m
RESET = \033[0m

define log_info
	@echo "$(shell date +"%d-%m-%Y %T") $(PURPLE)$(LOG) $(BLUE)INFO$(RESET)$(1)"
endef

# ---- RULES ---- #
.PHONY: all libft end clean fclean re bonus 

all: libft $(NAME) end

$(NAME): $(OBJS) 
	@$(call log_info, "Linking $(NAME)...")
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@$(call log_info, "Created $(NAME) executable")

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(call log_info, "Compiling $< to $@...")
	@$(CC) $(CFLAGS) -c $< -o $@

libft:
	@$(call log_info, "Calling libft Makefile...")
	@$(MAKE) -sC $(LIBFT_DIR)

end:
	@$(call log_info, "Everything is done!")

clean:
	@$(call log_info, "Calling libft Makefile...")
	@$(MAKE) -sC $(LIBFT_DIR) clean
	@$(call log_info, "Cleaning $(NAME) object files...")
	@rm -rf $(OBJ_DIR)

fclean:
	@$(call log_info, "Calling libft Makefile...")
	@$(MAKE) -sC $(LIBFT_DIR) fclean
	@$(call log_info, "Cleaning $(NAME) object files...")
	@rm -rf $(OBJ_DIR)
	@$(call log_info, "Cleaning $(NAME)...")
	@rm -f $(NAME)

re: fclean all
