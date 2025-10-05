/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:47:43 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 19:07:15 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_unquoted_char(t_data *data, char **token_buffer,
		int *command_boundary)
{
	if (ft_iswhitespace(*data->line))
	{
		if (*token_buffer && **token_buffer)
		{
			if (add_classified_token(data, *token_buffer, command_boundary))
				return (EXIT_FAILURE);
			*token_buffer = NULL;
		}
	}
	else if (ft_strchr(SUPPORTED_OPERATORS, *data->line))
	{
		if (*token_buffer && **token_buffer)
		{
			if (add_classified_token(data, *token_buffer, command_boundary))
				return (EXIT_FAILURE);
			*token_buffer = NULL;
		}
		if (add_operator_token(data, *data->line, command_boundary))
			return (EXIT_FAILURE);
	}
	else
		*token_buffer = ft_append_char(*token_buffer, *data->line);
	if (!*token_buffer)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	handle_no_quote(t_data *data, t_quote *quote_state, char **token_buffer,
		int *command_boundary)
{
	if (*data->line == '\'')
		*quote_state = SINGLE_QUOTE;
	else if (*data->line == '\"')
		*quote_state = DOUBLE_QUOTE;
	else
	{
		if (process_unquoted_char(data, token_buffer, command_boundary) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_single_quoted(t_data *data, t_quote *quote_state,
		char **token_buffer)
{
	if (*data->line == '\'')
		*quote_state = NO_QUOTE;
	else
	{
		*token_buffer = ft_append_char(*token_buffer, *data->line);
		if (!*token_buffer)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_double_quoted(t_data *data, t_quote *quote_state,
		char **token_buffer)
{
	if (*data->line == '\"')
		*quote_state = NO_QUOTE;
	else
	{
		*token_buffer = ft_append_char(*token_buffer, *data->line);
		if (!*token_buffer)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
