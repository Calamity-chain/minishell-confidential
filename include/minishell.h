/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:52:56 by rmei              #+#    #+#             */
/*   Updated: 2025/06/01 20:49:40 by rmei             ###   ########.fr       */
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
# include "lexer.h"

extern volatile sig_atomic_t    g_signal_received;

// Function Prototypes :  
//Built-ins commands
int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_exit(char **args);
 

// Signals
void                            ft_init_signal_handlers(void);

// Tokenizer
t_list                          *ft_tokenize(const char *input_line);
// void ft_free_tokens(t_list **tokens_head); // We'll add this later

#endif 
