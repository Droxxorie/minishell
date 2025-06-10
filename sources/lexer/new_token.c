/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:49:55 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 16:15:40 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/**
 * @brief Creates a new token with the specified type and value.
 *
 * @param type The type of the token to create.
 * @param value The value of the token to create. If NULL, the token's value will be set to NULL.
 * 
 * @return A pointer to the newly created token, or NULL if memory allocation fails.
 */
t_token *new_token(t_token_type type, const char *value)
{
	t_token = token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value == NULL)
		token->value = NULL;
	else
		token->value = ft_strdup(value);
	//TODO: Check if ft_strdup returns NULL and handle the error accordingly
	token->next = NULL;
	return (token);
}
