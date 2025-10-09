/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:52:23 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:29:34 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	sigint_handler_child(int signum)
{
	(void)signum;
	write(2, "\n", 1);
	g_waiting = 3;
	rl_done = 1;
	return ;
}

static void	sigquit_handler_child(int signum)
{
	(void)signum;
	write(2, "Quit (core dumped)\n", 20);
	g_waiting = 4;
	rl_done = 1;
	return ;
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	rl_event_hook = sig_event;
	sa.sa_handler = sigint_handler_child;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, sigquit_handler_child);
	signal(SIGTSTP, SIG_IGN);
}
