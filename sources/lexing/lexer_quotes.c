/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:32:51 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 17:33:30 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_unquoted_char(t_data *data, char **token_buffer,
		int *command_boundary)
{
	if (ft_iswhitespace(*data->line))
		return (process_whitespace(data, token_buffer, command_boundary));
	if (ft_strchr(SUPPORTED_OPERATORS, *data->line))
		return (process_operator(data, token_buffer, command_boundary));
	if (data->emit_empty_word)
		data->emit_empty_word = FALSE;
	return (ft_append_char(token_buffer, *data->line));
}

int	handle_double_quoted(t_data *data, t_quote *quote_state,
		char **token_buffer)
{
	if (*data->line == '\"')
	{
		*quote_state = NO_QUOTE;
		if (!*token_buffer || !**token_buffer)
			data->emit_empty_word = TRUE;
	}
	else
	{
		if (ft_append_char(token_buffer, *data->line) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		data->current_quote = DOUBLE_QUOTE;
	}
	return (EXIT_SUCCESS);
}

int	handle_single_quoted(t_data *data, t_quote *quote_state,
		char **token_buffer)
{
	if (*data->line == '\'')
	{
		*quote_state = NO_QUOTE;
		if (!*token_buffer || !**token_buffer)
			data->emit_empty_word = TRUE;
	}
	else
	{
		if (ft_append_char(token_buffer, *data->line) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		data->current_quote = SINGLE_QUOTE;
	}
	return (EXIT_SUCCESS);
}

int	handle_no_quote(t_data *data, t_quote *quote_state, char **token_buffer,
		int *command_boundary)
{
	if (data->line[0] == '\'' && data->line[1] == '\'')
	{
		data->emit_empty_word = TRUE;
		data->line++;
		return (EXIT_SUCCESS);
	}
	if (data->line[0] == '\"' && data->line[1] == '\"')
	{
		data->emit_empty_word = TRUE;
		data->line++;
		return (EXIT_SUCCESS);
	}
	if (*data->line == '\'')
		*quote_state = SINGLE_QUOTE;
	else if (*data->line == '\"')
		*quote_state = DOUBLE_QUOTE;
	else
	{
		if (process_unquoted_char(data, token_buffer,
				command_boundary) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		data->current_quote = NO_QUOTE;
	}
	return (EXIT_SUCCESS);
}
