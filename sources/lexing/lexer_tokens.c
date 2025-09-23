/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:13:17 by eraad             #+#    #+#             */
/*   Updated: 2025/09/23 17:34:33 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



t_token *add_classified_token(t_data *data, char *token_buffer, int *command_boundary)
{
	t_type type;
	t_token *new_token;

	if (token_buffer[0] == '-')
		type = FLAG;
	else
		type = ARG;
	if (*command_boundary)
	{
		type = CMD;
		*command_boundary = 0;
	}
	new_token = create_token(token_buffer, token_buffer + ft_strlen(token_buffer), NO_QUOTE); //TODO
	if (!new_token)
		return (NULL);
	add_token(data, new_token);
	return (new_token);
}
