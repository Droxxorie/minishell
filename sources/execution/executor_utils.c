/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:26:51 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 03:07:16 by eraad            ###   ########.fr       */
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

static int	pipes_alloc_arrays(t_data *data, int number_of_commands)
{
	data->pipes->pids = (pid_t *)ft_calloc(number_of_commands, sizeof(pid_t));
	if (!data->pipes->pids)
	{
		report_error(data, "calloc", -1);
		free(data->pipes);
		data->pipes = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	pipes_alloc_struct(t_data *data)
{
	data->pipes = (t_pipes *)ft_calloc(1, sizeof(t_pipes));
	if (!data->pipes)
	{
		report_error(data, "calloc", -1);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	prepare_pipes(t_data *data, int number_of_commands)
{
	if (pipes_alloc_struct(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pipes_alloc_arrays(data, number_of_commands) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->pipes->nb = number_of_commands - 1;
	if (number_of_commands > 1)
	{
		data->pipes->fds = setup_pipes(data, number_of_commands);
		if (!data->pipes->fds)
		{
			free_pipes_all(data);
			return (EXIT_FAILURE);
		}
	}
	else
		data->pipes->fds = NULL;
	return (EXIT_SUCCESS);
}
