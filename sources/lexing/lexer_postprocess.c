/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:57:36 by eraad             #+#    #+#             */
/*   Updated: 2025/09/24 20:46:42 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_pipe_syntax(t_data *data)
{
	t_token	*current_token;

	if (!data->tokens)
		return (EXIT_FAILURE);
	current_token = data->tokens;
	while (current_token)
	{
		if (current_token->type == PIPE)
		{
			if (!current_token->next || current_token->next->type == PIPE
				|| current_token->next->type == REDIR_IN
				|| current_token->next->type == REDIR_OUT
				|| current_token->next->type == REDIR_APPEND
				|| current_token->next->type == HEREDOC)
			{
				data->exit_status = 2;
				free_tokens(data);
				print_syntax_error('|', 4);
				return (EXIT_FAILURE);
			}
		}
		current_token = current_token->next;
	}
	return (EXIT_SUCCESS);
}

t_token	*normalize_redirection_args(t_token *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == REDIR_IN || current_token->type == REDIR_OUT
			|| current_token->type == REDIR_APPEND)
		{
			if (current_token->next && current_token->next->type == ARG)
				current_token->next->type = FILE_NAME;
		}
		current_token = current_token->next;
	}
	return (tokens);
}

t_token	*normalize_exit_echo_args(t_token *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == CMD)
		{
			if (ft_strcmp(current_token->value, "exit") == 0
				|| ft_strcmp(current_token->value, "echo") == 0)
			{
				while (current_token->next && (current_token->next->type == ARG
						|| current_token->next->type == FLAG))
				{
					if (current_token->next
						&& current_token->next->type == FLAG)
						current_token->next->type = ARG;
					current_token = current_token->next;
				}
			}
		}
		current_token = current_token->next;
	}
	return (tokens);
}

t_token	*classify_heredoc_delimiters(t_token *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == HEREDOC && current_token->next)
		{
			if (current_token->next->type != ARG
				&& current_token->next->type != CMD)
				return (print_syntax_error('<', 5), NULL);
			current_token->next->type = LIMITER;
			if (current_token->next->next
				&& current_token->next->next->type == ARG
				&& !current_token->prev)
				current_token->next->next->type = CMD;
		}
		current_token = current_token->next;
	}
	return (tokens);
}

t_token	*classify_input_redirections(t_token *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == REDIR_IN && current_token->next)
		{
			if (current_token->next->type != ARG
				&& current_token->next->type != CMD
				&& current_token->next->type != FILE_NAME)
				return (print_syntax_error('<', 4), NULL);
			current_token->next->type = FILE_NAME;
			if (current_token->next->next && !current_token->prev)
				current_token->next->next->type = CMD;
		}
		current_token = current_token->next;
	}
	return (tokens);
}
