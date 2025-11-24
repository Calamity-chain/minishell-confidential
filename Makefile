# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/19 15:12:00 by ltoscani          #+#    #+#              #
#    Updated: 2025/10/10 00:58:27 by asalniko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc -g
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

SRC_DIR = src
OBJ_DIR = obj
DIRS = $(sort $(dir $(OBJS)))
LIBFT_DIR = lib/libft

SRCS = \
	lexer/lexer \
	lexer/lexer_helpers1 \
	lexer/lexer_helpers2 \
	lexer/lexer_helpers3 \
	parser/parser_commands \
	parser/parser_helpers \
	parser/parser_pipeline \
	parser/parser_redirect \
	parser/parser_command_helper \
	parser/parser_command_helper2 \
	parser/parser_redirect_helper \
	parser/parser_command_helper3 \
	executor/executor \
    	executor/executor_helpers \
    	executor/redirections \
    	executor/redirection_list \
	executor/redirections_helpers \
	executor/redirections_helpers2 \
    	executor/pipes \
	executor/execute_pipelines \
	executor/executor_helpers2 \
	executor/executor_helpers3 \
	executor/executor_helpers4 \
	expansion/expansion \
	expansion/expansion_helper \
	expansion/expansion_helper2 \
	free \
	free2 \
	main \
	signals/signals \
	signals/signals_helpers \
	commands/pwd \
	commands/echo \
	commands/exit \
	commands/cd \
	commands/cd_helpers \
    	commands/export \
    	commands/unset \
    	commands/env \
    	commands/exit_helpers \
    	
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

.PHONY: all libft end clean fclean re bonus
