/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:53:10 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:53:29 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_last_status(t_data *data, int status)
{
	int	sig;

	if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
		return ;
	}
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		data->exit_status = 128 + sig;
	}
}

int	wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count, int *fds)
{
	int		i;
	int		status;
	int		nb_fds;
	pid_t	wait;

	if (!data || !pids || count <= 0)
		return (EXIT_FAILURE);
	nb_fds = 2 * (count - 1);
	if (nb_fds > 0 && fds)
		close_pipe_fds(fds, nb_fds);
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			wait = waitpid(pids[i], &status, 0);
			while (wait == -1 && errno == EINTR)
				wait = waitpid(pids[i], &status, 0);
			if (wait > 0 && i == count - 1)
				update_last_status(data, status);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
