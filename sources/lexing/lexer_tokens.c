/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:13:17 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 00:41:31 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*init_new_token(t_type type, t_quote quote)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->quote = quote;
	new_token->value = NULL;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

t_token	*create_token(char *start, char *end, t_type type, t_quote quote)
{
	t_token	*new_token;
	size_t	length;

	new_token = init_new_token(type, quote);
	if (!new_token)
	{
		report_error(NULL, "malloc", -1);
		return (NULL);
	}
	if (start && end && end >= start)
	{
		length = end - start;
		new_token->value = ft_substr(start, 0, length);
		if (!new_token->value)
		{
			report_error(NULL, "malloc", -1);
			free(new_token);
			return (NULL);
		}
	}
	return (new_token);
}

void	add_back_token(t_data *data, t_token *new_token)
{
	t_token	*temp;

	if (!data->tokens)
	{
		new_token->prev = NULL;
		data->tokens = new_token;
		return ;
	}
	else
	{
		temp = data->tokens;
		while (temp->next)
			temp = temp->next;
		new_token->prev = temp;
		temp->next = new_token;
	}
}

t_token	*add_classified_token(t_data *data, char *token_buffer,
		int *command_boundary)
{
	t_type	type;
	t_token	*new_token;

	if (!token_buffer || !*token_buffer)
		return (NULL);
	if (token_buffer[0] == '-')
		type = FLAG;
	else
		type = ARG;
	if (*command_boundary)
	{
		type = CMD;
		*command_boundary = 0;
	}
	new_token = create_token(token_buffer, token_buffer
			+ ft_strlen(token_buffer), type, data->current_quote);
	free(token_buffer);
	if (!new_token)
		return (NULL);
	add_back_token(data, new_token);
	return (new_token);
}
