/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:08:30 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 19:30:56 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			handle_builtin_execution(data, pipes->fds, i * 2, current_command);
		else
			execute_env(data, pipes->fds, i * 2, &(pipes->pids[i])); // TODO
		i++;
		current_command = current_command->next;
	}
	finalize_child_processes(data, pipes->pids, i, pipes->fds); // TODO
	return (EXIT_SUCCESS);
}
