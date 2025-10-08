/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:36:14 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 18:13:47 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int	dispatch_command_tokens(t_data *data, t_command **current,
// 		t_token *token, int *command_boundary)
// {
// 	t_command	*command;

// 	command = *current;
// 	if (*command_boundary)
// 	{
// 		command = append_command_node(data);
// 		if (!command)
// 			return (EXIT_FAILURE);
// 		*current = command;
// 		*command_boundary = 0;
// 		if (set_command_name(command, token) == EXIT_FAILURE)
// 			return (EXIT_FAILURE);
// 		return (EXIT_SUCCESS);
// 	}
// 	if (token->type == FLAG && token->value && token->value[0] == '-')
// 		return (add_command_flag(command, token));
// 	if (token->type == ARG)
// 	{
// 		// if (is_redir_value_for_command(command, token))
// 		// 	return (EXIT_SUCCESS);
// 		return (add_command_arg(command, token));
// 	}
// 	return (EXIT_SUCCESS);
// }

static int	dispatch_command_tokens(t_data *data, t_command **current,
		t_token *token, int *command_boundary)
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

static int	handle_redirection_token(t_data *data, t_command **current,
		t_token *token)
{
	if (!*current)
	{
		*current = append_command_node(data);
		if (!*current)
			return (EXIT_FAILURE);
	}
	return (push_redir(data, *current, token));
}

static int	build_command_argv(t_data *data)
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

int	parser(t_data *data)
{
	t_token		*token;
	int			command_boundary;
	t_command	*current_command;
	t_command	*before_command;

	token = data->tokens;
	command_boundary = 1;
	current_command = NULL;
	while (token)
	{
		if (token->type == PIPE)
		{
			command_boundary = 1;
			current_command = NULL;
		}
		else if (token->type == CMD || token->type == ARG
			|| token->type == FLAG)
		{
			if (dispatch_command_tokens(data, &current_command, token,
					&command_boundary) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else if (token->type == REDIR_IN || token->type == REDIR_OUT
			|| token->type == REDIR_APPEND || token->type == HEREDOC)
		{
			before_command = current_command;
			if (handle_redirection_token(data, &current_command,
					token) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (before_command == NULL && current_command != NULL)
				command_boundary = 0;
		}
		token = token->next;
	}
	if (command_boundary && *data->line)
		return (report_error(data, "No command after pipe", -1), EXIT_FAILURE);
	if (build_command_argv(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
