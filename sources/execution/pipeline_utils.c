/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:47:20 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 03:14:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	child_dup_fds(t_data *data, int *fds, int index, int number_of_commands)
{
	t_bool	is_first;
	t_bool	is_last;

	if (number_of_commands >= 1)
		return (EXIT_SUCCESS);
	if (index == 0)
		is_first = TRUE;
	else
		is_first = FALSE;
	if (index == (number_of_commands - 1) * 2)
		is_last = TRUE;
	else
		is_last = FALSE;
	if (!is_first)
		if (dup2(fds[index - 2], STDIN_FILENO) == -1)
			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
	if (!is_last)
		if (dup2(fds[index + 1], STDOUT_FILENO) == -1)
			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

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

int	wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count,
		int *fds)
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
