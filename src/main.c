/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:45:37 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/10 01:21:37 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_shell(void) // Renamed and made static as it's only used here
{
	// Any shell initialization can go here
	// For example, loading history, setting up environment variables etc.
	// For now, it's empty.
	return ;
}

/* Recent modifs >> */
// Add proper cleanup 
/**
 * @brief Prints all tokens for debugging purposes.
 * @param tokens (t_list *) The list of tokens to print.
 */
static void	ft_print_tokens(t_list *tokens)
{
	t_list		*current;
	t_token		*token;
	int			i;
	const char	*type_names[] = {
		"WORD", "STRING_LITERAL", "ENV_VAR", "PIPE", "REDIRECT_IN", "REDIRECT_OUT", "HEREDOC", "APPEND_OUT", 
		"LPARENTHESIS", "RPARENTHESIS", "END_OF_FILE", "SPACE", "ERROR"
	};

	current = tokens;
	i = 0;
	while (current)
	{
		token = (t_token *)current->content;
		printf("Token %d: Type=%s, Value='%.*s'\n", 
			   i++, type_names[token->type], (int)token->len, token->value);
		current = current->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;

	ft_init_shell();
	ft_init_signal_handlers();

	while (1)
	{
		if (g_signal_received == SIGINT)
		{
			g_signal_received = 0;
		}

		line = readline(PROMPT);

		if (g_signal_received == SIGINT)
		{
			if (line) 
				free(line);
			continue;
		}

		if (!line)
		{
			printf("exit\n");
			break;
		}

		if (*line)
		{
			add_history(line);
			
			// Test builtins
			if (ft_strncmp(line, "pwd", 3) == 0 && (line[3] == '\0' || ft_isspace(line[3])))
				ft_pwd();
			else if (ft_strncmp(line, "echo", 4) == 0 && (line[4] == '\0' || ft_isspace(line[4])))
			{
				char *dummy_args[] = {"echo", "hello", "world", NULL};
				ft_echo(dummy_args);
			}
			else if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || ft_isspace(line[4])))
			{
				char *dummy_args[] = {"exit", "42", NULL};
				ft_exit(dummy_args);
			}
			else
			{
				// Tokenize and display tokens
				t_list *tokens = ft_tokenize(line);
				if (tokens)
				{
					printf("Tokens:\n");
					ft_print_tokens(tokens);
					ft_lstclear(&tokens, ft_free_token);
				}
			}
		}
		free(line);
	}

	return (0);
}
