/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:52:10 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 22:57:40 by eraad            ###   ########.fr       */
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

static int	collect_commands(t_command *cur, t_command **arr, int max, int *out)
{
	int	n;

	n = 0;
	while (cur && n < max)
	{
		arr[n] = cur;
		cur = cur->next;
		n++;
	}
	*out = n;
	return (EXIT_SUCCESS);
}

static int	dispatch_one(t_data *data, t_pipes *p, t_command *cmd, int i)
{
	if (g_waiting == 3)
		return (2);
	if (is_builtin_command(cmd) && p->nb + 1 > 1)
	{
		p->pids[i] = launch_builtin_child(data, cmd, p->fds, 2 * i);
		if (p->pids[i] < 0)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (is_builtin_command(cmd))
	{
		if (handle_builtin_command(data, cmd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (handle_external_command(data, p->fds, 2 * i,
			&p->pids[i]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	pipeline_loop(t_data *data, t_pipes *pipes, t_command *current, int *count)
{
	t_command	*arr[256];
	int			n;
	int			i;
	int			ret;

	n = 0;
	i = 0;
	ret = collect_commands(current, arr, 256, &n);
	if (ret == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = n - 1;
	while (i >= 0)
	{
		ret = dispatch_one(data, pipes, arr[i], i);
		if (ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (ret == 2)
			break ;
		i--;
	}
	*count = n;
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
