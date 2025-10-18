/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:50:26 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 12:28:56 by eraad            ###   ########.fr       */
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

int	executor(t_data *data)
{
	int	number_of_commands;

	number_of_commands = count_commands(data->commands);
	if (number_of_commands < 0)
		return (EXIT_FAILURE);
	if (number_of_commands == 0)
	{
		if (data->saw_empty_word_as_command)
		{
			report_error2("", "command not found");
			data->exit_status = 127;
		}
		else
			data->exit_status = 0;
		return (EXIT_SUCCESS);
	}
	if (prepare_pipes(data, number_of_commands) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (launch_pipeline(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free_pipes_all(data);
	return (EXIT_SUCCESS);
}
