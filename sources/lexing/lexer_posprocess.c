/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_posprocess.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:33 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 17:25:31 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*classify_input_redirections(t_data *data, t_token *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == REDIR_IN)
		{
			if (current_token->next == NULL)
				return (report_error(data,
						"syntax error near unexpected token `newline'",
						2), NULL);
			else if (current_token->next->type == PIPE
				&& current_token->next->type == REDIR_IN
				&& current_token->next->type == REDIR_OUT
				&& current_token->next->type == REDIR_APPEND
				&& current_token->next->type == HEREDOC)
				return (print_syntax_error('<', 4), NULL);
			if (current_token->next)
				current_token->next->type = FILE_NAME;
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
			current_token->next->quote = current_token->quote;
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

int	validate_pipe_syntax(t_data *data)
{
	t_token	*cur;

	if (!data->tokens)
		return (EXIT_FAILURE);
	cur = data->tokens;
	if (cur && cur->type == PIPE)
	{
		data->exit_status = 2;
		print_syntax_error('|', 4);
		return (EXIT_FAILURE);
	}
	while (cur)
	{
		if (cur->type == PIPE)
		{
			if (!cur->next || cur->next->type == PIPE)
			{
				data->exit_status = 2;
				print_syntax_error('|', 4);
				return (EXIT_FAILURE);
			}
		}
		cur = cur->next;
	}
	return (EXIT_SUCCESS);
}
