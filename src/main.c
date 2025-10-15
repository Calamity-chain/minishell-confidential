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

volatile sig_atomic_t g_signal_received = 0;  // ADDED THIS LINE

static char	**copy_envp(char **envp)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_matrix(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

static void	ft_init_shell(t_data *data, char **envp)
{
	data->env = copy_envp(envp);
	data->exit_status = 0;
}

static void	process_input(char *line, t_data *data)
{
	t_list		*tokens;
	t_command	*pipeline;

	if (!line || !*line)
		return ;
	add_history(line);
	tokens = ft_tokenize(line);
	if (!tokens)
		return ;
	pipeline = parse_pipeline(tokens->content);
	if (pipeline)
	{
		execute_pipeline(pipeline, data);
		free_pipeline(pipeline);
	}
	ft_lstclear(&tokens, ft_free_token);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	data;

	(void)argc;
	(void)argv;
	ft_init_shell(&data, envp);
	ft_init_signal_handlers();
	
	while (1)
	{
		g_signal_received = 0;  // Reset at start of loop
		line = readline(PROMPT);
		
		if (g_signal_received == SIGINT)
		{
			// Signal interrupted readline - cleanup and continue
			data.exit_status = 130;  // Signal exit status
			if (line)
				free(line);
			continue;
		}
		
		if (!line)
		{
			printf("exit\n");
			break;
		}
		
		process_input(line, &data);
		free(line);
	}
	ft_free_matrix(data.env);
	return (data.exit_status);
}
