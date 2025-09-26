/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 01:20:53 by eraad             #+#    #+#             */
/*   Updated: 2025/09/26 01:49:02 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redirection_value(t_data *data, t_token *current_token)
{
	if (!data ||!current_token || !current_token->value)
		return (EXIT_FAILURE);
	if (data->input.value && ft_strcmp(data->input.value, current_token->value) == 0)
		return (EXIT_SUCCESS);
	if (data->output.value && ft_strcmp(data->output.value, current_token->value) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
	
}

static void	link_command_node(t_data *data, t_command *new_command)
{
	t_command *commands;

	commands = data->commands;
	while (commands->next)
		commands = commands->next;
	commands->next = new_command;
}

t_command	*append_command_node(t_data *data)
{
	t_command	*new_command;

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
