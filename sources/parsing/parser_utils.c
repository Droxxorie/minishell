/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:37:32 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:40:27 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	dispatch_command_tokens(t_data *data, t_command **current, t_token *token,
		int *command_boundary)
{
	t_command	*cmd;

	cmd = *current;
	if (*command_boundary)
	{
		if (!cmd)
		{
			cmd = append_command_node(data);
			if (!cmd)
				return (EXIT_FAILURE);
			*current = cmd;
		}
		*command_boundary = 0;
	}
	if (cmd->command == NULL && (token->type == CMD || token->type == ARG))
		return (set_command_name(cmd, token));
	if (token->type == FLAG && token->value && token->value[0] == '-')
		return (add_command_flag(cmd, token));
	if (token->type == ARG)
		return (add_command_arg(cmd, token));
	return (EXIT_SUCCESS);
}

int	handle_redirection_token(t_data *data, t_command **current, t_token *token)
{
	if (!*current)
	{
		*current = append_command_node(data);
		if (!*current)
			return (EXIT_FAILURE);
	}
	return (push_redir(data, *current, token));
}

int	build_command_argv(t_data *data)
{
	t_command	*current_command;
	size_t		argc;
	size_t		i;

	current_command = data->commands;
	while (current_command)
	{
		argc = count_command_elements(current_command);
		current_command->argv = malloc(sizeof(char *) * (argc + 1));
		if (!current_command->argv)
		{
			perror("minishell: malloc");
			return (EXIT_FAILURE);
		}
		i = 0;
		if (append_command_to_argv(current_command, &i) == EXIT_FAILURE
			|| append_args_to_argv(current_command, &i) == EXIT_FAILURE
			|| append_flags_to_argv(current_command, &i) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		current_command->argv[i] = NULL;
		current_command = current_command->next;
	}
	return (EXIT_SUCCESS);
}
