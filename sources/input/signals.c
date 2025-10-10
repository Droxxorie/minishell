/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:50:41 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 15:15:33 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_waiting = 0;

static void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_waiting = 3;
	rl_done = 1;
	return ;
}

static void	sigint_handler(int signum)
{
	(void)signum;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_waiting = 1;
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	rl_event_hook = sig_event;
	sa.sa_handler = heredoc_sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

int	sig_event(void)
{
	return (EXIT_SUCCESS);
}

void	signals_handler(void)
{
	g_waiting = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
