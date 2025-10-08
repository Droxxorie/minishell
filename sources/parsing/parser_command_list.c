/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:38:39 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:38:54 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	link_command_node(t_data *data, t_command *new_command)
{
	t_command	*commands;

	commands = data->commands;
	while (commands->next)
		commands = commands->next;
	commands->next = new_command;
}

t_command	*append_command_node(t_data *data)
{
	t_command *new_command;

	if (!data->commands)
	{
		data->commands = malloc(sizeof(t_command));
		if (!data->commands)
		{
			perror("minishell: malloc");
			return (NULL);
		}
		ft_memset(data->commands, 0, sizeof(t_command));
		return (data->commands);
	}
	else
	{
		new_command = malloc(sizeof(t_command));
		if (!new_command)
		{
			perror("minishell: malloc");
			return (NULL);
		}
		ft_memset(new_command, 0, sizeof(t_command));
		link_command_node(data, new_command);
		return (new_command);
	}
}
