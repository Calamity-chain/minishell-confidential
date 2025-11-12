#include "../include/minishell.h"

void	ft_handle_sigint(int signo)
{
	(void)signo;
	g_signal_received = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
}

void	ft_handle_sigquit(int signo)
{
	(void)signo;
}

void	handle_sigint_child(int signo)
{
	(void)signo;
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(130);
}

void	handle_sigquit_child(int signo)
{
	(void)signo;
	ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	exit(131);
}
