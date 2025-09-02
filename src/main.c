/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:52:41 by rmei              #+#    #+#             */
/*   Updated: 2025/06/01 20:50:11 by rmei             ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp; // We will use envp later

	ft_init_shell();
	ft_init_signal_handlers(); // Call the renamed signal setup function
	// rl_catch_signals = 0; // Set this to 0 if you want to handle signals entirely manually
							 // Readline's default SIGINT handler might conflict or be redundant

	while (1)
	{
		// Reset signal flag before reading new input
		if (g_signal_received == SIGINT)
		{
			// Potentially update exit status $? here if needed
			g_signal_received = 0; // Reset the flag
		}

		line = readline(PROMPT);

		// Check if readline was interrupted by our SIGINT handler
		// and g_signal_received was set. If so, readline might return NULL
		// or the line it was processing. We already handled the display
		// in the signal handler itself.
		if (g_signal_received == SIGINT) // Check flag again after readline
		{
			if (line) free(line); // Free if readline returned something despite interrupt
			continue; // Loop back to get fresh prompt and input
		}

		if (!line) // Ctrl-D was pressed (EOF)
		{
			printf("exit\n"); // Mimic bash behavior
			break;
		}

		if (*line) // If the line is not empty
		{
			add_history(line);
			// Here we will parse and execute the command
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
				/*
				 = {"exit", NULL}; // — should exit with 0.
				 = {"exit", "abc", NULL}; // — error, exits with 255.
				 = {"exit", "1", "2", NULL};// — too many args, doesn't exit.
				 */
				ft_exit(dummy_args);
			}


			// For now, let's just print it back
			printf("You entered: %s\n", line);

			t_list *tokens = ft_tokenize(line);
			if (tokens)
			{
				printf("Tokens generated (actual processing TBD).\n");
				// ft_print_tokens(tokens); // A helper to print for debugging, TBD
				// ft_free_tokens(&tokens); // A helper to free the token list, TBD
				// For now, manually clear if using the dummy EOF token logic in ft_tokenize
				// and if ft_lstclear is available and correctly frees token content.
				// This requires a proper ft_lstclear that can take a content-freeing function.
				// Example for a simple ft_lstclear(list, free_content_func):
				// ft_lstclear(&tokens, free_token_content); // where free_token_content frees t_token and its value
 				// If your ft_lstclear just frees nodes and you need to free content first:
 				/*
 				t_list *current = tokens;
 				t_list *next;
 				while(current)
 				{
 					next = current->next;
 					t_token *token_data = (t_token *)current->content;
 					if (token_data)
 					{
 						free(token_data->value);
 						free(token_data);
 					}
 					free(current);
 					current = next;
 				}
 				tokens = NULL;
 				*/
				// Assuming a proper ft_lstclear for now:
				// If ft_tokenize only adds an EOF, and ft_add_token_to_list frees on failure,
				// then the list clear should be robust. A specific token free function is better.
				// For now, to avoid leaks with the current dummy ft_tokenize:
				ft_lstclear(&tokens, ft_free_token); // We will create ft_free_token_void
			}
		}
		free(line); // Free the memory allocated by readline
	}

	// rl_clear_history(); // Optional: clear history on exit
	return (0);
} 
