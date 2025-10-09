/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:38:51 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/10 00:40:39 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Project-specific defines
# define PROMPT "minishell> "

// Standard Libraries
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, exit, getenv
# include <unistd.h>     // access, fork, pipe, execve, dup, dup2, close, getcwd, chdir, unlink
# include <string.h>     // strerror
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <sys/stat.h>   // stat, lstat, fstat
# include <fcntl.h>      // open, read, write
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>    // opendir, readdir, closedir
# include <termios.h>    // tcsetattr, tcgetattr
# include <term.h>      // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

// Readline
# include <readline/readline.h>
# include <readline/history.h>

// Project-specific includes
# include "../lib/libft/include/libft.h"
# include "../include/lexer.h"
# include "../include/parser.h"

extern volatile sig_atomic_t    g_signal_received;

typedef struct s_data
{
    char	**env;         // current environment (NULL-terminated)
    int		exit_status;  // last exit code
    // maybe we need to add other necessary fields (token lists, history, pipeline, etc.)
}	t_data;

// env expansion
char	*handle_quoted_env(t_data *data, t_token *token, char **quoted_string);
void	handle_env(t_data *data, t_command *command, int *ac, t_token *token);
char	*expand_tilda(t_data *data, const char *env_name);
char	*expand_env_variable(t_data *data, const char *env_name);
char	*ft_getenv(t_data *data, const char *env_name);

// Function Prototypes :  
//Built-ins commands
int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_exit(char **args);
 
// Signals
void                            ft_init_signal_handlers(void);

// free //
void	free_pipeline(t_command *head);
void	ft_free_token(void *token_ptr);
void	ft_free_matrix(char **matrix);
void	free_command(t_command *cmd);

#endif 
