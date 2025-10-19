/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 17:29:43 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 17:34:30 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_whitespace(t_data *data, char **token_buffer, int *command_boundary)
{
	char	*empty;

	if (data->emit_empty_word && (!*token_buffer || !**token_buffer))
	{
		empty = ft_strdup("");
		if (!empty)
			return (report_error(NULL, "strdup", -1), EXIT_FAILURE);
		if (!add_classified_token(data, empty, command_boundary))
			return (EXIT_FAILURE);
	}
	data->emit_empty_word = FALSE;
	if (*token_buffer && **token_buffer)
	{
		if (!add_classified_token(data, *token_buffer, command_boundary))
			return (EXIT_FAILURE);
		*token_buffer = NULL;
	}
	return (EXIT_SUCCESS);
}

int	process_operator(t_data *data, char **token_buffer, int *command_boundary)
{
	char	*empty;

	if (data->emit_empty_word && (!*token_buffer || !**token_buffer))
	{
		empty = ft_strdup("");
		if (!empty)
			return (report_error(NULL, "strdup", -1), EXIT_FAILURE);
		if (!add_classified_token(data, empty, command_boundary))
			return (EXIT_FAILURE);
	}
	data->emit_empty_word = FALSE;
	if (*token_buffer && **token_buffer)
	{
		if (!add_classified_token(data, *token_buffer, command_boundary))
			return (EXIT_FAILURE);
		*token_buffer = NULL;
	}
	if (add_operator_token(data, *data->line, command_boundary) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
