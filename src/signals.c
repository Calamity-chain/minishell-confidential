/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:13:44 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:13:47 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
 * handle_sigint (Ctrl-C) - for interactive prompt
 */
static void	ft_handle_sigint(int signo)
{
	(void)signo;
	g_signal_received = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	// Don't call rl_replace_line or rl_redisplay here
	// Let the main loop handle the cleanup
}

/*
 * handle_sigquit (Ctrl-\) - for interactive prompt
 */
static void	ft_handle_sigquit(int signo)
{
	(void)signo;
}

/*
 * handle_sigint_child - for child processes
 */
static void	handle_sigint_child(int signo)
{
	(void)signo;
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(130);
}

/*
 * handle_sigquit_child - for child processes
 */
static void	handle_sigquit_child(int signo)
{
	(void)signo;
	ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	exit(131);
}

/*
 * ft_init_signal_handlers - for interactive prompt
 */
void	ft_init_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = ft_handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		perror("Error setting SIGINT handler");
	sa_quit.sa_handler = ft_handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		perror("Error setting SIGQUIT handler");
}

/*
 * setup_child_signals - for child processes during command execution
 */
void	setup_child_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_child;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit_child;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
