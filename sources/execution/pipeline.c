/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:08:30 by eraad             #+#    #+#             */
/*   Updated: 2025/10/03 20:43:34 by eraad            ###   ########.fr       */
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

static int	wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count, int *fds)
{
	int		i;
	int		status;
	int		nb_fds;
	pid_t	wait;

	if (!data || !pids || count <= 0 || !fds)
		return (EXIT_FAILURE);
	nb_fds = 2 * (count - 1);
	if (nb_fds > 0)
		close_pipe_fds(fds, nb_fds);
	i = 0;
	while (i < count)
	{
		wait = waitpid(pids[i], &status, 0);
		while (wait == -1 && errno == EINTR)
			wait = waitpid(pids[i], &status, 0);
		if (wait > 0 && i == count - 1)
			update_last_status(data, status);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	launch_pipeline(t_data *data)
{
	int			i;
	t_pipes		*pipes;
	t_command	*current_command;

	if (!data->commands || !data->pipes)
		return (EXIT_FAILURE);
	i = 0;
	pipes = data->pipes;
	current_command = data->commands;
	while (current_command)
	{
		if (g_waiting == 3)
			break ;
		if (is_builtin_command(current_command) == TRUE)
			handle_builtin_command(data, pipes->fds, i * 2, current_command);
		else
			handle_external_command(data, pipes->fds, i * 2, &(pipes->pids[i]));
		i++;
		current_command = current_command->next;
	}
	if (wait_and_cleanup_pipeline(data, pipes->pids, i, pipes->fds) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
