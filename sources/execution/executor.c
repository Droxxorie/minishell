/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:50:26 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 17:12:32 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	close_fds(int in_fd, int out_fd)
{
	if (in_fd != -1 && in_fd > 2)
		close(in_fd);
	if (out_fd != -1 && out_fd > 2)
		close(out_fd);
}

static int	exec_redirs_only_in_parent(t_data *data)
{
	t_command	*node;
	int			in_fd;
	int			out_fd;

	in_fd = -1;
	out_fd = -1;
	node = data->commands;
	if (!node)
		return (EXIT_SUCCESS);
	if (open_redirs_for_command(data, node, &in_fd, &out_fd) == -1)
	{
		close_fds(in_fd, out_fd);
		data->exit_status = 1;
		return (EXIT_FAILURE);
	}
	close_fds(in_fd, out_fd);
	data->exit_status = 0;
	return (EXIT_SUCCESS);
}

int	executor(t_data *data)
{
	int	number_of_commands;

	number_of_commands = count_commands(data->commands);
	if (number_of_commands < 0)
		return (EXIT_FAILURE);
	if (number_of_commands == 1 && data->commands
		&& data->commands->command == NULL && data->commands->redirs != NULL)
		return (exec_redirs_only_in_parent(data));
	if (number_of_commands == 0)
	{
		if (data->saw_empty_word_as_command)
		{
			report_error2("", "command not found");
			data->exit_status = 127;
		}
		return (EXIT_SUCCESS);
	}
	if (prepare_pipes(data, number_of_commands) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (launch_pipeline(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free_pipes_all(data);
	return (EXIT_SUCCESS);
}
