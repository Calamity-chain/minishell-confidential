/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:52:47 by rmei              #+#    #+#             */
/*   Updated: 2025/06/01 12:20:04 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

/*
 * handle_sigint (Ctrl-C)
 * 1. Sets a global flag `g_signal_received` to indicate SIGINT was received.
 * 2. Prints a newline character.
 * 3. Replaces the current readline line buffer with an empty string.
 * 4. Redisplays the prompt on a new line.
 */
static void	ft_handle_sigint(int signo)
{
	g_signal_received = signo; // Store signo (SIGINT)
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line(); // Notify readline that we've moved to a new line
	rl_redisplay(); // Redisplay the prompt and cleared buffer
}

/*
 * handle_sigquit (Ctrl-\)
 * This handler does nothing, as per the project requirements.
 * The `(void)signo;` is to prevent unused parameter warnings.
 */
static void	ft_handle_sigquit(int signo)
{
	(void)signo; // Do nothing for the interactive prompt
	// When a command is running, this behavior will be different (parent ignores, child default).
	// For now, we want it to do absolutely nothing at the prompt.
	// rl_on_new_line(); // No need to call if doing nothing
	// rl_redisplay();   // No need to call if doing nothing
}

/*
 * setup_signal_handlers
 * Configures the signal handlers for SIGINT and SIGQUIT.
 * SIG_IGN makes the process ignore the signal.
 * SIG_DFL restores the default signal handler.
 */
void	ft_init_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = ft_handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART; // SA_RESTART to avoid interrupting syscalls like read
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		perror("Error setting SIGINT handler");

	sa_quit.sa_handler = ft_handle_sigquit; // Initially do nothing, or custom handler
//	sa_quit.sa_handler = SIG_IGN; // Alternative: explicitly ignore SIGQUIT
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		perror("Error setting SIGQUIT handler");
} 
