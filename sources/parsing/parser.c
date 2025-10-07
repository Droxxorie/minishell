/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 19:08:35 by eraad             #+#    #+#             */
/*   Updated: 2025/10/07 20:14:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int build_command_argv(t_data *data)
{
	t_command	*current_command;
	size_t			argc;
	size_t			i;

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

static int handle_redirection_token(t_data *data, t_token *token)
{
	if (token->type == HEREDOC)
	{
		if (!token->next || token->next->type != LIMITER)
			return (print_syntax_error(token->value[0], 7), EXIT_FAILURE);
		if (setup_heredoc(data, token->next->value) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (token->type == REDIR_IN)
	{
		if (handle_redirection_fd(data, &data->input, token, O_RDONLY))
			return (EXIT_FAILURE);
	}
	else if (token->type == REDIR_OUT)
	{
		if (handle_redirection_fd(data, &data->output, token, O_WRONLY | O_TRUNC | O_CREAT))
			return (EXIT_FAILURE);
	}
	else if (token->type == REDIR_APPEND)
	{
		if (handle_redirection_fd(data, &data->output, token, O_WRONLY | O_APPEND | O_CREAT))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	dispatch_command_tokens(t_data *data, t_token *current_token,
		int *command_boundary)
{
	static t_command	*current_command;

	if (*command_boundary)
	{
		current_command = append_command_node(data);
		*command_boundary = 0;
		if (!current_command)
			return (EXIT_FAILURE);
		if (set_command_name(current_command, current_token) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (current_token->value[0] == '-' && current_token->type == FLAG)
	{
		if (add_command_flag(current_command, current_token) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (current_token->type == ARG)
	{
		if (is_redirection_value(data, current_token) == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		if (add_command_arg(current_command, current_token) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	parser(t_data *data)
{
	t_token	*current_token;
	int		command_boundary;

	current_token = data->tokens;
	command_boundary = 1;
	while (current_token)
	{
		if (current_token->type == PIPE)
			command_boundary = 1;
		else if (current_token->type == CMD || current_token->type == ARG
			|| current_token->type == FLAG)
		{
			if (dispatch_command_tokens(data, current_token,
					&command_boundary) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else if (handle_redirection_token(data, current_token) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		current_token = current_token->next;
	}
	if (command_boundary && *data->line)
		return (perror("No command after pipe\n"), EXIT_FAILURE); //? maybe int FD OUT
	if (build_command_argv(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
