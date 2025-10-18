/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:33 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 18:23:25 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	handle_empty_word(t_data *data, char **token_buffer,
		int command_boundary)
{
	char	*empty;

	if (data->emit_empty_word && (!*token_buffer || !**token_buffer))
	{
		empty = ft_strdup("");
		if (!empty)
			return (report_error(NULL, "strdup", -1), EXIT_FAILURE);
		if (!add_classified_token(data, empty, &command_boundary))
			return (EXIT_FAILURE);
		data->emit_empty_word = FALSE;
	}
	if (command_boundary == 1 && (*token_buffer == NULL
			|| **token_buffer == '\0'))
		data->saw_empty_word_as_command = TRUE;
	return (EXIT_SUCCESS);
}

int	lexer_postprocess(t_data *data, t_quote quote_state, char **token_buffer,
		int command_boundary)
{
	if (quote_state != NO_QUOTE)
		return (print_syntax_error('\0', 6), data->exit_status = 2,
			free(*token_buffer), free_tokens(data), EXIT_FAILURE);
	if (*token_buffer && **token_buffer)
	{
		if (!add_classified_token(data, *token_buffer, &command_boundary))
			return (EXIT_FAILURE);
		*token_buffer = NULL;
	}
	if (handle_empty_word(data, token_buffer, command_boundary)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!data->tokens)
		return (EXIT_SUCCESS);
	if (!classify_heredoc_delimiters(data, data->tokens)
		|| !classify_input_redirections(data, data->tokens)
		|| !classify_output_redirections(data, data->tokens))
		return (free(*token_buffer), free_tokens(data), EXIT_FAILURE);
	normalize_exit_echo_args(data->tokens);
	normalize_redirection_args(data->tokens);
	if (validate_pipe_syntax(data) == EXIT_FAILURE)
		return (free(*token_buffer), free_tokens(data), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
