/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:37:23 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 14:53:57 by eraad            ###   ########.fr       */
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

int executor(t_data *data)
{
	int	number_of_commands;

	number_of_commands = count_commands(data->commands);
	if (number_of_commands <= 0)
		return (EXIT_FAILURE);
	if (prepare_pipes(data, number_of_commands) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (launch_pipeline(data) == EXIT_FAILURE)
	{
		free_pipes_all(data);
		return (EXIT_FAILURE);
	}
	free_pipes_all(data);
	return (EXIT_SUCCESS);
}
