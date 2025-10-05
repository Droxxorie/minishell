/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_command_context.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:03:56 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 12:04:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	reset_commands(t_data *data)
{
	t_command	*temp;

	while (data->commands)
	{
		temp = data->commands;
		data->commands = data->commands->next;
		if (temp->command)
		{
			free(temp->command);
			temp->command = NULL;
		}
		if (temp->flags)
			minilist_clear(&temp->flags);
		if (temp->args)
			minilist_clear(&temp->args);
		if (temp->argv)
			free_char_array(temp->argv);
		free(temp);
		temp = NULL;
	}
}

static void reset_tokens(t_data *data)
{
	t_token	*temp;
	
	while (data->tokens)
	{
		temp = data->tokens;
		data->tokens = data->tokens->next;
		if (temp->value)
		{
			free(temp->value);
			temp->value = NULL;
		}
		if (temp)
			free(temp);
		temp = NULL;
	}
}

static void reset_redirections(t_data *data)
{
	data->input.type = REDIR_IN;
	if (data->input.value)
	{
		free(data->input.value);
		data->input.value = NULL;
	}
	data->input.fd = 0;
	data->output.type = REDIR_OUT;
	if (data->output.value)
	{
		free(data->output.value);
		data->output.value = NULL;
	}
	data->output.fd = 1;
}

void	reset_command_context(t_data *data)
{
	reset_commands(data);
	reset_tokens(data);
	reset_redirections(data);
}
