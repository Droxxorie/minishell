/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_postprocess_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 13:21:55 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 18:18:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	report_error_token_type(t_data *data, t_type type)
{
	char	*str;

	if (type == PIPE)
		str = "|";
	else if (type == REDIR_IN)
		str = "<";
	else if (type == REDIR_OUT)
		str = ">";
	else if (type == REDIR_APPEND)
		str = ">>";
	else if (type == HEREDOC)
		str = "<<";
	data->exit_status = 2;
	report_error3("syntax error near unexpected token `", str, "'");
}

t_token	*classify_input_redirections(t_data *data, t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			next = current->next;
			if (!next)
				return (data->exit_status = 2,
					report_error3("syntax error near unexpected token `",
						"newline", "'"), NULL);
			if (next->type == REDIR_IN || next->type == REDIR_OUT
				|| next->type == REDIR_APPEND)
				return (report_error(data, "ambiguous redirect", -1), NULL);
			if (next->type == PIPE || next->type == HEREDOC)
				return (report_error_token_type(data, next->type), NULL);
			next->type = FILE_NAME;
		}
		current = current->next;
	}
	return (tokens);
}

t_token	*classify_output_redirections(t_data *data, t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			next = current->next;
			if (!next)
				return (data->exit_status = 2, report_error3("syntax error ",
						"near unexpected token `newline", "'"), NULL);
			if (next->type == REDIR_IN || next->type == REDIR_OUT
				|| next->type == REDIR_APPEND || next->type == HEREDOC)
				return (report_error_token_type(data, next->type), NULL);
			if (next->type == PIPE)
				return (report_error_token_type(data, next->type), NULL);
			if (next->type == ARG || next->type == CMD || next->type == LIMITER
				|| next->type == FLAG)
				next->type = FILE_NAME;
		}
		current = current->next;
	}
	return (tokens);
}

t_token	*classify_heredoc_delimiters(t_data *data, t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			next = current->next;
			if (!next)
				return (data->exit_status = 2,
					report_error3("syntax error near unexpected token `",
						"newline", "'"), NULL);
			if (next->type != ARG && next->type != CMD)
				return (data->exit_status = 2,
					report_error3("syntax error near unexpected token `",
						"newline", "'"), NULL);
			next->type = LIMITER;
			next->quote = current->quote;
		}
		current = current->next;
	}
	return (tokens);
}
