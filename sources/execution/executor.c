/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:37:23 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:32:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	*setup_pipes(t_data *data, int number_of_commands)
{
	int	i;
	int	*fds;
	int	pipes_needed;

	if (number_of_commands <= 1)
		return (NULL);
	pipes_needed = number_of_commands - 1;
	fds = (int *)ft_calloc(2 * pipes_needed, sizeof(int));
	if (!fds)
		return (report_error(data, "calloc", -1), NULL);
	i = 0;
	while (fds && i < pipes_needed)
	{
		if (pipe(&fds[2 * i]) == -1)
		{
			while (--i >= 0)
			{
				close(fds[2 * i]);
				close(fds[2 * i + 1]);
			}
			return (free(fds), report_error(data, "pipe", -1), NULL);
		}
		i++;
	}
	return (fds);
}

static int	count_commands(t_command *commands)
{
	int			count;
	t_command	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	executor(t_data *data)
{
	int	number_of_commands;

	number_of_commands = count_commands(data->commands);
	if (number_of_commands <= 0)
		return (EXIT_FAILURE);
	data->pipes = (t_pipes *)ft_calloc(1, sizeof(t_pipes));
	if (!data->pipes)
		return (report_error(data, "calloc", -1), EXIT_FAILURE);
	data->pipes->pids = (pid_t *)ft_calloc(number_of_commands, sizeof(pid_t));
	if (!data->pipes->pids)
		return (free(data->pipes), data->pipes = NULL, report_error(data,
				"calloc", -1), EXIT_FAILURE);
	data->pipes->nb = number_of_commands - 1;
	data->pipes->fds = setup_pipes(data, number_of_commands);
	if (!data->pipes->fds && number_of_commands > 0)
		return (free(data->pipes), data->pipes = NULL, EXIT_FAILURE);
	setup_child_signals();
	launch_pipeline(data);
	free(data->pipes->fds);
	// data->pipes->fds = NULL; Pour evité double free on verra
	free(data->pipes->pids);
	// data->pipes->pids = NULL;
	return (EXIT_SUCCESS);
}
