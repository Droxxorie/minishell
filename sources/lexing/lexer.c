/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:29:46 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 12:14:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	lexer_postprocess(t_data *data, t_quote quote_state,
		char **token_buffer, int command_boundary)
{
	if (quote_state != NO_QUOTE)
	{
		print_syntax_error('\0', 6);
		data->exit_status = 2;
		free(*token_buffer);
		free_tokens(data);
		return (EXIT_FAILURE);
	}
	if (*token_buffer && **token_buffer)
	{
		if (!add_classified_token(data, *token_buffer, &command_boundary))
			return (EXIT_FAILURE);
		*token_buffer = NULL;
	}
	if (classify_heredoc_delimiters(data->tokens)
		|| classify_input_redirections(data->tokens))
		return (free(*token_buffer), free_tokens(data), EXIT_FAILURE);
	normalize_exit_echo_args(data->tokens);
	normalize_redirection_args(data->tokens);
	if (validate_pipe_syntax(data) == EXIT_FAILURE)
		return (free(*token_buffer), free_tokens(data), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	scan_line(t_data *data, t_quote *quote_state, char **token_buffer,
		int *command_boundary)
{
	while (*data->line)
	{
		if (quote_state == NO_QUOTE)
		{
			if (handle_no_quote(data, quote_state, token_buffer,
					command_boundary) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else if (*quote_state == SINGLE_QUOTE)
		{
			if (handle_single_quoted(data, quote_state, token_buffer) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else if (*quote_state == DOUBLE_QUOTE)
		{
			if (handle_double_quoted(data, quote_state, token_buffer) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		data->line++;
	}
	return (EXIT_SUCCESS);
}

int	lexer(t_data *data)
{
	t_quote	quote_state;
	char	*token_buffer;
	int		errno;
	int		command_boundary;

	quote_state = NO_QUOTE;
	token_buffer = NULL;
	command_boundary = 1;
	if (scan_line(data, &quote_state, &token_buffer, &command_boundary) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (lexer_postprocess(data, quote_state, &token_buffer,
								command_boundary));
}
