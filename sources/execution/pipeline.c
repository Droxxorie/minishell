/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:52:10 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 19:49:33 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	exec_if_single_builtin(t_data *data)
{
	t_command	*node;
	int			saved[2];
	int			status;

	if (!data || !data->commands || data->commands->next)
		return (0);
	node = data->commands;
	if (!is_builtin_command(node))
		return (0);
	if (apply_redirections_in_parent(data, node, saved) == EXIT_FAILURE)
		return (-1);
	status = handle_builtin_command(data, node);
	restore_saved_stdio(saved);
	if (status == EXIT_FAILURE)
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
			pipes->pids[i] = launch_builtin_child(data, current, pipes->fds, 2
					* i);
		else if (is_builtin_command(current))
			handle_builtin_command(data, current);
		else if (handle_external_command(data, pipes->fds, 2 * i,
				&pipes->pids[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
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
