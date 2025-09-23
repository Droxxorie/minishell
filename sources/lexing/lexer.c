/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:29:46 by eraad             #+#    #+#             */
/*   Updated: 2025/09/23 17:00:34 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int scan_line(t_data *data, t_quote *quote_state, char **token_buffer, int command_boundary)
{
	while (*data->line)
	{
		if (quote_state == NO_QUOTE)
		{
			if (handle_unquoted(data, quote_state, token_buffer, &command_boundary)) //TODO
				return (EXIT_FAILURE);
		}
		else if (quote_state == SINGLE_QUOTE)
		{
			if (handle_single_quoted(data, quote_state, token_buffer)) //TODO
				return (EXIT_FAILURE);
		}
		else if (quote_state == DOUBLE_QUOTE)
		{
			if (handle_double_quoted(data, quote_state, token_buffer)) //TODO
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
	if (scan_line(data, &quote_state, &token_buffer, &command_boundary)) //TODO
		return (EXIT_FAILURE);
	return (finalize_tokens(data, quote_state, &token_buffer,
			command_boundary)); //TODO
}
