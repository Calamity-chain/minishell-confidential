/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 16:59:23 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/15 16:59:25 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

char	*remove_outer_quotes(char *str)
{
	char	*result;
	size_t	len;
	char	first_char;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	first_char = str[0];
	if ((first_char == '\'' || first_char == '"') && str[len - 1] == first_char)
		result = ft_substr(str, 1, len - 2);
	else
		result = ft_strdup(str);
	return (result);
}

void	shift_arguments_left(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
		return ;
	if (!cmd->args[1])
	{
		free(cmd->args[0]);
		cmd->args[0] = NULL;
		return ;
	}
	free(cmd->args[0]);
	i = 1;
	while (cmd->args[i])
	{
		cmd->args[i - 1] = cmd->args[i];
		i++;
	}
	cmd->args[i - 1] = NULL;
}

int	expand_command_name(t_command *cmd, t_data *data)
{
	char	*expanded;

	if (!cmd || !cmd->args || !cmd->args[0] || !data)
		return (0);
	if (cmd->args[0][0] == '$')
		cmd->from_env_var = 1;
	expanded = expand_env_variable_in_string(data, cmd->args[0]);
	if (expanded)
	{
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}
	if (cmd->args[0][0] == '~'
		&& (cmd->args[0][1] == '/' || cmd->args[0][1] == '\0'))
	{
		expanded = expand_tilda(data, cmd->args[0]);
		if (expanded)
		{
			free(cmd->args[0]);
			cmd->args[0] = expanded;
		}
	}
	return (cmd->args[0][0] == '\0');
}
