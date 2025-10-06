/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:08:30 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 03:10:40 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static pid_t	launch_builtin_child(t_data *data, t_command *node, int *fds, int index)
{
	pid_t	pid;
	int		number_of_commands;

	number_of_commands = data->pipes->nb + 1;
	pid = fork();
	if (pid == -1)
		return (report_error(data, "fork", -1), (pid_t)-1);
	if (pid == 0)
	{
		setup_child_signals();
		if (child_dup_fds(data, fds, index, number_of_commands) == EXIT_FAILURE)
			exit_minishell(data, 1);
		if (number_of_commands > 1)
			close_pipe_fds(fds, (number_of_commands - 1) * 2);
		if (dispatch_builtin(data, node) == EXIT_FAILURE)
			exit_minishell(data, 1);
		exit_minishell(data, data->exit_status);
	}
	parent_close_after_fork(fds, index, number_of_commands);
	return (pid);
}

static int	exec_if_single_builtin(t_data *data)
{
	t_command	*node;
	
	if (!data || !data->commands || data->commands->next)
		return (0);
	node = data->commands;
	if (!is_builtin_command(node))
		return (0);
	if (handle_builtin_command(data, NULL, 0, node) == EXIT_FAILURE)
		return (-1);
	return (1);
}

static int	pipeline_loop(t_data *data, t_pipes *pipes, t_command *current,
		int *count)
{
	int	i;

	i = 0;
	while (current)
	{
		if (g_waiting == 3)
			break ;
		if (is_builtin_command(current) && pipes->nb + 1 > 1)
			pipes->pids[i] = launch_builtin_child(data, current, pipes->fds, 2 * i);
		else if (is_builtin_command(current))
			handle_builtin_command(data, pipes->fds, 2 * i, current);
		else
			handle_external_command(data, pipes->fds, 2 * i, &pipes->pids[i]);
		i++;
		current = current->next;
	}
	*count = i;
	return (EXIT_SUCCESS);
}

int	launch_pipeline(t_data *data)
{
	int			status;
	int			count;
	t_command	*current;
	t_pipes		*pipes;

	if (!data || !data->commands || !data->pipes)
		return (EXIT_FAILURE);
	current = data->commands;
	status = exec_if_single_builtin(data);
	if (status == 1)
		return (EXIT_SUCCESS);
	else if (status == -1)
		return (EXIT_FAILURE);
	pipes = data->pipes;
	if (pipeline_loop(data, pipes, current, &count) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (wait_and_cleanup_pipeline(data, pipes->pids, count,
			pipes->fds) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
